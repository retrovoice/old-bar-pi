# Top-level build file for Barpi

# This insures that the cmake on your build system is new enough to
# work properly.

cmake_minimum_required( VERSION 2.8 )

# We define the location for where "make install" (Linux) or the
# "INSTALL project" (Microsoft Visual Studio) will put the build
# products of this project.  If you're performing development work,
# and don't want to install files in system wide directories, or if
# you lack system admin privileges, then a local directory in your
# user space can be specified.  This is the default setting for this
# project.
#
# For the installed files to execute properly, the appropriate
# environment variables need to be set.  Assuming a base directory,
# "INSTALLDIR", the typical environment variables needed for Windows
# and Linux are:
#
# Windows
#   PATH - Add %INSTALLDIR%\bin
#
# Linux
#   PATH - Add $HOME/local/bin
#   LD_LIBRARY_PATH - Add $HOME/local/lib
#
# Additional environment variables that might be needed by your
# compiler are:
#
# Windows
#   LIBPATH - Add %INSTALLDIR%\lib
#   INCLUDE - Add %INSTALLDIR%\include
#
# Linux 
#   LIBRARY_PATH - Add $HOME/local/lib
#   C_INCLUDE_PATH, CPLUS_INCLUDE_PATH - Add $HOME/local/include

# The environment variable that defines your "home" directory is used
# to define the INSTALLDIR.

if( WIN32 )
  set ( HOMED $ENV{HOMEDRIVE} )
  set ( HOMEP $ENV{HOMEPATH} )
  set ( WORKSP $ENV{WORKSPACE} )
  if ( WORKSP )
    get_filename_component( INSTALLDIR ${WORKSP}/local ABSOLUTE )
  elseif ( HOMED AND HOMEP )
    get_filename_component( INSTALLDIR ${HOMED}${HOMEP}/local ABSOLUTE )
  else ( WORKSP )
    get_filename_component( INSTALLDIR "C:\\" ABSOLUTE )
  endif ( WORKSP )
else( WIN32 )
  set( INSTALLDIR $ENV{HOME}/local )
endif( WIN32 )
message (STATUS "Installation directory set to - ${INSTALLDIR}")

# Set the base directory for where "make install" will place files.
# Look for the CMake command "install" in the CMakeLists.txt files of
# each build directory to determine the specific subdirectories,
# e.g. bin, lib, include, that are specified for this project. 

set( CMAKE_INSTALL_PREFIX ${INSTALLDIR} )
message ( STATUS "CSF: CMAKE_INSTALL_PREFIX is ${CMAKE_INSTALL_PREFIX}" )

# Include INSTALLDIR in the path where Cmake will look for packages
# that this project depends on.  If you have built and installed other
# software that is needed by this project, check to make sure those
# other packages are either installed at the system level (where CMake
# should be able to find them), or in your local path.  You can add
# additional paths to CMAKE_PREFIX_PATH by appending them as space
# separated items, such as:
#
# set( CMAKE_PREFIX_PATH ${INSTALLDIR} /add/path2 /add/path3 )

set( CMAKE_PREFIX_PATH ${INSTALLDIR} )

# You may have custom CMake modules installed on your system.  Use
# this command to tell CMake where to look for additional modules.

get_filename_component( CMAKE_MODULE_PATH
     ${CMAKE_SOURCE_DIR}/cmake_modules ABSOLUTE )

set( MAJOR_VERSION 0 )
set( MINOR_VERSION 1 )
set( PATCH_VERSION 0 )
set( VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION} )

project( barpi-${VERSION} )
message("Processing cmake files for ${CMAKE_PROJECT_NAME} project.")

# Check to see if this is a 64 bit Windows build If true, define the
# string that should be appended to targets, and looked for in link
# libs.
if( ${CMAKE_CL_64} )
   set( X64_SUFFIX _x64 )
endif( ${CMAKE_CL_64} )

# Add 'd' postfix to library names for debug mode.
# This automatically(?) adjusts the names of link libs
# in Visual Studio ?!?
set(CMAKE_DEBUG_POSTFIX _d)

# Enable automated testing.  This command must be
# in root CMakeLists.txt file.
enable_testing()

find_package(Qt4 4 REQUIRED QtCore QtGui QtSql)
include(${QT_USE_FILE})

set ( OUTPUT_NAME barpi )

file ( GLOB SOURCES *.cpp )

set (Q_OBJECT_HEADERS
  mainwindow.h
  catalog.h
  databasedialog.h
  inventory.h
  inventorymanager.h
  invoicemanager.h
  bpscan.h
  scannerdialog.h)
#set (Q_OBJECT_HEADERS catalogue.h )

qt4_wrap_cpp( MOCD_FILES ${Q_OBJECT_HEADERS} )

add_executable( ${OUTPUT_NAME} ${SOURCES} ${MOCD_FILES} )
#add_executable( ${OUTPUT_NAME} main.cpp )

target_link_libraries( ${OUTPUT_NAME}  ${QT_LIBRARIES} )

install( TARGETS ${OUTPUT_NAME}
         RUNTIME DESTINATION bin
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib )


# Top-level build file for Barpi
