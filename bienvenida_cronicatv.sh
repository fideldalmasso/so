#!/bin/bash

echo Hola $1, Ud. esta logueado como usuario $(whoami)

dia=$(date +%A)
numero=$(date +%d)
mes=$(date +%B)
anio=$(date +%Y)


echo Hoy es $dia, $numero de $mes de $anio,

invierno=172
inviernofuturo=$((172+365))
primavera=264
diaactual=$(date +%j)
diasrestantes=0

if [ $diaactual -lt $invierno ] 
then
diasrestantes=$(($invierno - $diaactual)) 
else
	if [ $diaactual -lt $primavera ] 
	then
	diasrestantes=0
	else
	diasrestantes=$(($inviernofuturo - $diaactual))
	fi
fi

echo ULTIMO MOMENTO: faltan $diasrestantes para el invierno!
