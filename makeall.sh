#!/bin/bash

# Makefileのあるディレクトリを検索
srcdir=`find ./ -name "Makefile" -exec dirname {} \;`

for path in ${srcdir}
do
	if [ `echo ${path} | grep 'failtureproject'` ] ; then
		continue
	fi

	echo "start"${path}
	pushd ${path}
	make clean
	make
	popd
	echo "end"
	echo "-------------------------------------------"
	echo ""
done
