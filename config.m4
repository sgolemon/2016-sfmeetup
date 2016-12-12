dnl config.m4
PHP_ARG_WITH(hello)

if test "$PHP_HELLO" != "no"; then
  AC_MSG_CHECKING([for libcurl])
  for i in $PHP_HELLO /usr/local /usr; do
    if test -f "$i/include/curl/easy.h"; then
      PHP_CURL_DIR=$i
      break
    fi
  done
  if test -z "$PHP_CURL_DIR"; then
    AC_MSG_RESULT([not found])
  else
    dnl Enable optional cURL features
    AC_MSG_RESULT([found in $PHP_CURL_DIR])
    AC_DEFINE(HAVE_CURL_EASY_H, 1, [Define if libcurl is available])

    PHP_ADD_INCLUDE($PHP_CURL_DIR/include)
    PHP_ADD_LIBRARY_WITH_PATH(curl, $PHP_CURL_DIR/$PHP_LIB_DIR, HELLO_SHARED_LIBADD)
    PHP_SUBST(HELLO_SHARED_LIBADD)
  fi

  PHP_NEW_EXTENSION(hello, hello.c, $ext_shared)
fi
