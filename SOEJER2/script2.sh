#!/bin/bash 
IFS=:
count=0;
var=0;

#Con -z busca la cadena con longitud 0 con -n busca un valor mayor que 0
if [ -z $@ ]; then
	echo "Introduce algo, para poder ejecutar no puede ser 0 la longitud"
	
fi
for i in $@;
do
	count=`expr $count + 1`;
done 

echo "La longitud es $count"



