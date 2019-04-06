#!/bin/bash
DATE="$(date)"
LOG=tests/log
RC=0

exec 2> /dev/null		# stop output to stderr (segfault spam)

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
# $1 - actual exit code (int)
# $2 - expected exit code (int)
# $3 - test description (string)
# 
# Returns - 0 on pass, 1 on fail
# 
function rc_check () { 

    printf "(%s) Exit code: expected %s, actual %s.\n" "$3" "$2" "$1" >> $LOG

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

mem_check "bin/drn"
rc_check $? 1 "No arguements"
(( RC+= $? ))

mem_check "bin/drn" "|"
rc_check $? 1 "One arguement"
(( RC+= $? ))

mem_check "bin/drn" "|" ""
rc_check $? 1 "Second arguement with empty string"
(( RC+= $? ))

mem_check "bin/drn" "|" "notafn"
rc_check $? 1 "Second arguement (of two) not a vaild symbol"
(( RC+= $? ))

mem_check "bin/drn" "|" "notafn" "batt_capacity"
rc_check $? 1 "Second arguement (of three) not a valid symbol"
(( RC+= $? ))

mem_check "bin/drn" "|" "batt_status" "notafn" "alsonotfn" "batt_capacity"
rc_check $? 2 "Third and fourth arguement (of five) not valid symbols"
(( RC+= $? ))

mem_check "bin/drn" " · " "local_time" "batt_capacity" "batt_status"
rc_check $? 0 "Five valid arguements"
(( RC+= $? ))

printf "Testing complete... $RC test(s) failed\n"

printf "Opening log in $LOG\n\n"
sleep 2
less -RF $LOG

exit $RC;			# returns number of errors

# "\e[1;31mFail\e[0m\n"
# "\e[1;32mPass\e[0m\n"
