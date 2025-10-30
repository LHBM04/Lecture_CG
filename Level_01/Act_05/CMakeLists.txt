add_executable(Level_01_Act_05 Program.cpp)

target_link_libraries(Level_01_Act_05 PRIVATE
        GLEW::GLEW
        glad::glad
        glfw
        FreeGLUT::freeglut
        OpenGL::GL
        glm::glm
        spdlog::spdlog
)

set_target_properties(Level_01_Act_05 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 05"
)