#!/bin/bash

source test.sh
source utils.sh
if [ "$1" == --valgrind ]; then VALGRIND=1; else VALGRIND=0; fi


