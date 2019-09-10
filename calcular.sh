#!/bin/bash

operacion=$1
num1=0
num2=0
read -p "Ingrese el nro1 --> " num1
read -p "Ingrese el nro2 -> " num2

if [ $operacion == "suma" ] 
then
	resultado=$(( $num1 + $num2 ))
	echo "$num1 + $num2 = $resultado"
else
	if [ $operacion == "resta" ] 
	then 
		resultado=$(( $num1 - $num2 ))
		echo "$num1 - $num2 = $resultado"
	else
		if [ $operacion == "multiplicacion" ] 
		then 
			resultado=$(( $num1 * $num2 )) 
			echo "$num1 * $num2 = $resultado"
		else
			if [ $operacion == "division" ]
			then
				if [ $num2 == 0 ]
				then
					echo "La division por 0 no esta definida"
				else
					resultado=$(( $num1 / $num2 ))
					echo "$num1 / $num2 = $resultado"
				fi
			fi
			
		fi
	fi
fi
