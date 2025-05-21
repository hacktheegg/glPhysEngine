// https://learnopengl.com/Getting-started/Hello-Window

#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

//#include "physSolver.hpp"
#include "renderer.hpp"
#include "shader_s.hpp"


int windowWidth = 512;
int windowHeight = 512;


bool clickState = false;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  std::cout << "Hello, World!" << std::endl;


  GLFWwindow *window = renderer::init(windowWidth, windowHeight, 1);


  //// SHADERS ////
  Shader ourShader("./shader.vert", "./shader.frag");

  renderer::getWindowHandle(window);


  std::vector<std::vector<std::vector<float>>> points = {};

  int counterX=0, counterY=0;
  for (float x = -0.9f; x <= 0.9f; x+=0.05f) {
    points.push_back({});
    for (float y = -0.9f; y <= 0.9f; y+=0.05f) {
      points[counterX].push_back({x,y});
      counterY++;
    }
    counterX++;
  }


  // uncomment this call to draw in wireframe polygons.
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {

    //std::cout << renderer::getWindowDimensions()[0] << std::endl;

    processInput(window);


    glClearColor(
      /*
      (sin(glfwGetTime() + (3.14f / 3 * 3)) + 1.0f) / 2.0f, // red
      (sin(glfwGetTime() + (3.14f / 3 * 2)) + 1.0f) / 2.0f, // green
      (sin(glfwGetTime() + (3.14f / 3 * 1)) + 1.0f) / 2.0f, // blue
      */
      0.10f, 0.10f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();


    std::vector<std::vector<std::vector<float>>> previousPoints = points;

    std::vector<float> avrPos = {0,0};
    int avrPosCounter = 0;

    for (int i = 0; i < points.size(); i++) {
      for (int j = 0; j < points[i].size(); j++) {
        avrPosCounter = 0;
        avrPos = {0,0};
        for (int x = -1; x <= 1; x++) {
          for (int y = -1; y <= 1; y++) {

            if (
              i+x >= 0 &&
              i+x <= points.size()-1 &&
              j+y >= 0 &
              j+y <= points[i].size()-1
            ){
              avrPos[0] += previousPoints[i+x][j+y][0];
              avrPos[1] += previousPoints[i+x][j+y][1];
              avrPosCounter++;
            }

          }
        }
        points[i][j][0] = avrPos[0]/((float)avrPosCounter+0.0);
        points[i][j][1] = avrPos[1]/((float)avrPosCounter+0.0);
      }
    }

    points[0][0] = {0,0};
    points[points.size()-1][0] = {0,0};
    points[0][points[0].size()-1] = {0,0};
    points[points.size()-1][points[points.size()-1].size()-1] = {0,0};

    //points[(int)points.size()/2][(int)points[(int)points.size()/2].size()/2][0] = sin(glfwGetTime()*M_PI*3.0f)/0.05f;
    //points[(int)points.size()/2][(int)points[(int)points.size()/2].size()/2][1] = cos(glfwGetTime()*M_PI*3.0f)/0.05f;

    for (auto &row : points) {
      for (auto &obj : row) {
        render::standard::circle(renderer::standard::circle(obj,0.01f), renderer::colour({(obj[0]+1)/2.5f,(obj[1]+1)/2.5f,1.0f-(obj[0]+1)/2.5f}));
      }
    }


    // Modes (Swap out the first object given to glDrawElements
    /*
    * GL_POINTS
    * GL_LINES
    * GL_LINE_STRIP
    * GL_LINE_LOOP
    * GL_TRIANGLES
    * GL_TRIANGLE_STRIP
    * GL_TRIANGLE_FAN
    * autocomplete says there are others as well
    */
    glfwSwapBuffers(window);
    glfwPollEvents();
  }





  /*
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  */
  glfwTerminate();
  return 0;
}
