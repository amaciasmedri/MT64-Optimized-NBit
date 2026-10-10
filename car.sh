#!/bin/sh
if [ -z $1 ];then exit 1;fi
if [ -z $2 ];then exit 2;fi
\rm -f $2.x
# -Warray-bounds=0
FLAGS='-Wall -Wextra -pedantic -O3 -march=native -mtune=native -funroll-loops -flto=auto -pipe -msahf'
FMATH='-ffast-math -funsafe-math-optimizations -fno-math-errno -fno-trapping-math'
FL2TW='-Wno-char-subscripts -Wno-unused-parameter -Wno-pedantic -Wno-register'
if [ $1 -eq 0 ]; then
	gcc $FLAGS $FMATH $FL2TW $2 -o $2.x
else
	g++ -std=c++$1 $FLAGS $FMATH $2 -o $2.x
fi
time ./$2.x $3 $4 $5 $6 $7 $8 $9 ${10}
#\rm -f $2.x
