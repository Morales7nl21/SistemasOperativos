#/bin/bash
array[0]=15
array[1]=20
array[3]=40
echo "El tamaño del arreglo es: ${#array[*]}"
echo "El contenido del arreglo es: ${array[*]}"


echo "El primer valor del  arreglo es: ${array[0]}"
for i in `seq 0 5`
do
array2[$i]=$i
done

for i in `seq 0 5`
do
echo "El contenido del arreglo en la posicion $i es: ${array2[$i]}"
done



