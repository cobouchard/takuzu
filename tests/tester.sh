#!/usr/bin/env bash

cant_parse_folder="tests/parse_error"
solvable_folder="tests/solvable"
inconsistent_folder="tests/inconsistent"
executable="takuzu"

if [ "$(basename "$(pwd)")" != "takuzu" ]; then
	echo "This script has to be executed from the root folder of the project."
	exit 1
fi
test_count=0
test_success=0


#### Testing files that will raise an error by the parser
count_parse=0
success=0
for file in "$cant_parse_folder"/*
do
	./$executable $file > /dev/null 2>&1
	if [ $? -eq 1 ]; then
		((success++))
	else
		echo "$file has not exited with an error (parser error expected)"
	fi
	((count_parse++))
done

echo "Executed all files in $cant_parse_folder ; $success out of $count_parse exited with an error as intended"
test_count=$((test_count+count_parse))
test_success=$((test_success+success))
####

#### Testing grids that are not consistent
count_inconsistent=0
success=0
for file in $inconsistent_folder/*
do
	./$executable $file > /dev/null 2>&1
	if [ $? -eq 1 ]; then
		((success++))
	else
		echo "$file has not exited with an error (consistency error expected)"
	fi
	((count_inconsistent++))
done

echo "Executed all files in $inconsistent_folder ; $success out of $count_inconsistent exited with an error as intended"
test_count=$((test_count+count_inconsistent))
test_success=$((test_success+success))
#### Testing grids than can try to be solved
count_solvable=0
success=0
for file in "$solvable_folder"/*
do
	./$executable $file > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		((success++))
	else
		echo "$file has exited with an error"
	fi
	((count_solvable++))
done

echo "Executed all files in $solvable_folder ; $success out of $count_solvable exited without any errors"
test_count=$((test_count+count_solvable))
test_success=$((test_success+success))
####


#### Testing generator
count_generator=0
success=0
for i in {4,8,16,32,64}
do
	./$executable -g $i > /dev/null 2>&1
	if [ $? -eq 0 ]; then
		((success++))
	else
		echo "Generating grid for N=$i has exited with an error"
	fi
	((count_generator++))
done
echo "Generated grids for N = 4,8,16,32,64 ; $success out of $count_generator has exited without any errors"
test_count=$((test_count+count_generator))
test_success=$((test_success+success))
####


#### Testing arguments parser


echo ""
echo "$test_count / $test_success tests passed correctly"