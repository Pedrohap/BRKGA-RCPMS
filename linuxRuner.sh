#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one

#for i in {0..10}
#do
#  ./samplecode < ./instances/RCPMSI/RCPMS_Instance_1_m=2_n=8_t=3 > ./solucoes/exit$i.txt
#done

for i in ./instances/RCPMSI/*
do
    if test -f "$i" 
    then
        mkdir -p solucoes/$i
        for j in {1..10}
        do
          ./samplecode < $i >> ./solucoes/$i/result.csv
        done
    fi
done

exit 0