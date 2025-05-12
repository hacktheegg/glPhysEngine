// https://learnopengl.com/Getting-started/Hello-Window

#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pendulum.hpp"
//#include "physSolver.hpp"
#include "renderer.hpp"
#include "shader_s.hpp"





bool clickState = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  std::cout << "Hello, World!" << std::endl;

  // initialise GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  // initialise GLFW window
  GLFWwindow *window = glfwCreateWindow(512, 512, "glThings", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // initialise GLAD function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //// SHADERS ////
  Shader ourShader("./shader.vert", "./shader.frag");


  renderer::colour green({0.0f,1.0f,0.0f});

  dPendulum::pendulum innerPendulum;
  dPendulum::pendulum outerPendulum;
  delete [] outerPendulum.originPosition;
  outerPendulum.originPosition = innerPendulum.pendulumPosition;
  outerPendulum.angle = 90.0f;
  innerPendulum.updatePendulumPosition();
  outerPendulum.updatePendulumPosition();

  
  renderer::init();


  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  double oldTime = glfwGetTime();
  double newTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {

    newTime = glfwGetTime();
    double deltaTime = ( newTime - oldTime ) * 1.0f;

    processInput(window);

    glClearColor(
      /*
      (sin(glfwGetTime() + (3.14f / 3 * 3)) + 1.0f) / 2.0f, // red
      (sin(glfwGetTime() + (3.14f / 3 * 2)) + 1.0f) / 2.0f, // green
      (sin(glfwGetTime() + (3.14f / 3 * 1)) + 1.0f) / 2.0f, // blue
      */
      0.80f, 0.40, 0.20, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    innerPendulum.render(green);
    outerPendulum.render(green);




    ourShader.use();

    glfwSwapBuffers(window);
    glfwPollEvents();

    oldTime = newTime;
  }

  /*
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  */
  glfwTerminate();
  return 0;
}

