add_executable(Level_01_Act_22
    "Main.cpp"
)

target_link_libraries(Level_01_Act_22 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_22 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 22"
)

add_custom_command(TARGET Level_01_Act_22 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_22/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_22>/Resources"
        VERBATIM
)