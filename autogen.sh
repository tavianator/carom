#! /bin/sh

aclocal
autoconf
libtoolize --copy --force
automake --copy --add-missing
