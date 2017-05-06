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
	cat ${testName}test | $EXEC 2>/dev/null | tee tmp
	result=$(diff --unchanged-line-format= --old-line-format='    [%dn]Got     : %L' --new-line-format='    [%dn]Expected: %L' tmp ${testName}results)
	if [ $? -eq 0 ]; then
		printf "\033[32mSUCCESS\033[0m\n"
	else
		printf "\033[31mFAIL\033[0m\n"
		echo "$result"
		exit 1
	fi
done