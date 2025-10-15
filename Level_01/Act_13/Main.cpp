#include <array>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>

#include <glad/glad.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <spdlog/spdlog.h>

#include "Application.h"

int main()
{
    Initialize();
    Run();
}