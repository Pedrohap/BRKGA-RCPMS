#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one

#Debug for specific instance

#Heavy instance debug
#./samplecode < ./instances/RCPMSII/RCPMS_Instance_136_m=8_n=200_t=9 > ./debug/RCPMS_Instance_136_m=8_n=200_t=9.txt

#Small instance debug
#./samplecode < ./debug/debug_intances/SMALL_Test > ./debug/SMALL_Test.txt

#Instance 8 Test
#./samplecode < ./instances/RCPMSI/RCPMS_Instance_8_m=2_n=15_t=3 > ./debug/SMALL_Test.txt

#for i in {1..10}
#do
#    ./samplecode < ./instances/RCPMSII/RCPMS_Instance_136_m=8_n=200_t=9 > ./solucoes/RCPMSII/RCPMS_Instance_136_m=8_n=200_t=9_$i.txt
#done

for i in {1..10}
do
    ./samplecode < ./instances/RCPMSI/RCPMS_Instance_3_m=2_n=8_t=3 > ./debug/SMALL_Test_$i.txt
done

exit 0