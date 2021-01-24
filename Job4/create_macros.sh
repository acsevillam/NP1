#!/bin/sh
#
# job1.sh run NP1 with test macro on lxplus 
# Copyright Andrés Camilo Sevilla
#
# !!!! Must be changed for each user !!! #

# Loading environmental variables 
#source /afs/cern.ch/work/a/asevilla/workspace/G4WORK/SciFW1/setup_lxplus.sh

# Defining variables
TEMPLATE_DIR=/Users/asevilla/workspace/JOBS/NP1
JOB_DIR=/Users/asevilla/workspace/JOBS/NP1/Job4

concentrationArray=(0 5 10 15 20 25 30 35 40 45 50)

for i in ${!concentrationArray[*]}
do

	sed "
	
		s/%%FileName%%/${concentrationArray[$i]}mg_g/g;
		
		s/%%ParticleCoatingMaterial%%/G4_Gd/g;
		
		s/%%ParticleCoatingThickness%%/0/g;
		
		s/%%ParticleCoatingThicknessUnit%%/nm/g;
		
		s/%%ParticleMaterial%%/G4_Gd/g;
		
		s/%%ParticleRadius%%/10/g;
		
		s/%%ParticleRadiusUnit%%/nm/g;
		
		s/%%Concentration%%/${concentrationArray[$i]}/g;
		
		s/%%ConcentrationUnit%%/mg\/g/g;
		
		s/%%VoxelsXNo%%/10/g;
		
		s/%%VoxelsYNo%%/10/g;
		
		s/%%VoxelsZNo%%/10/g;
		
		s/%%SquareShapeBeam%%//g;
		
		s/%%CircularShapeBeam%%/#/g;
		
		s/%%SphericalVolumeSource%%/#/g;
		
		s/%%SphericalVolumeSource%%/#/g;
		
		s/%%BeamRadius%%/10/g;
		
		s/%%BeamRadiusUnit%%/nm/g;
		
		s/%%BeamMacroFile%%/beam\/RX40kVp.mac/g;
		
		" $TEMPLATE_DIR/template_1.mac > $JOB_DIR/mac/${concentrationArray[$i]}mg_g.mac
		
done