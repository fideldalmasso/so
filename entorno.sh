#!/bin/bash
echo "Bienvenido usuario: $USERNAME"

fechaLogIn=$(last $USERNAME -F -R -n 1 | head -n 1 | grep -oE '\b(Mon|Tue|Wed|Thu|Fri|Sat|Sun)\b.*2019')

echo "Usted se ha conectado en el día $(date -d "$fechaLogIn" +%d/%m/%Y)"

echo "A la hora: $(date -d "$fechaLogIn" +%Hhs:%Mm:%Ss)"

echo "En la terminal: $TERM del host: $HOSTNAME"

echo "Se encuentran conectados en este momento, los siguientes usuarios:"
echo -e "\r$(who)"

echo "Su path es:"
echo $PATH

export PS1="\D{%Y}-\H:\w#"
