#!/bin/bash

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

# == VARIABLES ==

DELIMITER="------------------------------------------------------------------------------------------------------------------------"

# == FUNCTIONS ==

tester_setup()
{
	printf "\n\n${BOLD}${MAGENTA}%-90s%-8s%-8s%-8s%-8s\n${RESET}" "TESTNAME" "OUT" "EXIT" "TIME" "LEAKS"
}

# -- PRINTING FUNCTIONS --

print_tester_header() {
	printf "${YELLOW}\n%s\n${RESET}" "$1"
}

print_arg_array() {
	local size=${#ARG_ARRAY[@]}

	for ((i=0; i<$size; i++)); do
		printf "\"%s\" " "${ARG_ARRAY[i]}"
	done
}

print_outfile_header()
{
	echo "========== Test $TEST_NUM ==========" > "$OUTPUT_FILE"
	echo "Command: $@" >> "$OUTPUT_FILE"
	echo "======================================" >> "$OUTPUT_FILE"
	echo -n "Output: " >> "$OUTPUT_FILE"
}

print_outfile_footer()
{
	echo "======================================" >> "$OUTPUT_FILE"
	echo "Return Value: $BASH_RVALUE" >> "$OUTPUT_FILE"
	echo "========== End of Test ${TEST_NUM} ==========" >> "$OUTPUT_FILE"
}
