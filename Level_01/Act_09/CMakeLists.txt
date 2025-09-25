add_executable(Level_01_Act_09
        Program.cpp
)

target_link_libraries(Level_01_Act_09 PRIVATE
        GLAD
        GLFW
        GLM
        opengl32
        glu32
)

set_target_properties(Level_01_Act_09 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 09"
)

add_custom_command(TARGET Level_01_Act_09 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/Binaries/$<CONFIG>/glfw3.dll"
        "$<TARGET_FILE_DIR:Level_01_Act_09>/glfw3.dll"
)

add_custom_command(TARGET Level_01_Act_09 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_09/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_09>/Vertex.glsl"
)

add_custom_command(TARGET Level_01_Act_09 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_09/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_09>/Fragment.glsl"
)