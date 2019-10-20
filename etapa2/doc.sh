#!/bin/bash

anexar(){
	cat $1 >> doc.md
}
anexarej(){
	#echo -e "#\c" >> doc.md
	echo  "## $1:" >> doc.md
	echo '```c'>> doc.md
	cat $1 >> doc.md
	echo ' ' >> doc.md
	echo '```'>> doc.md
}
salto(){
	echo '\pagebreak' >> doc.md
	echo ' ' >> doc.md
}
ejercicio(){
	echo  "# Ejercicio $1:" >> doc.md
}
titulo (){
	echo  "# $1" >> doc.md
	#echo  "___" >> doc.md
}

#-----------------------------------------
cat caratula.txt > doc.md
salto
titulo 'Argumentos por línea de comandos (argv - argc - opciones)'
ejercicio 1
anexarej rectangulo.c 
salto
titulo 'Gestión de procesos'
ejercicio 2
anexarej tejidos.c 
anexarej celulas.c 
ejercicio 3
anexarej produc.c
ejercicio 4
anexarej eco.c
salto
titulo 'Gestión de archivos, directorios y sistemas de archivos + señales'
ejercicio 5
anexarej buscador.c
anexarej monitor.c
salto
titulo 'Redirección de entrada y salida'
ejercicio 6
anexarej isbn.c
anexarej editorial.c
anexarej imprenta.c







#-----------------------------------------
pandoc  doc.md metadata.yaml --pdf-engine=xelatex  -o doc.pdf
#pandoc doc.md  --pdf-engine=xelatex  -o doc2.pdf