#!/bin/bash

directorio=$1

while [ ! -d $directorio ]
do
read -p "$directorio no es un directorio válido, ingrese uno correcto:" directorio
done

rm  -f $directorio/dir1 $directorio/dir2
mkdir $directorio/dir1 $directorio/dir2

#Ordenar de la Z a la A

head /etc/passwd | sort -r > $directorio/dir1/ordenadoTP_dir1
tail -n 5 /etc/passwd | sort -r > $directorio/dir2/ordenadoTP_dir2

#Ordenar de abajo hacia arriba
#head /etc/passwd | tac > $directorio/dir1/ordenadoTP_dir1
#tail -n 5 /etc/passwd | tac > $directorio/dir2/ordenadoTP_dir2

salida=$(wc -l /etc/passwd)

echo "Cantidad de líneas del archivo /etc/passwd: "$(echo $salida | cut -d " " -f1)
