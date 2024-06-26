#!/bin/bash

source test.sh
source utils.sh
if [ "$1" == --valgrind ]; then VALGRIND=1; else VALGRIND=0; fi

# == VARIABLES ==

MINISHELL_DIR=""

TEST_NUM=0
ERROR_FLAG=0

ARG_ARRAY=""

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


