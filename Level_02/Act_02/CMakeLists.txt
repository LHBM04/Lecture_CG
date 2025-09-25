add_executable(Level_2_Act_02 Program.cpp)

target_link_libraries(Level_2_Act_02 PRIVATE
        GLAD
        GLFW
        GLM
        opengl32
        glu32
)

set_target_properties(Level_2_Act_02 PROPERTIES
        RUNTIME_OUTPUT_NAME "Act 1"
)

add_custom_command(TARGET Level_2_Act_02 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/Binaries/$<CONFIG>/glfw3.dll"
        "$<TARGET_FILE_DIR:Level_2_Act_02>/glfw3.dll"
)