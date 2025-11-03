add_executable(Level_01_Act_18
        Sources/Core/Application.cpp
        Sources/Core/Application.h
        Sources/Core/File.cpp
        Sources/Core/File.h
        Sources/Core/Input.cpp
        Sources/Core/Input.h
        Sources/Core/Time.cpp
        Sources/Core/Time.h

        Sources/Objects/Axes.cpp
        Sources/Objects/Axes.h
        Sources/Objects/Object.cpp
        Sources/Objects/Object.h
        Sources/Objects/Planet.cpp
        Sources/Objects/Planet.h

        Sources/Rendering/Camera.cpp
        Sources/Rendering/Camera.h
        Sources/Rendering/Mesh.cpp
        Sources/Rendering/Mesh.h
        Sources/Rendering/Shader.cpp
        Sources/Rendering/Shader.h

        Sources/Main.cpp
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