#!/bin/sh
#
# job1.sh run NP1 with test macro on lxplus 
# Copyright Andrés Camilo Sevilla
#
# !!!! Must be changed for each user !!! #

# Loading environmental variables 
#source /afs/cern.ch/work/a/asevilla/workspace/G4WORK/SciFW1/setup_lxplus.sh

# Defining variables
NP1_DIR=/home/asevilla/workspace/G4WORK/NP1-build
JOB_DIR=/home/asevilla/workspace/JOBS/NP1/Job1
DataNo=1
i=0

# Creating DataNo directory

while [ $i -lt 1 ]
do
	if [ -d "./${DataNo}" ]
	then
		echo " “./${DataNo}” has been created"
		DataNo=$(($DataNo+1))
		i=0
	else
		mkdir ./${DataNo}
		i=1
fi
done

# Creating log directory

if [ -d "./${DataNo}/log" ]
then
	echo " “./${DataNo}/log” has been created"
else
	mkdir ./${DataNo}/log
fi

# Creating data directory

if [ -d "./${DataNo}/data" ]
then
	echo " “./${DataNo}/data” has been created"
else
	mkdir ./${DataNo}/data
fi

# Running NP1

cp -R $NP1_DIR/beam $JOB_DIR/${DataNo}/beam
cp -R $NP1_DIR/mac $JOB_DIR/${DataNo}/mac
cp -R $JOB_DIR/mac/* $JOB_DIR/${DataNo}/mac/.

cd $JOB_DIR/${DataNo}

sizesArray=(10 20 30 40 50 60 70 80 90 100)

for i in ${!sizesArray[*]}
do
echo " ${sizesArray[$i]}nm is running"
time $NP1_DIR/NP1 -m $JOB_DIR/mac/${sizesArray[$i]}nm.mac -v off -n 1000000 > $JOB_DIR/${DataNo}/log/${sizesArray[$i]}nm.log &
sleep 10
done

cd $JOB_DIR/