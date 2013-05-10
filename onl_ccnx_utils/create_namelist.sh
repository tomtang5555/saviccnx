#!/bin/bash

OUTFILE="namelist"
PREFIX="ccnx:/gen"
ENTRY_PER_NODE=100
NUM_NODE=0
SUFFIX=""

if [[ -n "$1" ]]; then
	OUTFILE="$1"
fi
if [[ -n "$2" ]]; then
	PREFIX=$2
fi
if [[ -n "$3" ]]; then
	ENTRY_PER_NODE=$3
fi
if [[ -n "$4" ]]; then
	NUM_NODE=$4
fi
if [[ -n "$5" ]]; then
	SUFFIX=$5
fi

# echo $PREFIX > $OUTFILE
:>$OUTFILE

if [[ $NUM_NODE -eq 0 ]]
	then
	for (( i=0; i<$ENTRY_PER_NODE; i++ ))
	do
		echo "$PREFIX/$i/$SUFFIX" >> $OUTFILE
	done
	
	else
	for (( i=0; i<$NUM_NODE; i++ ))
	do
		for (( j=0; j<$ENTRY_PER_NODE; j++ ))
		do
			echo "$PREFIX/$i/$j/$SUFFIX" >> $OUTFILE
		done
	done
fi

