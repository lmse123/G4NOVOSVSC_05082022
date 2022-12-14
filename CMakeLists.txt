#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.8...3.18)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(novosvsc_project)

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
#${PROJECT_SOURCE_DIR}="${PROJECT_SOURCE_DIR}"
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/macros)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(novosvsc novosvsc.cc ${sources} ${headers})
target_link_libraries(novosvsc ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build the program. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(NOVO_SCRIPTS
  beamIso.in
  beamPPencil.in
  beamPencil.in
  beamPencil_1cm.in
  beamPencil_3cm.in
  beamPencil_5cm.in
  beamPencil_7cm.in
  beamPencil_9cm.in
  beam1.in
  beamN.in
  beam_hist.in
  novosvsc.in
  init_vis.mac
  vis.mac
  )

# set(NOVO_SCRIPTS
#   macros/*.in
#   init_vis.mac
#   vis.mac
#   )
# set(NOVO_SCRIPTS
#   macros/beamIso.in
#   macros/beamPencil.in
#   macros/beam1.in
#   macros/beamN.in
#   macros/beam_hist.in
#   macros/novosvsc.in
#   init_vis.mac
#   vis.mac
#   )

foreach(_script ${NOVO_SCRIPTS})
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
add_custom_target(novosvsc_project DEPENDS novosvsc)
# (7)
#install(TARGETS runverification DESTINATION bin)
