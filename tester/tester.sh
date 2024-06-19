#!/bin/bash

# == VARIABLES ==

BASH_OUTPUT=""
BASH_ERROR=""
BASH_RVALUE=0

# Get the script directory
TESTER_DIR="$(dirname "$0")"
# Define the relative path for input files
RELATIVE_INFILE_DIR="infiles"
# Define the absolute path for input files
INFILE_DIR="$TESTER_DIR/$RELATIVE_INFILE_DIR"
# Define the logs directory
MINISHELL_OUTPUT=""
MINISHELL_ERROR=""
MINISHELL_RVALUE=0

TESTER_DIR="$(dirname "$0")"
INFILE_DIR="$TESTER_DIR/infiles"
LOGS_DIR="$TESTER_DIR/logs"
# Define the expected outputs directory
EXPECTED_OUTPUTS_DIR="$TESTER_DIR/expected_outputs"

# == FUNCTIONS ==

run_test()
{
    local TEST_NUM=$1
    local OUTPUT_FILE="$LOGS_DIR/output_${TEST_NUM}.txt"
    local ERROR_FILE="$LOGS_DIR/error_${TEST_NUM}.txt"
    shift
	local TEST_NUM=$1
	local OUTPUT_FILE="$LOGS_DIR/test_${TEST_NUM}.txt"
	shift

	# Create a logs directory in the script's location
	mkdir -p "$LOGS_DIR"

    # Execute the command and capture the output
    BASH_OUTPUT=$(bash -c "$@" 2>$ERROR_FILE)
    echo -e -n $BASH_OUTPUT > $OUTPUT_FILE
    BASH_RVALUE=$?
    printf "%i\n" "$BASH_RVALUE"
	# Add header to the output file
	echo "========== Test ${TEST_NUM} ==========" > "$OUTPUT_FILE"
	echo "Command: $@" >> "$OUTPUT_FILE"
	echo "======================================" >> "$OUTPUT_FILE"
	echo -n "Output: " >> "$OUTPUT_FILE"

	# Execute the command and capture the output and errors
	BASH_OUTPUT=$(bash -c "$@" >> "$OUTPUT_FILE" 2>&1)
	BASH_RVALUE=$?

	# Add footer to the output file
	echo "" >> "$OUTPUT_FILE"
	echo "======================================" >> "$OUTPUT_FILE"
	echo "Return Value: $BASH_RVALUE" >> "$OUTPUT_FILE"
	
	echo "========== End of Test ${TEST_NUM} ==========" >> "$OUTPUT_FILE"

	# Print return value
	printf "%i\n" "$BASH_RVALUE"
}

compare_output()
{
	local TEST_NUM=$1
	local GENERATED_FILE="$LOGS_DIR/test_${TEST_NUM}.txt"
	local EXPECTED_FILE="$EXPECTED_OUTPUTS_DIR/test_${TEST_NUM}.txt"
	local DIFF_FILE="$LOGS_DIR/diff_${TEST_NUM}.txt"

	# Compare the generated file with the expected file and log differences
	diff "$GENERATED_FILE" "$EXPECTED_FILE" > "$DIFF_FILE"
	local DIFF_RVALUE=$?

	if [ $DIFF_RVALUE -eq 0 ]; then
		echo "Test ${TEST_NUM}: PASS"
		rm "$DIFF_FILE"
	else
		echo "Test ${TEST_NUM}: FAIL. Differences logged in $DIFF_FILE"
	fi

	return $DIFF_RVALUE
}

# == TESTS ==

# Example test
run_test 1 "cat $RELATIVE_INFILE_DIR/infile1 $RELATIVE_INFILE_DIR/infile2 $RELATIVE_INFILE_DIR/infile3"
compare_output 1

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
