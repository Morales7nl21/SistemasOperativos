#!/bin/bash 
IFS=:
count=0;
var=0;
band=0;
var=$1
var2=$2
#Con -z busca la cadena con longitud 0 con -n busca un valor mayor que 0

if [  $# -lt 2 ]; then
        echo "Introduce mas numeros, para poder ejecutar el script"
fi

if [ $var -gt $var2 ];then
echo -e "$var\n$var2"
else
	echo -e  "$var2\n$var"
fi














