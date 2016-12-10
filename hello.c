/* hello.c */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"

zend_module_entry hello_module_entry = {
    STANDARD_MODULE_HEADER,
    "hello",
    NULL, NULL, NULL, NULL, NULL, NULL,
    NO_VERSION_YET,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HELLO
ZEND_GET_MODULE(hello)
#endif
