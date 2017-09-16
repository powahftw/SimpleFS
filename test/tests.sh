#!/bin/bash

cd "$(dirname "$0")"

normal=$(tput sgr0)
red=$(tput setaf 1)
green=$(tput setaf 2)

printf "Running tests:\n"
i=1
tot=$(ls cases/*.input -1 | wc -l)
for input in cases/*.input; do
    tname=$(basename "$input" .input)
    out=$(cat cases/$tname.input | ../comp.out | diff cases/$tname.output -)
    if [ $? -eq 0 ]; then
        printf "[${green}  OK  ${normal}] ${i}/${tot}: $tname\n"
    else
        printf "[${red}FAILED${normal}] ${i}/${tot}: $tname\n\n"
        printf "Output from diff:\n"
		printf "%s\n" "$out"
		exit 1
    fi
    ((i++))
done
