add_executable(Level_01_Act_06 Program.cpp)

target_link_libraries(Level_01_Act_06 PRIVATE
        GLAD
        GLFW
        GLM
        opengl32
        glu32
)

set_target_properties(Level_01_Act_06 PROPERTIES
        RUNTIME_OUTPUT_NAME "Level 01 - Act 06"
)

add_custom_command(TARGET Level_01_Act_06 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/Binaries/$<CONFIG>/glfw3.dll"
        "$<TARGET_FILE_DIR:Level_01_Act_06>/glfw3.dll"
)