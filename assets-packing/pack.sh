#!/bin/bash
#https://github.com/urraka/texpack

echo Packing...

./texpack --output sheets/space --padding 1 --size 2048x2048 --max-size --allow-rotate --mode auto --pretty input_space.txt

echo Complete! Packed to ./sheets
