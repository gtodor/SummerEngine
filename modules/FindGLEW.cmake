# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIRS
# GLEW_LIBRARY
# GLEW_DLL
# 

IF (WIN32)
	FIND_PATH( GLEW_INCLUDE_DIRS GL/glew.h
		${PROJECT_SOURCE_DIR}/extern/glew-2.1.0/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES glew GLEW glew32 glew32s
		PATHS
		${PROJECT_SOURCE_DIR}/extern/glew-2.1.0/lib/Release/x64
		DOC "The GLEW library")
	set(GLEW_DLL "${PROJECT_SOURCE_DIR}/extern/glew-2.1.0/bin/Release/x64/glew32.dll")
ELSE (WIN32)
	FIND_PATH( GLEW_INCLUDE_DIRS GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES GLEW glew
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		DOC "The GLEW library")
ENDIF (WIN32)

IF (GLEW_INCLUDE_DIRS)
	SET( GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ELSE (GLEW_INCLUDE_DIRS)
	SET( GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ENDIF (GLEW_INCLUDE_DIRS)

MARK_AS_ADVANCED( GLEW_FOUND )
