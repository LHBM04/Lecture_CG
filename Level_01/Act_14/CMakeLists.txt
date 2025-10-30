add_executable(Level_01_Act_14
        Program.cpp
)

target_link_libraries(Level_01_Act_14 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_14 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 14"
)

add_custom_command(TARGET Level_01_Act_14 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_14/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_14>/Vertex.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_14 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_14/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_14>/Fragment.glsl"
        VERBATIM
)