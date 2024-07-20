#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "include/game.hh"
#include "include/resource_manager.hh"
#include "include/info.hh"

#include <iostream>

// GLFW function declarations
void mouseCallback(GLFWwindow* window, int button, int state, int mods);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void characterCallback(GLFWwindow* window, unsigned int codepoint);
void framebufferResizeCallback(GLFWwindow* window, int width, int height);