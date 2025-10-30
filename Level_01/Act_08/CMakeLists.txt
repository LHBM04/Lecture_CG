add_executable(Level_01_Act_08
        Program.cpp
)

target_link_libraries(Level_01_Act_08 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_08 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 08"
)

add_custom_command(TARGET Level_01_Act_08 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_08/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_08>/Vertex.glsl"
)

add_custom_command(TARGET Level_01_Act_08 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_08/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_08>/Fragment.glsl"
)