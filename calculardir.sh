#!/bin/bash

directorio=$1

while [ ! -d $directorio ]
do
read -p "$directorio no es un directorio válido, ingrese uno correcto:" directorio
done

total=0
if [ -r $directorio ]
then
  for i in ${directorio}*
    do
	echo $i
    aux=$(stat -c %s $i)
	echo $aux
    total=$(($total + $aux))
    done
  echo "Total directorio $directorio = $total"
else
echo "Error: No tienes permisos para leer este directorio"
fi
