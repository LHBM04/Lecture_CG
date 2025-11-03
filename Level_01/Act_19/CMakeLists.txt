add_executable(Level_01_Act_19
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
        Sources/Objects/Cone.cpp
        Sources/Objects/Cone.h
        Sources/Objects/Cube.cpp
        Sources/Objects/Cube.h
        Sources/Objects/Object.cpp
        Sources/Objects/Object.h

        Sources/Rendering/Camera.cpp
        Sources/Rendering/Camera.h
        Sources/Rendering/Cylinder.cpp
        Sources/Rendering/Cylinder.h
        Sources/Rendering/Disk.cpp
        Sources/Rendering/Disk.h
        Sources/Rendering/Model.cpp
        Sources/Rendering/Model.h
        Sources/Rendering/Shader.cpp
        Sources/Rendering/Shader.h
        Sources/Rendering/Sphere.cpp
        Sources/Rendering/Sphere.h

        Sources/World/World.cpp
        Sources/World/World.h

        Sources/Main.cpp
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