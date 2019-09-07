#!/bin/bash

if [ -z $1 ]
then
echo "ERROR: Este script necesita un parámetro"
exit 1
fi


ps
find / -type f -name "c*" &> $1 &
echo "Se está generando una lista de archivos que comienzan con c"
ps
read -p 'Ingrese el id del proceso de búsqueda de archivos que inician con letra "c":' id

while [ $! != $id ]
	do	
	read -p "El id ingresado no es el solicitado, ingrese uno correcto:" id
done

#if kill -0 $!
#if pgrep -x "find" > /dev/null
if ps -p $id > /dev/null
	then
	kill "$id"
	echo "Usted finalizó el proceso con éxito"
	else
	echo "Usted ingresó el id correcto, pero el proceso finalizó antes"
fi