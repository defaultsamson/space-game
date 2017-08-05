#!/bin/bash
#https://github.com/urraka/texpack

echo Packing...

./texpack --output sheets/ambient --padding 1 --size 2048x2048 --max-size --allow-rotate --mode auto --pretty --format xml input_ambient.txt

./texpack --output sheets/entity --padding 1 --size 2048x2048 --max-size --allow-rotate --mode auto --pretty --format xml input_entity.txt

echo Complete! Packed to ./sheets
