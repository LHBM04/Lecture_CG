add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(Level_01_Act_12
        Main.cpp
        Program.cpp
        Shader.cpp
        Shape.cpp
)

target_link_libraries(Level_01_Act_12 PRIVATE
        GLAD
        GLFW
        GLM
        Fmt
        Spdlog
        opengl32
        glu32
)

set_target_properties(Level_01_Act_12 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 12"
)

add_custom_command(TARGET Level_01_Act_12 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/External/Binaries/Debug/glfw3.dll,${CMAKE_SOURCE_DIR}/External/Binaries/Release/glfw3.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/glfw3.dll"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_12 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/External/Binaries/Debug/fmtd.dll,${CMAKE_SOURCE_DIR}/External/Binaries/Release/fmt.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/$<IF:$<CONFIG:Debug>,fmtd.dll,fmt.dll>"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_12 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<IF:$<CONFIG:Debug>,${CMAKE_SOURCE_DIR}/External/Binaries/Debug/spdlogd.dll,${CMAKE_SOURCE_DIR}/External/Binaries/Release/spdlog.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/$<IF:$<CONFIG:Debug>,spdlogd.dll,spdlog.dll>"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_12 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_12/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/Vertex.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_12 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_12/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/Fragment.glsl"
        VERBATIM
)