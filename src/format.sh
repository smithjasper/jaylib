#!/usr/bin/env sh

# Format all code with astyle

STYLEOPTS="--style=attach --indent-switches --convert-tabs \
		  --align-pointer=name --pad-header --pad-oper --unpad-paren --indent-labels --formatted"

astyle $STYLEOPTS ./*.c
astyle $STYLEOPTS ./*.h

rm ./*.h.orig
rm ./*.c.orig
