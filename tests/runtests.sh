#!/bin/bash

EXEC='./main'

for param in "$@"
do
	testName="$(echo -e $param | grep -Po '(.*)\.')"

	if ! test -x $EXEC; then
		echo "Erreur: $EXEC commande inconnue" 1>&2
		exit 1
	fi

	cat ${testName}test | $EXEC 2>/dev/null 1> tmp
	result=$(diff --unchanged-line-format= --old-line-format='    [%dn]Got     : %L' --new-line-format='    [%dn]Expected: %L' tmp ${testName}result 2>/dev/null)
	if [ $? -eq 0 ]; then
		printf "\033[32mSUCCESS\033[0m %q\n" "${testName}test"
	else
		printf "\033[31mFAIL\033[0m %q\n" "${testName}test"
		echo "$result"
		exit 1
	fi
	cat tmp
done