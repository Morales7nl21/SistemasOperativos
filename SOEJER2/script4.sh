#!/bin/bash
#El caracter * se le conoce como globing del archivo
IFS=:
if [ -d "$1" ]; then
	echo -e "$1\n Existe"
	echo "Es  un directorio"
	if [ -w "$1" ]; then 
		echo "Tiene permisos de escritura"
	fi
	if [ -x "$1" ]; then
		echo "Tiene permisos de ejecución"
	fi
	if [ -r "$1" ]; then
		echo "Tiene permisos de lectura"
	fi
elif [ -f "$1" ]; then 
	echo -e "\n$1: Existe"
	echo "$1\n Es un archivo regular"
	if [ -w "$1" ]; then 
                echo "Tiene permisos de escritura"
	fi

        if [ -x "$1" ]; then
                echo "Tiene permisos de ejecución"
	fi
        if [ -r "$1" ]; then
                echo "Tiene permisos de lectura"
        fi

else
	echo "$var no existe"

fi
