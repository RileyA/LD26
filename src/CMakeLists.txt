set(SUBSYSTEMS
	${OGRE_LIB}
	${AL_LIB}
	${OIS_LIB}
	${BULLET_LIB}
	${GUI_LIB}
	)

set(SOURCE_FILES
	PlayState.cpp
	main.cpp
	FPSCamera.cpp
	ThreadPool.cpp
  TerrainTile.cpp
  TerrainManager.cpp
  Enemy.cpp
  Ruins.cpp
  Powerup.cpp
  MenuState.cpp
	)

set(HEADER_FILES
	PlayState.h
	FPSCamera.h
	ThreadPool.h
  TerrainTile.h
  TerrainManager.h
  iVec2.h
  Enemy.h
  Ruins.h
  MenuState.h
  Powerup.h
	)

add_executable(ld26 ${SOURCE_FILES} ${HEADER_FILES} )
set_target_properties(ld26 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries (ld26 ${ORYX_LIB} ${BOOST_LIB} ${SUBSYSTEMS} ${NOISE_LIB})
