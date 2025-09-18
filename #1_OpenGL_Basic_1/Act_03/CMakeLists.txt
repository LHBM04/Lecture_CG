add_executable(1_Act_03 Program.cpp)

target_link_libraries(1_Act_03 PRIVATE
    GLEW
    FreeGLUT
    GLM
    opengl32
    glu32
)

set_target_properties(1_Act_03 PROPERTIES
        RUNTIME_OUTPUT_NAME "Act 3"
)