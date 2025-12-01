add_executable(Level_01_Act_30
        "Sources/Application.cpp"
        "Sources/Camera.cpp"
        "Sources/Input.cpp"
        "Sources/Light.cpp"
        "Sources/Main.cpp"
        "Sources/Mesh.cpp"
        "Sources/Object.cpp"
        "Sources/Shader.cpp"
        "Sources/Texture.cpp"
)

target_link_libraries(Level_01_Act_30 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

target_include_directories(Level_01_Act_30 PRIVATE ${Stb_INCLUDE_DIR})

target_precompile_headers(Level_01_Act_30 PRIVATE
        "Sources/PCH.h"
)

set_target_properties(Level_01_Act_30 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 30"
)

add_custom_command(TARGET Level_01_Act_30 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_30/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_30>/Resources"
        VERBATIM
)