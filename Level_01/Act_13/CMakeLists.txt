add_executable(Level_01_Act_13
        Application.cpp
        Main.cpp
        Shader.cpp
        Shape.cpp
)

target_link_libraries(Level_01_Act_13 PRIVATE
        GLAD
        GLFW
        GLM
        Fmt
        Spdlog
        opengl32
        glu32
)

add_custom_command(TARGET Level_01_Act_13 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_13/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_13>/Vertex.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_13 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_13/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_13>/Fragment.glsl"
        VERBATIM
)

set_target_properties(Level_01_Act_13 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 13"
)

add_custom_command(TARGET Level_01_Act_13 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/External/Binaries/Debug/glfw3.dll,${CMAKE_SOURCE_DIR}/External/Binaries/Release/glfw3.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_13>/glfw3.dll"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_13 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/External/Binaries/Debug/fmtd.dll,${CMAKE_SOURCE_DIR}/External/Binaries/Release/fmt.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_13>/$<IF:$<CONFIG:Debug>,fmtd.dll,fmt.dll>"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_13 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/External/Binaries/Debug/spdlogd.dll,${CMAKE_SOURCE_DIR}/External/Binaries/Release/spdlog.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_13>/$<IF:$<CONFIG:Debug>,spdlogd.dll,spdlog.dll>"
        VERBATIM
)