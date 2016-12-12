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

zend_function_entry hello_functions[] = {
    PHP_FE(hello_world, NULL)
    PHP_FE(hello_return, NULL)
    PHP_FE(hello_number, NULL)
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
