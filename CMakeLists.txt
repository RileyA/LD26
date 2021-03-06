cmake_minimum_required (VERSION 2.6)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# Some lib stuff, probably unneeded, but I don't know cmake too well yet...
if (WIN32)
	set(LIB_PREFIX )
	set(LIB_POSTFIX .lib)
else ()
	set(LIB_PREFIX lib)
	set(LIB_POSTFIX .so)
endif ()

# Some options:
set(INSTALL_RUNTIMES_TO "/usr/bin" CACHE STRING "Where install executables.")
set(INSTALL_LIBS_TO "/usr/lib" CACHE STRING "Where to install libs.")

set(ORYX_INCLUDE_DIR "../OryxEngine/src" CACHE STRING "Where to look for Oryx includes.")
set(ORYX_LIB_DIR "/usr/lib" CACHE STRING "Where to look for Oryx libs.")
set(ORYX_SUBSYSTEM_INCLUDE_DIR "../OryxEngine/src" CACHE STRING "Where to look for Oryx Subsystem includes.")
set(ORYX_SUBSYSTEM_LIB_DIR "/usr/lib" CACHE STRING "Where to look for Oryx Subsystems.")

set(BOOST_INCLUDE_DIR "/usr/include" CACHE STRING "Where to look for boost includes.")
set(BOOST_LIB_DIR "/usr/lib" CACHE STRING "Where to look for boost libs.")

# libnoise
set(NOISE_INCLUDE_DIR "/usr/include" CACHE STRING "Where to look for libnoise includes.")
set(NOISE_LIB_DIR "/usr/lib" CACHE STRING "Where to look for libnoise libs.")

# all the oryx libs
set(ORYX_LIB ${ORYX_LIB_DIR}/${LIB_PREFIX}OryxEngine${LIB_POSTFIX})
if (WIN32)
	set(OGRE_LIB ${ORYX_LIB_DIR}/OgreSubsystem/${LIB_PREFIX}OgreSubsystem${LIB_POSTFIX})
	set(AL_LIB ${ORYX_LIB_DIR}/ALSubsystem/${LIB_PREFIX}ALSubsystem${LIB_POSTFIX})
	set(GUI_LIB ${ORYX_LIB_DIR}/GUISubsystem/${LIB_PREFIX}GUISubsystem${LIB_POSTFIX})
	set(BULLET_LIB ${ORYX_LIB_DIR}/BulletSubsystem/${LIB_PREFIX}BulletSubsystem${LIB_POSTFIX})
	set(OIS_LIB ${ORYX_LIB_DIR}/OISSubsystem/${LIB_PREFIX}OISSubsystem${LIB_POSTFIX})
  set(NOISE_LIB ${NOISE_LIB_DIR}/${LIB_PREFIX}libnoise${LIB_POSTFIX})
else ()
	set(OGRE_LIB ${ORYX_LIB_DIR}/${LIB_PREFIX}OgreSubsystem${LIB_POSTFIX})
	set(AL_LIB ${ORYX_LIB_DIR}/${LIB_PREFIX}ALSubsystem${LIB_POSTFIX})
	set(GUI_LIB ${ORYX_LIB_DIR}/${LIB_PREFIX}GUISubsystem${LIB_POSTFIX})
	set(BULLET_LIB ${ORYX_LIB_DIR}/${LIB_PREFIX}BulletSubsystem${LIB_POSTFIX})
	set(OIS_LIB ${ORYX_LIB_DIR}/${LIB_PREFIX}OISSubsystem${LIB_POSTFIX})
  set(NOISE_LIB ${NOISE_LIB_DIR}/${LIB_PREFIX}noise${LIB_POSTFIX})
endif ()

if (WIN32)
	set(BOOST_THREAD_LIB ${BOOST_LIB_DIR}/${LIB_PREFIX}libboost_thread-vc100-mt-1_44${LIB_POSTFIX})
	set(BOOST_TIME_LIB ${BOOST_LIB_DIR}/${LIB_PREFIX}libboost_date_time-vc100-mt-1_44${LIB_POSTFIX})
else ()
	set(BOOST_THREAD_LIB ${BOOST_LIB_DIR}/${LIB_PREFIX}boost_thread${LIB_POSTFIX})
endif ()

set(BOOST_LIB ${BOOST_THREAD_LIB} ${BOOST_TIME_LIB})

include_directories("${ORYX_INCLUDE_DIR}")
include_directories("${BOOST_INCLUDE_DIR}")
include_directories("${NOISE_INCLUDE_DIR}")
include_directories("${ORYX_SUBSYSTEM_INCLUDE_DIR}")
include_directories("src")

# copy art assets and scripts over to the bin dir
file(COPY ./media DESTINATION ./)

add_subdirectory(src)
