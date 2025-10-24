set(EXTERNAL_INCLUDES_DIR  "${CMAKE_SOURCE_DIR}/External/Includes")
set(EXTERNAL_LIBRARIES_DIR "${CMAKE_SOURCE_DIR}/External/Libraries/${CMAKE_BUILD_TYPE}")

# GLEW
add_library(GLAD STATIC IMPORTED)
set_target_properties(GLAD PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${EXTERNAL_LIBRARIES_DIR}/glad.lib"
        IMPORTED_LOCATION_RELEASE     "${EXTERNAL_LIBRARIES_DIR}/glad.lib"
)

# GLFW
add_library(GLFW STATIC IMPORTED)
set_target_properties(GLFW PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${EXTERNAL_LIBRARIES_DIR}/glfw3dll.lib"
        IMPORTED_LOCATION_RELEASE     "${EXTERNAL_LIBRARIES_DIR}/glfw3dll.lib"
)

# GLM
add_library(GLM STATIC IMPORTED)
set_target_properties(GLM PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${EXTERNAL_LIBRARIES_DIR}/glm.lib"
        IMPORTED_LOCATION_RELEASE     "${EXTERNAL_LIBRARIES_DIR}/glm.lib"
)

# fmt
add_library(Fmt STATIC IMPORTED)
set_target_properties(Fmt PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${EXTERNAL_LIBRARIES_DIR}/fmtd.lib"
        IMPORTED_LOCATION_RELEASE     "${EXTERNAL_LIBRARIES_DIR}/fmt.lib"
)

# spdlog
add_library(Spdlog STATIC IMPORTED)
set_target_properties(Spdlog PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/External/Includes"
        IMPORTED_LOCATION_DEBUG       "${EXTERNAL_LIBRARIES_DIR}/spdlogd.lib"
        IMPORTED_LOCATION_RELEASE     "${EXTERNAL_LIBRARIES_DIR}/spdlog.lib"
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
add_subdirectory("Act_16")
add_subdirectory("Act_17")
add_subdirectory("Act_18")