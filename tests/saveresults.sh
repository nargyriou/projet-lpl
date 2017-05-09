#!/bin/bash

EXEC='./main'

for param in "$@"
do
	printf "$param: "
	testName="$(echo -e $param | grep -Po '(.*)\.')"

	if ! test -x $EXEC; then
		echo "Erreur: $EXEC commande inconnue" 1>&2
		exit 1
	fi

	echo "cat ${testName}test"
	cat ${testName}test | $EXEC 2>/dev/null 1> ${testName}result
done