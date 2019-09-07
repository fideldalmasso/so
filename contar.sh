#!/bin/bash

if man $1 > /dev/null 2>&1
then

man $1 | grep '^\s*[aAeEiIoOuU]' > vocales
man $1 | grep '^\s*[0-9]' > números

else
	echo "No existe una página de man para $1"
fi
