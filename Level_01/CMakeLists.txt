# GLEW
add_library(GLAD STATIC IMPORTED)
set_target_properties(GLAD PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/glad.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/glad.lib"
)

# GLFW
add_library(GLFW STATIC IMPORTED)
set_target_properties(GLFW PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/glfw3dll.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/glfw3dll.lib"
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