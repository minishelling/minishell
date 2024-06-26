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

tester_setup() {
	printf "\n\n${BOLD}${CYAN}%-90s%-8s%-8s%-8s%-8s\n${RESET}" "TESTNAME" "OUT" "EXIT" "TIME" "LEAKS"
}

# -- PRINTING FUNCTIONS --

print_header() {
	printf "${YELLOW}\n%s\n${RESET}" "$1"
}

print_arg_array() {
	local size=${#ARG_ARRAY[@]}

	for ((i=0; i<$size; i++)); do
		printf "\"%s\" " "${ARG_ARRAY[i]}"
	done
}
