#!/bin/bash

EXEC=albatrosscc
DUMMY=test
OUTPUT=output
TEST_DIR=tests
TIMEOUT=1s
DIFF_FILE=d
MARS=Mars4_5.jar
JAVA=/usr/bin/java

RED='\033[41;37m'
GREEN='\033[42m'
RESET='\033[0m'

for T in $(ls $TEST_DIR | grep -E "$1" | sort)
do
    PASS=1
    for F in $(ls $TEST_DIR/$T | grep ".albatross$" | grep pass | sort)
    do
        TESTFILE="$TEST_DIR/$T/$F"
        EXPECTED=$(sed 's/.albatross/.expected/g' <<<"$TESTFILE")
        echo -n -e "Generating $EXPECTED"
        echo -n $'\t'
        ./$EXEC $TESTFILE out.mips
        $JAVA -jar $MARS nc out.mips > $EXPECTED
        RET=$?

        if [ $RET -eq 0 ]
        then
            echo -e "${GREEN}RET OK${RESET}"
        else
            echo -e "${RED}RET FAIL${RESET}"
            exit 1
        fi
    done

done

