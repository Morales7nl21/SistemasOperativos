#/bin/bash

echo "el bumero  de argu,entos es: $#"
echo "los argumentos son: $* "
echo "El argumento 1 es : $1"
for i in `ls`
do
	echo "Archivo: $i"
done
for i in `seq 0 5`
do
	echo "$i"
done
