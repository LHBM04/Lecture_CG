add_executable(Level_01_Act_19
        Sources/Program.cpp

        Sources/Graphics/Camera.cpp
        Sources/Graphics/Camera.h
        Sources/Graphics/Shader.cpp
        Sources/Graphics/Shader.h

        Sources/Objects/Object.cpp
        Sources/Objects/Object.h
)

target_link_libraries(Level_01_Act_19 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_19 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 19"
)

add_custom_command(TARGET Level_01_Act_19 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_19/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_19>/Resources"
        VERBATIM
)