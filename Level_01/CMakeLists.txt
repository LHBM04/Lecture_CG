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

# fmt
add_library(Fmt STATIC IMPORTED)
set_target_properties(Fmt PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/fmtd.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/fmt.lib"
)

# spdlog
add_library(Spdlog STATIC IMPORTED)
set_target_properties(Spdlog PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${CMAKE_SOURCE_DIR}/External/Libraries/Debug/spdlogd.lib"
        IMPORTED_LOCATION_RELEASE     "${CMAKE_SOURCE_DIR}/External/Libraries/Release/spdlog.lib"
)

add_subdirectory("Act_01")
add_subdirectory("Act_02")
add_subdirectory("Act_03")
add_subdirectory("Act_04")
add_subdirectory("Act_05")
add_subdirectory("Act_06")
add_subdirectory("Act_07")
add_subdirectory("Act_08")
add_subdirectory("Act_09")
add_subdirectory("Act_10")
add_subdirectory("Act_11")
add_subdirectory("Act_12")
add_subdirectory("Act_13")
add_subdirectory("Act_14")
add_subdirectory("Act_15")