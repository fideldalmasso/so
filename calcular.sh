#!/bin/bash

operacion=$1
num1=0
num2=0
read -p "Ingrese el nro1 --> " num1
read -p "Ingrese el nro2 -> " num2

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
		if [ $num2 == 0 ]
		then
			echo "La division por 0 no esta definida"
		else
			resultado=$(( $num1 / $num2 ))
			echo "$num1 / $num2 = $resultado"
		fi
	;;
	*)
esac
