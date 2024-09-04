#!/usr/bin/env bash

NAME=${1%/}

# print the head once
file=($NAME/*.csv)
head -n1 $file > $NAME.csv

# print the content
for file in $NAME/*.csv; do
    tail -n+2 $file >> $NAME.csv
done;
