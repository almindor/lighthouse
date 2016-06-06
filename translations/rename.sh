#!/bin/bash

for file in lighthousets_*.ts
do
    nf=`echo $file | sed 's/lighthousets_/harbour-lighthouse-/g'`
    mv "$file" "$nf"
done

