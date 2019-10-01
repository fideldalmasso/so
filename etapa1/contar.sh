#!/bin/bash

if ! man $1 > /dev/null 2>&1
then 
	echo "No existe una página de man para $1"
else

	man $1 | grep '^\s*[aAeEiIoOuU]' > vocales.txt
	man $1 | grep '^\s*[0-9]' > numeros.txt

	if [ -s vocales.txt ]
	then
		echo "Archivo que contiene vocales:"
		num=($(wc -l vocales.txt))
		echo "Cantidad de líneas: ${num[0]}"
		num=($(wc -w vocales.txt))
		echo "Cantidad de palabras: ${num[0]}"
		num=($(wc -m vocales.txt))
		echo -e "Cantidad de caracteres: ${num[0]}\n"
	else
		echo "El archivo que contiene vocales está vacío"
	fi

	if [ -s numeros.txt ]
	then
		echo "Archivo que contiene números:"
		num=($(wc -l numeros.txt))
		echo "Cantidad de líneas: ${num[0]}"
		num=($(wc -w numeros.txt))
		echo "Cantidad de palabras: ${num[0]}"
		num=($(wc -m numeros.txt))
		echo "Cantidad de caracteres: ${num[0]}"
	else
		echo "El archivo que contiene números está vacío"
	fi
	
fi
