/* hello.c */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"

#ifdef HAVE_CURL_EASY_H
# include <curl/curl.h>
#endif

zend_class_entry *php_hello_curl_ce;
zend_object_handlers phc_handlers;

typedef struct _phc_object {
    CURL *handle;
    zend_object std; // Keep this last
} phc_object;

#define FETCH_CURLOBJ(zo) (((phc_object*)(zo + 1)) - 1)

PHP_FUNCTION(hello_world) {
    php_printf("Hello World!\n");
}

PHP_FUNCTION(hello_return) {
    RETURN_STRING("Hello World!\n");
}

PHP_FUNCTION(hello_number) {
    RETURN_LONG(42);
}

PHP_FUNCTION(hello_you) {
    char *name;
    size_t name_len; /* Note: In PHP5, this is of type int */

    if (zend_parse_parameters(ZEND_NUM_ARGS(),
                              "s", &name, &name_len) == FAILURE) {
        return;
    }

    /* Easy way: php_printf("Hello %s!\n", name); */

    /* Binary safe way: */
    php_printf("Hello ");
    PHPWRITE(name, name_len);
    php_printf("!\n");
}

PHP_FUNCTION(hello_everyone) {
    zval *zarr, *name;
    zend_bool greet = 1;
    const char *salutation = "Hello";

    if (zend_parse_parameters(ZEND_NUM_ARGS(),
                              "a|b", &zarr, &greet) == FAILURE) {
        return;
    }

    if (!greet) salutation = "Goodbye";
    ZEND_HASH_FOREACH_VAL(Z_ARR_P(zarr), name)
        if (Z_TYPE_P(name) == IS_STRING) {
            php_printf("%s %s!\n", salutation, Z_STRVAL_P(name));
        }
    ZEND_HASH_FOREACH_END();
}

PHP_FUNCTION(hello_greetings) {
    array_init(return_value);
    add_next_index_string(return_value, "Hello");
    add_index_string(return_value, 1, "Goodbye");
}

zend_function_entry hello_functions[] = {
    PHP_FE(hello_world, NULL)
    PHP_FE(hello_return, NULL)
    PHP_FE(hello_number, NULL)
    PHP_FE(hello_you, NULL)
    PHP_FE(hello_everyone, NULL)
    PHP_FE(hello_greetings, NULL)
    PHP_FE_END
};

size_t phc_write(char *ptr, size_t size, size_t nmemb, void *ctx) {
    PHPWRITE(ptr, size * nmemb);
}

PHP_METHOD(HelloCurl, __construct) {
    phc_object *phc = FETCH_CURLOBJ(Z_OBJ_P(getThis()));
    zend_string *url = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "|S!", &url) == FAILURE) {
        return;
    }

    phc->handle = curl_easy_init();
    curl_easy_setopt(phc->handle, CURLOPT_WRITEFUNCTION, phc_write);
    if (url) {
        curl_easy_setopt(phc->handle, CURLOPT_URL, ZSTR_VAL(url));
    }
}

PHP_METHOD(HelloCurl, setUrl) {
    zend_string *url;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S", &url) == FAILURE) {
        return;
    }

    curl_easy_setopt(FETCH_CURLOBJ(Z_OBJ_P(getThis()))->handle, CURLOPT_URL, ZSTR_VAL(url));
}

PHP_METHOD(HelloCurl, perform)  {
    curl_easy_perform(FETCH_CURLOBJ(Z_OBJ_P(getThis()))->handle);
}

zend_function_entry php_hello_curl_methods[] = {
    PHP_ME(HelloCurl, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(HelloCurl, setUrl, NULL, ZEND_ACC_PUBLIC)
    PHP_ME(HelloCurl, perform, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

zend_object *phc_create(zend_class_entry *ce) {
    phc_object *ret = ecalloc(1, sizeof(phc_object) + zend_object_properties_size(ce));
    zend_object *obj = &(ret->std);
    zend_object_std_init(obj, ce);
    obj->handlers = &phc_handlers;
    object_properties_init(obj, ce);
    return obj;
}

void phc_free(zend_object *obj) {
    phc_object *phc = ((phc_object*)(obj + 1)) - 1;
    if (phc->handle) curl_easy_cleanup(phc->handle);
}

PHP_MINIT_FUNCTION(hello) {
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "Hello\\cURL", php_hello_curl_methods);
    php_hello_curl_ce = zend_register_internal_class(&ce);
    php_hello_curl_ce->create_object = phc_create;
    memcpy(&phc_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    phc_handlers.offset = XtOffsetOf(phc_object, std);
    phc_handlers.dtor_obj = phc_free;
    phc_handlers.clone_obj = NULL;

#ifdef HAVE_CURL_EASY_H
    REGISTER_BOOL_CONSTANT("HELLO_CURL", 1, CONST_CS | CONST_PERSISTENT);
    REGISTER_STRING_CONSTANT("HELLO_CURL_VERSION", curl_version(), CONST_CS | CONST_PERSISTENT);
#else
    REGISTER_BOOL_CONSTANT("HELLO_CURL", 0, CONST_CS | CONST_PERSISTENT);
#endif

    return SUCCESS;
}

zend_module_entry hello_module_entry = {
    STANDARD_MODULE_HEADER,
    "hello",
    hello_functions,
    PHP_MINIT(hello),
    NULL, NULL, NULL, NULL,
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HELLO
ZEND_GET_MODULE(hello)
#endif
