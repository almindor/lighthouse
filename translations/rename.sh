#!/bin/bash

for file in *_*.ts
do
    nf=`echo $file | sed 's/for_use_lighthouse_lighthousets_//g'`
    mv "$file" "lighthouse-$nf"
done

