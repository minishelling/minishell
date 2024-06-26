#!/bin/bash

# == FUNCTIONS ==

test()
{
	TEST_NUM=$(( TEST_NUM + 1 ))
	ARG_ARRAY="$@"

	ERROR_FLAG=0
    local ERROR_LOG="$LOGS_DIR/error_${TEST_NUM}.txt"

	# Define temporary files for outputs
	local BASH_OUTPUT_FILE="$LOGS_DIR/bash_output_${TEST_NUM}.txt"
    local MINISHELL_OUTPUT_FILE="$LOGS_DIR/mini_output_${TEST_NUM}.txt"

	printf "#%2i: %-85.83s" "${TEST_NUM}" "$(print_arg_array)"

    # Execute BASH command and capture the output
    BASH_OUTPUT=$(bash -c "$ARG_ARRAY")
	BASH_EXIT_CODE=$?
	if 
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

