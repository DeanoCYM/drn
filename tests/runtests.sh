#!/bin/bash
DATE="$(date)"
LOG=tests/log
RC=0

# MEM_CHECK ()
# Use valgrind memcheck tool to check for memory leaks
#
# $1   - binary
# $@:2 - command line arguments to binary
#
# Returns - 33 on memory error or the binary exit code
# 
function mem_check () {
    catargs="${@:2}"
    printf "\nTESTING %s %s...\n" "$1" "$catargs" >> $LOG
    printf "Testing %s %s... " "$1" "$catargs"
    
    valgrind --leak-check=full --errors-for-leak-kinds=all                     \
    	     --error-exitcode=33 --quiet "$1" "${@:2}" >> $LOG 2>&1

    return $?
}

# RC_CHECK ()
# Check the exit code for expected value
#
# $1 - exit code (int)
# $2 - expected exit code (int)
#
# Returns - 0 on pass, 1 on fail
# 
function rc_check () { 

    if [ $1 -eq 33 ]; then
	printf "[FAIL]: Memory error\n"
	return 1

    elif [ $1 -eq $2 ]; then
	printf "[PASS]\n"

    else
	printf "[FAIL]: error code $1\n"
	return 1;
    fi

    return 0;
}

printf "Test log [%s]\n" "$DATE" > $LOG

mem_check "tests/drn_sll_test" "string1" "string2" # linked list object
rc_check $? 0
(( RC+= $? ))

mem_check "bin/drn" 		# no args
rc_check $? 1
(( RC+= $? ))

mem_check "bin/drn" "|"		# one arg
rc_check $? 1
(( RC+= $? ))

mem_check "bin/drn" "|" "drn_test1" # 3 args
rc_check $? 0
(( RC+= $? ))

mem_check "bin/drn" "|" "drn_test1" "drn_test2" "drn_test3" # 5 args
rc_check $? 0
(( RC+= $? ))

mem_check "bin/drn" " " "drn_test1" "drn_test2" "drn_test3" # 5 args
rc_check $? 0
(( RC+= $? ))

printf "Testing complete... $RC test(s) failed\n"

printf "Opening log in $LOG\n\n"
sleep 2
less -RF $LOG

exit $RC;			# returns number of errors

# "\e[1;31mFail\e[0m\n"
# "\e[1;32mPass\e[0m\n"
