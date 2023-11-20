#!/usr/bin/env bash

cant_parse_folder="parse_error"
solvable_folder=solvable
executable="../takuzu"

if [ "$(basename "$(pwd)")" != "tests" ]; then
	echo "This script has to be executed from the tests folder."
	exit 1
fi

echo "Testing files that can't be parsed"
count_parse=0
success=0
for file in "$cant_parse_folder"/*
do
	$executable $file > /dev/null 2>&1
	if [ $? -eq 1 ]; then
		((success++))
	else
		echo "$file has not exited with an error"
	fi
	((count_parse++))
done

echo "Executed all files in $cant_parse_folder ; $success out of $count_parse exited with an error as intended"



echo "Testing files with correct grids"
count_solvable=0
success=0
for file in "$solvable_folder"/*
do
	$executable $file > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		((success++))
	else
		echo "$file has exited with an error"
	fi
	((count_solvable++))
done

echo "Executed all files in $solvable_folder ; $success out of $count_solvable exited without any errors"
