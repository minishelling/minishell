#!/bin/bash

# == VARIABLES ==
BASH_OUTPUT=""
BASH_RVALUE=0

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

# Get the script directory
TESTER_DIR="$(dirname "$0")"
# Define the absolute path for input files
INFILE_DIR="$TESTER_DIR/$RELATIVE_INFILE_DIR"
# Define the logs directory
LOGS_DIR="$TESTER_DIR/logs"
# Define the expected outputs directory
EXPECTED_OUTPUTS_DIR="$TESTER_DIR/expected_outputs"

# Define expected exit values for each test (adjust as needed)
declare -A EXPECTED_EXIT_VALUES=(
	[1]=0
	[2]=0
	[3]=0
	[4]=0
	# Add more tests as needed
)

# Clear the screen
clear

# Print header to console
printf "${BOLD}${MAGENTA}%-90s%-8s%-8s%-8s%-8s${RESET}\n" "TEST NAME" "OUT" "EXIT" "TIME" "LEAKS"

# Create a logs directory in the script's location
mkdir -p "$LOGS_DIR"

run_test() {
	local TEST_NUM=$1
	local OUTPUT_FILE="$LOGS_DIR/test_${TEST_NUM}.txt"
	local EXPECTED_FILE="$EXPECTED_OUTPUTS_DIR/test_${TEST_NUM}.txt"
	local EXPECTED_EXIT_VALUE="${EXPECTED_EXIT_VALUES[$TEST_NUM]}"  # Get expected exit value
	local DIFF_FILE="$LOGS_DIR/diff_${TEST_NUM}.txt"
	shift

	# Write header to output file
	echo "========== Test $TEST_NUM ==========" > "$OUTPUT_FILE"
	echo "Command: $@" >> "$OUTPUT_FILE"
	echo "======================================" >> "$OUTPUT_FILE"
	echo -n "Output: " >> "$OUTPUT_FILE"

	# Execute the command and capture the output and errors, append to output file
	BASH_OUTPUT=$(bash -c "$@" 2>&1)
	BASH_RVALUE=$?
	echo "$BASH_OUTPUT" >> "$OUTPUT_FILE"

	# Add footer to the output file
	echo "======================================" >> "$OUTPUT_FILE"
	echo "Return Value: $BASH_RVALUE" >> "$OUTPUT_FILE"
	echo "========== End of Test ${TEST_NUM} ==========" >> "$OUTPUT_FILE"

	# Compare generated vs expected output
	if [ -f "$EXPECTED_FILE" ]; then
		diff "$OUTPUT_FILE" "$EXPECTED_FILE" > "$DIFF_FILE"
		COMPARE_RVALUE=$?
	else
		COMPARE_RVALUE=1  # Set to 1 if expected file does not exist
		echo "No expected output file found: $EXPECTED_FILE" >> "$OUTPUT_FILE"
	fi

	# Print test command
	printf "${YELLOW}%-90s${RESET}" "$@"

	# Determine status for OUT column
		if [ -f "$EXPECTED_FILE" ]; then
			if [ $COMPARE_RVALUE -eq 0 ]; then
				printf "${GREEN}%-8s${RESET}" "[OK]"
			else
				printf "${RED}%-8s${RESET}" "[FAIL]"
			fi
		else
			printf "${RED}%-8s${RESET}" "[FAIL]"
		fi


	# Determine status for EXIT column
		if [ $BASH_RVALUE -eq $EXPECTED_EXIT_VALUE ]; then
			printf "${GREEN}%-8s${RESET}\n" "[OK]"
		else
			printf "${RED}%-8s${RESET}\n" "[KO]"
		fi
}

# == TESTS ==
#Input redirection
echo
run_test 1 "cat infile1 infile2 infile3"
echo
run_test 2 "cat <infile1 <infile2 <infile3"
echo
run_test 3 "cat <infile1 <infile2 <infile3 infile1 infile2 infile3"
echo

#Output redirection
# run_test 4 "<infile1 cat >outfile1 >outfile2 | cat"


# run_test  "echo $(ls)"
