#!/bin/bash
echo "Bienvenido usuario: $USERNAME"

echo "Usted se ha conectado en el día $(date +%d/%m/%Y)"

echo "A la hora: $(date +%Hhs:%Mm:%Ss)"

echo "En la terminal: $TERM del host: $HOSTNAME"

echo "Se encuentran conectados en este momento, los siguientes usuarios:"
echo -e "\r$(who)"

echo "Su path es:"
echo $PATH

export PS1="\D{%Y}-\H:\w#"
