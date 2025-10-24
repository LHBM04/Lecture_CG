set(EXTERNAL_BINARIES_DIR "${CMAKE_SOURCE_DIR}/External/Binaries/${CMAKE_BUILD_TYPE}")

add_executable(Level_01_Act_18
        Program.cpp
)

target_link_libraries(Level_01_Act_18 PRIVATE
        GLAD
        GLFW
        GLM
        Fmt
        Spdlog
        opengl32
        glu32
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Shaders/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Shaders/Vertex.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Shaders/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Shaders/Fragment.glsl"
        VERBATIM
)

set_target_properties(Level_01_Act_18 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 18"
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Objects/Cube.obj"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Objects/Cube.obj"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Objects/Diamond.obj"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Objects/Diamond.obj"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${EXTERNAL_BINARIES_DIR}/$<IF:$<CONFIG:Debug>,glfw3.dll,glfw3.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/glfw3.dll"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${EXTERNAL_BINARIES_DIR}/$<IF:$<CONFIG:Debug>,fmtd.dll,fmt.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/$<IF:$<CONFIG:Debug>,fmtd.dll,fmt.dll>"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${EXTERNAL_BINARIES_DIR}/$<IF:$<CONFIG:Debug>,spdlogd.dll,spdlog.dll>"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/$<IF:$<CONFIG:Debug>,spdlogd.dll,spdlog.dll>"
        VERBATIM
)