add_executable(Level_01_Act_21
        "Sources/Main.cpp"
)

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