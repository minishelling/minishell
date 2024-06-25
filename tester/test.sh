#!/bin/bash

# == VARIABLES ==

MINISHELL_DIR=""

TEST_NUM=0
ERROR_FLAG=0


OUTPUTFILE_BASH=""
OUTPUTFILE_MINISHELL=""

TESTER_DIR=$(dirname "$0")
MINISHELL_DIR="$TESTER_DIR/../"

# Define relative paths
REL_INFILE_DIR="infiles"
REL_LOGS_DIR="logs"

# Define absolute paths
INFILE_DIR="$TESTER_DIR/$REL_INFILE_DIR"
LOGS_DIR="$TESTER_DIR/$REL_LOGS_DIR"

# Define the logs directory
TESTER_DIR="$(dirname "$0")"
INFILE_DIR="$TESTER_DIR/infiles"
LOGS_DIR="$TESTER_DIR/logs"

# == FUNCTIONS ==

test()
{
	TEST_NUM=$(( TEST_NUM + 1 ))

    local ERROR_LOG="$LOGS_DIR/error_${TEST_NUM}.txt"

	# Define temporary files for outputs
	local BASH_OUTPUT_FILE="$LOGS_DIR/bash_output_${TEST_NUM}.txt"
    local MINISHELL_OUTPUT_FILE="$LOGS_DIR/mini_output_${TEST_NUM}.txt"

    # Execute BASH command and capture the output
    BASH_OUTPUT=$(bash -c "$@")
	BASH_EXIT_CODE=$?
	echo "$BASH_OUTPUT" > $BASH_OUTPUT_FILE
	BASH_ERROR=$(bash -c "$@" 2>&1 >/dev/null)

	# Execute MINISHELL command and capture the output

	MINISHELL_OUTPUT=$(echo "$@" | $MINISHELL_DIR/minishell)
	MINISHELL_EXIT_CODE=$?
	echo "$MINISHELL_OUTPUT" > $MINISHELL_OUTPUT_FILE
	#MINISHELL_ERROR=$($MINISHELL_DIR/minishell "$@" 2>&1 >/dev/null)

	diff $BASH_OUTPUT_FILE $MINISHELL_OUTPUT_FILE
}

# == TESTS ==

# Example test
run_test "cat $RELATIVE_INFILE_DIR/infile1 $RELATIVE_INFILE_DIR/infile2 $RELATIVE_INFILE_DIR/infile3"

