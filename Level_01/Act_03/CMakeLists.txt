add_executable(Level_01_Act_03 Program.cpp)

target_link_libraries(Level_01_Act_03 PRIVATE
        GLAD
        GLFW
        GLM
        opengl32
        glu32
)

set_target_properties(Level_01_Act_03 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 03"
)

add_custom_command(TARGET Level_01_Act_03 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/Binaries/$<CONFIG>/glfw3.dll"
        "$<TARGET_FILE_DIR:Level_01_Act_03>/glfw3.dll"
)