#/bin/bash
#num1=$1
#num2=$2

function suma
{
	a=`expr $1 + $2`
	echo "la suma de $1 y $2 es  $a"
	if [ $1 -gt $2 ] 
	then
		echo "$1 es mayor que $2"
	else
		echo "$2 es mayor que $1"
	fi
}


echo "El numero d eargumentos es de: $#"
echo "los argumentos son: $*"     #@
echo "el resultado es de 1 : $1"
suma $1 $2

#echla suma es: $num1+$num2" 
