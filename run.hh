#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "include/game.hh"
#include "include/resource_manager.hh"
#include "include/info.hh"
#include "include/font.hh"

#include <iostream>

// GLFW function declarations
void mouse_callback(GLFWwindow* window, int button, int state, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);