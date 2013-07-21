#!/bin/bash

OUTFILE="namelist"
PREFIX="ccnx:/gen"
START_INDEX=0
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
	START_INDEX=$5
fi
if [[ -n "$6" ]]; then
	SUFFIX=$6
fi

# echo $PREFIX > $OUTFILE
:>$OUTFILE

if [[ $NUM_NODE -eq 0 ]]
then
	for (( i=$START_INDEX; i<($START_INDEX + $ENTRY_PER_NODE); i++ ))
	do
		echo "$PREFIX/$i/$SUFFIX" >> $OUTFILE
	done
	
else
	for (( i=0; i<$NUM_NODE; i++ ))
	do
		for (( j=$START_INDEX; j<($START_INDEX + $ENTRY_PER_NODE); j++ ))
		do
			echo "$PREFIX/$i/$j/$SUFFIX" >> $OUTFILE
		done
	done
fi

