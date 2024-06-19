#!/bin/bash

# == VARIABLES ==

TEST_NUM=0

BASH_OUTPUT=""
BASH_ERROR=""
BASH_EXIT_CODE=0

MINISHELL_OUTPUT=""
MINISHELL_ERROR=""
MINISHELL_EXIT_CODE=0

# Get the script directory
TESTER_DIR="$(dirname "$0")"
MINISHELL_DIR="$TESTER_DIR/.."

# Define the relative path for input files
RELATIVE_INFILE_DIR="infiles"

# Define the absolute path for input files
INFILE_DIR="$TESTER_DIR/$RELATIVE_INFILE_DIR"

# Define the logs directory
TESTER_DIR="$(dirname "$0")"
INFILE_DIR="$TESTER_DIR/infiles"
LOGS_DIR="$TESTER_DIR/logs"

# Define the expected outputs directory
EXPECTED_OUTPUTS_DIR="$TESTER_DIR/expected_outputs"


# == FUNCTIONS ==

run_test()
{
	TEST_NUM=$(( TEST_NUM + 1 ))
	# Create a logs directory in the script's location
	mkdir -p "$LOGS_DIR"

    local ERROR_LOG="$LOGS_DIR/output_${TEST_NUM}.txt"

	# Define temporary files for outputs
	local BASH_OUTPUT_FILE=$(mktemp)
    local MINISHELL_OUTPUT_FILE=$(mktemp)

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
