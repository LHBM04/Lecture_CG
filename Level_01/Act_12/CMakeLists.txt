add_executable(Level_01_Act_12
        Program.cpp
)

target_link_libraries(Level_01_Act_12 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

add_custom_command(TARGET Level_01_Act_12 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_12/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/Vertex.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_12 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_12/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_12>/Fragment.glsl"
        VERBATIM
)

set_target_properties(Level_01_Act_12 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 12"
)