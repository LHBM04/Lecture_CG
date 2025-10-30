add_executable(Level_01_Act_11
        Program.cpp
)

target_link_libraries(Level_01_Act_11 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_11 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 11"
)

add_custom_command(TARGET Level_01_Act_11 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_11/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_11>/Vertex.glsl"
)

add_custom_command(TARGET Level_01_Act_11 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_11/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_11>/Fragment.glsl"
)