add_executable(Level_01_Act_23
        "Sources/Application.cpp" 
        "Sources/Camera.cpp" 
        "Sources/File.cpp"
        "Sources/Input.cpp"
        "Sources/Main.cpp"
        "Sources/Mesh.cpp"
        "Sources/Object.cpp"
        "Sources/Shader.cpp" 
        "Sources/Tank.cpp"
)

target_link_libraries(Level_01_Act_23 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_23 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 23"
)

add_custom_command(TARGET Level_01_Act_23 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_23/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_23>/Resources"
        VERBATIM
)