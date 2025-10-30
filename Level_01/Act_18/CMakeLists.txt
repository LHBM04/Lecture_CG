add_executable(Level_01_Act_18
        Program.cpp
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
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Shaders/Vertex.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Shaders/Vertex.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Shaders/Fragment.glsl"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Shaders/Fragment.glsl"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Objects/Cube.obj"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Objects/Cube.obj"
        VERBATIM
)

add_custom_command(TARGET Level_01_Act_18 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Level_01/Act_18/Resources/Objects/Diamond.obj"
        "$<TARGET_FILE_DIR:Level_01_Act_18>/Resources/Objects/Diamond.obj"
        VERBATIM
)