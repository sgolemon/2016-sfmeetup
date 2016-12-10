dnl config.m4
PHP_ARG_ENABLE(hello)

if test "$PHP_HELLO" != "no"; then
  PHP_NEW_EXTENSION(hello, hello.c, $ext_shared)
fi
