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
JOB_DIR=/Users/asevilla/workspace/JOBS/NP1/Job1

sizesArray=(5)

for i in ${!sizesArray[*]}
do

	sed "
	
		s/%%FileName%%/${sizesArray[$i]}nm/g;
		
		s/%%ParticleCoatingMaterial%%/G4_Gd/g;
		
		s/%%ParticleCoatingThickness%%/0/g;
		
		s/%%ParticleCoatingThicknessUnit%%/nm/g;
		
		s/%%ParticleMaterial%%/G4_Gd/g;
		
		s/%%ParticleRadius%%/${sizesArray[$i]}/g;
		
		s/%%ParticleRadiusUnit%%/nm/g;
		
		s/%%Concentration%%/500/g;
		
		s/%%ConcentrationUnit%%/mg\/g/g;
		
		s/%%VoxelsXNo%%/1/g;
		
		s/%%VoxelsYNo%%/1/g;
		
		s/%%VoxelsZNo%%/1/g;
		
		s/%%WaterVoxelSideLength%%/20/g;
		
		s/%%WaterVoxelSideLengthUnit%%/um/g;
		
		s/%%SquareShapeBeam%%/#/g;
		
		s/%%CircularShapeBeam%%/#/g;
		
		s/%%CircularShapeBeam%%/#/g;
		
		s/%%SphericalVolumeSource%%/#/g;
		
		s/%%SphericalVolumeSource%%/#/g;
		
		s/%%BeamRadius%%/${sizesArray[$i]}/g;
		
		s/%%BeamRadiusUnit%%/nm/g;
		
		s/%%BeamMacroFile%%/beam\/I-131_2.mac/g;
		
		s/%%Histo1D0X%%/100 0 10 um/g;
		
		s/%%Histo1D1X%%/100 0 100/g;
		
		s/%%Histo1D2X%%/100 0 10 um/g;
		
		s/%%Histo1D3X%%/100 0 100/g;
		
		s/%%Histo1D4X%%/1000 0 40 keV/g;
		
		s/%%Histo1D5X%%/1000 0 10E-3 gray/g;
		
		s/%%Histo1D6X%%/1000 0 40 keV/g;
		
		s/%%Histo1D7X%%/1000 0 10E-3 gray/g;
		
		s/%%Histo1D8X%%/20 0 20/g;
		
		s/%%Histo1D9X%%/1000 0 40 keV/g;
		
		s/%%Histo1D10X%%/1000 0 40 keV/g;
		
		s/%%Histo1D11X%%/1000 0 100 um/g;
			
		s/%%Histo1D12X%%/1000 0 100 um/g;
		
		" $TEMPLATE_DIR/template_1.mac > $JOB_DIR/mac/${sizesArray[$i]}nm.mac
		
done