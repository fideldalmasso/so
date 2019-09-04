#!/bin/bash

directorio=$1

while [ ! -d $directorio ]
do
read -p "$directorio no es un directorio valido, ingrese uno correcto:" directorio
done

if [ -r $directorio ]
then
echo "Total directorio $directorio = $(du -hs $directorio | cut -f1)"
else
echo "Error: No tienes permisos para leer este directorio"
fi
