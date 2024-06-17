#!/bin/bash

# == VARIABLES ==
BASH_OUTPUT=""
MINISHELL_OUTPUT=""
BASH_ERROR=""
MINISHELL_ERROR=""
BASH_RVALUE=0
MINISHELL_RVALUE=0

# == FUNCTIONS ==

run_test()
{
    # Execute the command and capture the output
    BASH_OUTPUT=$(bash -c "$@")
    #bash -c "$@" > BASH_OUTPUT
    cat $BASH_OUTPUT
    BASH_RVALUE=$?
    printf "%i\n", "$BASH_RETURN";
}

# == TESTS ==

run_test "echo it works"




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