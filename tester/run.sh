#!/bin/bash

source test.sh
source utils.sh
if [ "$1" == --valgrind ]; then VALGRIND=1; else VALGRIND=0; fi

# == VARIABLES ==

MINISHELL_DIR=""

TEST_NUM=0
ERROR_FLAG=0

ARG_ARRAY=""

TESTER_DIR=$(dirname "$0")
MINISHELL_DIR="$TESTER_DIR/../"

# Define absolute paths
INFILE_DIR="$TESTER_DIR/infiles"
LOGS_DIR="$TESTER_DIR/logs"
EXPECTED_OUTPUTS_DIR="$TESTER_DIR/expected_outputs"

# Define expected exit values for each test (adjust as needed)
declare -A EXPECTED_EXIT_VALUES=(
	[1]=0
	[2]=0
	[3]=0
	# Add more tests as needed
)

# Clear the screen
clear

tester_setup

mkdir -p "$LOGS_DIR"

# == TESTS ==

test "cat infile1 infile2 infile3"
test "cat <infile1 <infile2 <infile3"
test "cat <infile1 <infile2 <infile3 infile1 infile2 infile3"
