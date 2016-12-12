/* hello.c */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"

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

zend_function_entry hello_functions[] = {
    PHP_FE(hello_world, NULL)
    PHP_FE(hello_return, NULL)
    PHP_FE(hello_number, NULL)
    PHP_FE(hello_you, NULL)
    PHP_FE(hello_everyone, NULL)
    PHP_FE_END
};

zend_module_entry hello_module_entry = {
    STANDARD_MODULE_HEADER,
    "hello",
    hello_functions,
    NULL, NULL, NULL, NULL, NULL,
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HELLO
ZEND_GET_MODULE(hello)
#endif
