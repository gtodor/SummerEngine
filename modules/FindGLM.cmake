#
# Find GLM

if (WIN32)
	# Find include files
	find_path(
		GLM_INCLUDE_DIRS
		NAMES glm/glm.hpp
		PATHS
		${PROJECT_SOURCE_DIR}/extern/glm
		DOC "The directory where glm/glm.hpp resides")
else()
	# Find include files
	find_path(
		GLM_INCLUDE_DIRS
		NAMES glm/glm.hpp
		PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${GLM_ROOT_DIR}/include
		DOC "The directory where glm/glm.hpp resides")
endif()

# Define GLM_INCLUDE_DIRS
if (GLM_INCLUDE_DIRS)
	SET( GLM_FOUND 1 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
else(GLM_INCLUDE_DIRS)
	SET( GLM_FOUND 0 CACHE STRING "Set to 1 if GLM is found, 0 otherwise")
endif(GLM_INCLUDE_DIRS)

mark_as_advanced(GLM_FOUND)