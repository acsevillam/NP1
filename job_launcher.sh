#!/bin/sh
#

materialsArray=(Ag Au Bi2S3 Pb Pt Ta2O5)

for i in ${!materialsArray[*]}
do
echo " ${materialsArray[$i]} beam is running"
time ./NP1 mac/run_${materialsArray[$i]}.mac > log/${materialsArray[$i]}.log &
sleep 10
done
