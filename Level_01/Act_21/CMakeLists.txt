add_executable(Level_01_Act_21
        "Sources/Application.cpp"
        "Sources/Camera.cpp"
        "Sources/Input.cpp"
        "Sources/Main.cpp"
        "Sources/Mesh.cpp"
        "Sources/Object.cpp"
        "Sources/Shader.cpp" "Sources/Stage.cpp" "Sources/Marble.cpp" "Sources/Block.cpp")

 target_precompile_headers(Level_01_Act_21 PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Sources/PCH.h"
 )

target_link_libraries(Level_01_Act_21 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_21 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 21"
)

add_custom_command(TARGET Level_01_Act_21 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Level_01/Act_21/Resources"
        "$<TARGET_FILE_DIR:Level_01_Act_21>/Resources"
        VERBATIM
)