#!/bin/bash
rm aclocal.m4
rm compile
rm config.log
rm configure
rm depcomp
rm install-sh
rm Makefile.in
rm missing
./autogen.sh 
./configure --with-bamtools=/home/iqbal/software/bamtools-master/installed --prefix=/home/iqbal/software/sof

