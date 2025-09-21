add_executable(1_Act_02 Program.cpp)

target_link_libraries(1_Act_02 PRIVATE
        GLAD
        GLFW
        GLM
        opengl32
        glu32
)

set_target_properties(1_Act_02 PROPERTIES
        RUNTIME_OUTPUT_NAME "Act 2"
)

add_custom_command(TARGET 1_Act_02 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/Binaries/$<CONFIG>/glfw3.dll"
        "$<TARGET_FILE_DIR:1_Act_02>/glfw3.dll"
)