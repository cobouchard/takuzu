#!/usr/bin/env bash

tests_folder="tests"
cant_parse_folder="$tests_folder/parse_error"
solvable_folder="$tests_folder/solvable"
unsolvable_folder="$tests_folder/unsolvable"
inconsistent_folder="$tests_folder/inconsistent"
generated_folder="$tests_folder/generated"
executable="takuzu"
VALGRIND_CMD="valgrind --leak-check=full --error-exitcode=7"

if [ ! -f $executable ]; then
	echo "$executable executable not found, please use make before"
	exit 1
fi


echo "Testing takuzu project, this might takes some time (~20s)." 
echo "You can stop it and restart if it's taking too long"
echo ""



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
####


#### Testing grids than cannot be solved
count_unsolvable=0
success=0
for file in "$unsolvable_folder"/*
do
	./$executable $file > /dev/null 2>&1
	if [ $? -eq 1 ]; then
		((success++))
	else
		echo "$file has exited without error, should be unsolvable"
	fi
	((count_unsolvable++))
done

echo "Executed all files in $unsolvable_folder ; $success out of $count_unsolvable exited with an error as intended"
test_count=$((test_count+count_unsolvable))
test_success=$((test_success+success))
####



#### Testing generator
count_generator=0
success=0
for i in {4,8,16,32,64}
do
	if [ $i -le "9" ]; then
		./$executable -g $i -o "$solvable_folder/generated$i" > /dev/null 2>&1
	else
		./$executable -g $i > /dev/null 2>&1
	fi

	
	if [ $? -eq 0 ]; then
		((success++))
	else
		echo "Generating grid for N=$i has exited with an error"
	fi
	((count_generator++))
done
echo "Generated grids for N = 4,8,16,32,64 ; $success out of $count_generator exited without any errors"
test_count=$((test_count+count_generator))
test_success=$((test_success+success))
####


#### Testing grids than can be solved
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


#### Testing Valgrind
count_valgrind=0
success=0
for argument in "-g 4" "-g 32" "-g 8 -o $generated_folder/generated1.txt" "$solvable_folder/size8.txt" "-a $solvable_folder/severalsolutions.txt" "$cant_parse_folder/size5.txt" "$inconsistent_folder/identical_lines.txt" "$unsolvable_folder/nosolution.txt" "$generated_folder/generated1" "-h" "-a -u" "anything --nothing"
do
	$VALGRIND_CMD ./$executable $argument > /dev/null 2>&1
	if [ $? -ne 7 ]; then
		((success++))
	else
		echo "Valgrind found memory leaks executing $VALGRIND_CMD ./$executable $argument"
	fi
	((count_valgrind++))
done

test_count=$((test_count+count_valgrind))
test_success=$((test_success+success))
echo "Valgrind tested arguments for $executable ; $success out of $count_valgrind without any memory leaks"

echo ""
echo "$test_success / $test_count tests passed correctly"

for i in {4,8,16}
do
	if [ -f $solvable_folder/generated$i ]; then
		rm $solvable_folder/generated$i
	fi
done

if [ -f $generated_folder/generated1.txt ]; then
	rm $generated_folder/generated1.txt
fi
