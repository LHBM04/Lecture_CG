add_executable(1_Act_01 Program.cpp)

target_link_libraries(1_Act_01 PRIVATE
    GLAD
    GLFW
    GLM
    opengl32
    glu32
)

set_target_properties(1_Act_01 PROPERTIES
        RUNTIME_OUTPUT_NAME "Act 1"
)