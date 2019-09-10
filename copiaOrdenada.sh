#!/bin/bash

directorio=$1

while [ ! -d $directorio ]
do
read -p "$directorio no es un directorio valido, ingrese uno correcto:" directorio
done

mkdir $directorio/dir1 $directorio/dir2
head /etc/passwd | sort -r > $directorio/dir1/ordenadoTP_dir1
tail -n 5 /etc/passwd | sort -r > $directorio/dir2/ordenadoTP_dir2

salida=$(wc -l /etc/passwd)
#echo "${salida%% }"
echo $(echo $salida | cut -d" " -f1)
