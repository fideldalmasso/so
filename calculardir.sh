#!/bin/bash

directorio=$1

while [ ! -d $directorio ]
do
read -p "$directorio no es un directorio valido, ingrese uno correcto:" directorio
done

if [ -r $directorio ]
then
  total=0
  for i in $directorio
    do
    echo $i
    aux=$(stat -c %s $i)
    total=$((total + $aux))
    done
  echo "Total directorio $directorio = $total"
else
echo "Error: No tienes permisos para leer este directorio"
fi
