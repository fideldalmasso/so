#!/bin/bash

anexar(){
	cat $1 >> doc.md
}
anexarej(){
	#echo -e "#\c" >> doc.md
	echo  "## $1:" >> doc.md
	echo '```c'>> doc.md
	cat $1 >> doc.md
	#echo ' ' >> doc.md
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
cat caratula.md > doc.md
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
echo '## Salida eco.c' >> doc.md
echo '![captura](captura.jpg){ width=400px } \' >> doc.md
echo ' ' >> doc.md
echo '## Esquematización en forma de árbol' >> doc.md
echo 'Cantidad de procesos generados: 8' >> doc.md
echo ' ' >> doc.md

echo '![arbol](arbol.jpg){ width=400px } \' >> doc.md
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
salto
titulo 'Descargos'
anexar descargos.md
salto
titulo 'Bibliografía'
anexar bibliografia.md

#-----------------------------------------
pandoc  doc.md metadata.yaml --pdf-engine=xelatex   -o doc.pdf -f markdown+implicit_figures
#pandoc doc.md  --pdf-engine=xelatex  -o doc2.pdf