#!/bin/bash
var="rename2.sh"
#cont=$(ls *.dat | wc -l)
count=0;

termino=$(ls)
for i in $termino;
do
	count=`expr $count + 1`;
	if [ $i != $var ]; then
	mv $i "archivo$count.dat";
	fi
#((cont=$cont+1))
done
