#!/bin/bash

# == VARIABLES ==

BASH_OUTPUT=""
BASH_ERROR=""
BASH_RVALUE=0
MINISHELL_OUTPUT=""
MINISHELL_ERROR=""
MINISHELL_RVALUE=0

TESTER_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INFILE_DIR="$TESTER_DIR/infiles"
LOGS_DIR="$TESTER_DIR/logs"

# == FUNCTIONS ==

run_test()
{
    local TEST_NUM=$1
    local OUTPUT_FILE="$LOGS_DIR/test_${TEST_NUM}.txt"
    shift

    # Create a logs directory in the script's location
    mkdir -p "$LOGS_DIR"

    # Execute the command and capture the output
    BASH_OUTPUT=$(bash -c "$@")
    echo -e -n $BASH_OUTPUT > $OUTPUT_FILE
    BASH_RVALUE=$?
    printf "%i\n" "$BASH_RVALUE"
}

# == TESTS ==

#run_test "echo it works"
run_test 1 "cat $INFILE_DIR/infile1 $INFILE_DIR/infile2 $INFILE_DIR/infile3"






# == COLOURS ==

BOLD="\033[1m"
RESET="\033[0m"
BLACK="\033[30m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
MAGENTA="\033[35m"
CYAN="\033[36m"
WHITE="\033[37m"