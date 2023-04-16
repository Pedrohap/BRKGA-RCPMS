#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one

#Debug for specific instance

#Heavy instance debug
#./samplecode < ./instances/RCPMSII/RCPMS_Instance_136_m=8_n=200_t=9 > ./debug/RCPMS_Instance_136_m=8_n=200_t=9.txt

#Small instance debug
./samplecode < ./debug/debug_intances/SMALL_Test > ./debug/SMALL_Test.txt


#for i in {1..10}
#do
#    ./samplecode < ./instances/RCPMSII/RCPMS_Instance_136_m=8_n=200_t=9 > ./solucoes/RCPMSII/RCPMS_Instance_136_m=8_n=200_t=9_$i.txt
#done

exit 0