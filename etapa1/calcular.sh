#!/bin/bash

esNumero(){
	if [[ $1 =~ [0-9]+ ]]; 
	then true  
	else false  
	fi
}

pedirReingreso(){
	aux="$1"
		while ! $(esNumero $aux); do
		read -p "ERROR: por favor ingrese un numero --> " aux
		done
	echo $aux
}

#------------------------------------------

if [ -z $1 ]; then echo "ERROR: Este script necesita un argumento" ; exit 1 ; fi

operacion=$1

read -p "Ingrese el nro1 --> " num1
num1=$(pedirReingreso $num1)

read -p "Ingrese el nro2 --> " num2
num2=$(pedirReingreso $num2)

case $operacion in
	suma)
	resultado=$(( $num1 + $num2 ))
	echo "$num1 + $num2 = $resultado"
	;;
	resta)
	resultado=$(( $num1 - $num2 ))
	echo "$num1 - $num2 = $resultado"
	;;
	multiplicacion)
	resultado=$(( $num1 * $num2 )) 
	echo "$num1 * $num2 = $resultado"
	;;
	division)
		if [[ $num2 == 0 ]]
		then
			echo "La división por 0 no está definida"
		else
			resultado=$(( $num1 / $num2 ))
			echo "$num1 / $num2 = $resultado"
		fi
	;;
	*)
	echo "ERROR: la operación ingresada no es válida"
esac