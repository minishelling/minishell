#!/bin/bash

# == FUNCTIONS ==

test()
{
	TEST_NUM=$(( TEST_NUM + 1 ))
	OUTPUT_FILE="$LOGS_DIR/test_${TEST_NUM}.txt"
	EXPECTED_FILE="$EXPECTED_OUTPUTS_DIR/test_${TEST_NUM}.txt"
	EXPECTED_EXIT_VALUE="${EXPECTED_EXIT_VALUES[$TEST_NUM]}"  # Get expected exit value
	DIFF_FILE="$LOGS_DIR/diff_${TEST_NUM}.txt"
	ARG_ARRAY="$@"

	ERROR_FLAG=0

	print_outfile_header

    # Execute BASH command and capture the output
    BASH_OUTPUT=$(bash -c "$ARG_ARRAY" 2>&1)
	BASH_EXIT_STATUS=$?
	echo "$BASH_OUTPUT" >> $OUTPUT_FILE

	print_outfile_footer

	if [ -f "$EXPECTED_FILE" ]; then
		diff "$OUTPUT_FILE" "$EXPECTED_FILE" > "$DIFF_FILE"
		COMPARE_EXIT_STATUS=$?
	else
		COMPARE_EXIT_STATUS=1
		echo "No expected outfile found: $EXPECTED_FILE" >> "$OUTPUT_FILE"
	fi

	# Determine status for OUT column
	if [ -f "$EXPECTED_FILE" ]; then
		if [ $COMPARE_EXIT_STATUS -eq 0 ]; then
			printf "%s${GREEN}%-8s${RESET}" "[OK]"
		else
			printf "${RED}%-8s${RESET}" "[KO]"
		fi
	else
		printf "${RED}%-8s${RESET}" "[KO]"
	fi

	# Determine status for EXIT column
	if [ $BASH_EXIT_STATUS -eq $EXPECTED_EXIT_VALUE ]; then
		printf "${GREEN}%-8s${RESET}\n" "[OK]"
	else
		printf "${RED}%-8s${RESET}\n" "[KO]"
	fi
}
