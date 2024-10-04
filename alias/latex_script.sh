#!/bin/sh

if [ $# -eq 0 ];
then
  echo "$0: Missing arguments"
  exit 1
elif [ $# -gt 2 ];
then
  echo "$0: Too many arguments"
  exit 1
else
  pandoc -s "Assignment $1 Mercury Mcindoe 85594505.md" -o Asst$1_85594505.pdf --pdf-engine=pdflatex
fi
