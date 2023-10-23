#ifndef RUN_HH
#define RUN_HH

#include <cmath>
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

#include "include/stb_image.h"

#include "include/shader.hh"
#include "include/card.hh"

/**
 * @return current window
 */
GLFWwindow* setup_window();

/**
 * @brief initialize project setting (vertex array, vertex buffer, element buffer)
*/
void setup_project(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO);

/**
 * @brief main gameplay loop, so that destructors can be called before the window is closed
 */
void run(GLFWwindow* window);

/**
  * @brief function to be run on window resize
  * @param window current window
  * @param width new window width
  * @param height new window height
  */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

#endif