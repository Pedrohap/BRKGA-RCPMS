#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one

#Print for data organization
#for i in ./instances/RCPMSI/*
#do
#    if test -f "$i" 
#    then
#      for j in {1..10}
#      do
#        echo $i"_"$j
#      done
#    fi
#done

for i in ./instances/RCPMSI/*
do
    if test -f "$i" 
    then
        mkdir -p solucoes/RCPMSI/
        for j in {1..10}
        do
          echo -n $i"_"$j"," >> ./solucoes/RCPMSI/result.csv
          ./samplecode < $i >> ./solucoes/RCPMSI/result.csv
        done
    fi
done

for i in ./instances/RCPMSII/*
do
    if test -f "$i" 
    then
        mkdir -p solucoes/RCPMSII/
        for j in {1..10}
        do
          echo -n $i"_"$j"," >> ./solucoes/RCPMSII/result.csv
          ./samplecode < $i >> ./solucoes/RCPMSII/result.csv
        done
    fi
done


exit 0