#!/bin/bash

echo "Hola $1, Ud. está logueado como usuario $(whoami)"

dia=$(date +%A)
numero=$(date +%d)
mes=$(date +%B)
anio=$(date +%Y)


echo "Hoy es $dia, $numero de $mes de $anio,"

invierno=172
inviernofuturo=$((172+365))
diaactual=$(date +%j)
diasrestantes=0

if [ $diaactual -lt $invierno ] 
then
diasrestantes=$(($invierno - $diaactual)) 
else
	if [ $diaactual -gt $invierno ] 
	then
	diasrestantes=$(($inviernofuturo - $diaactual))
	fi
fi

echo "ÚLTIMO MOMENTO: faltan $diasrestantes días para el invierno!"
