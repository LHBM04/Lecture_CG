add_executable(Level_01_Act_20
        Sources/Application.cpp
        Sources/Application.h
        Sources/Main.cpp
        Sources/Shader.cpp
        Sources/Shader.h
 )

target_link_libraries(Level_01_Act_20 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_20 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 20"
)

add_custom_command(TARGET Level_01_Act_20 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_20/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_20>/Resources"
        VERBATIM
)