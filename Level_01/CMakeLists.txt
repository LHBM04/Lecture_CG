# GLEW
add_library(GLEW STATIC IMPORTED)
set_target_properties(GLEW PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/glew32d.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/glew32.lib"
)

# FreeGLUT
add_library(FreeGLUT STATIC IMPORTED)
set_target_properties(FreeGLUT PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/freeglutd.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/freeglut.lib"
)

# GLM
add_library(GLM STATIC IMPORTED)
set_target_properties(GLM PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/glm.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/glm.lib"
)

add_subdirectory("Act_01")
add_subdirectory("Act_02")
add_subdirectory("Act_03")
add_subdirectory("Act_04")
add_subdirectory("Act_05")
add_subdirectory("Act_06")
add_subdirectory("Act_07")