#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(NP1)

# ROOT
include(FindROOT.cmake)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
)
include_directories( ${INCLUDE_DIRECTORIES})
set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
)
link_directories( ${LINK_DIRECTORIES})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Get examples sources from common and shared
#
#list(APPEND sources 
#  ${PROJECT_SOURCE_DIR}/../SciFW1/src/geometry/AcceleratorClinac2100.cc)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(NP1 NP1.cc ${sources} ${headers})
target_link_libraries(NP1 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build NP1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(NP1_SCRIPTS
  mac/vis1.mac
  mac/vis2.mac
  mac/init.mac
  mac/single_nanoparticle.mac
  mac/multiple_nanoparticles.mac
  beam/40keV.mac
  beam/RX40kVp.mac
  beam/RX100kVp.mac
  beam/RX180kVp.mac
  beam/x-ray-spectrum-geant4-40kV.dat
  beam/x-ray-spectrum-geant4-100kV.dat
  beam/x-ray-spectrum-geant4-180kV.dat
  beam/Co-60.mac
  beam/I-131_1.mac
  beam/I-131_2.mac
  beam/I-131_gamma_1.mac
  beam/I-131_gamma_2.mac
  beam/GammaAccelerator6MV.mac
  beam/square_plane.mac
  beam/circular_plane.mac
  beam/spherical_surface.mac
  beam/spherical_volume.mac
  data/README
  log/README
  plotHisto.C
  plotAllHisto.C
  plotNTuple.C
  job_launcher.sh
  )

foreach(_script ${NP1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(NP1 DEPENDS NP1)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS NP1 DESTINATION bin)


