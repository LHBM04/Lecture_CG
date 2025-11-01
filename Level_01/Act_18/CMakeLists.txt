add_executable(Level_01_Act_18
        Sources/Graphics/Mesh.cpp
        Sources/Graphics/Mesh.h
        Sources/Graphics/Shader.cpp
        Sources/Graphics/Shader.h

        Sources/Objects/Axes.cpp
        Sources/Objects/Axes.h
        Sources/Objects/Cone.cpp
        Sources/Objects/Cone.h
        Sources/Objects/Cube.cpp
        Sources/Objects/Cube.h
        Sources/Objects/Object.cpp
        Sources/Objects/Object.h

        Sources/Program.cpp
)

target_link_libraries(Level_01_Act_18 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_18 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 18"
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources"
        VERBATIM
)