#ifndef PENDULUM
#define PENDULUM

#include <renderer.hpp>
#include <cmath>

namespace dPendulum {

class pendulum {
  public:
  float *originPosition;
  float *pendulumPosition;
  float angle;
  float length;
  float mass;

  pendulum() { originPosition = new float[2] {0.0f,0.0f}; pendulumPosition = new float[2] {0.0f,0.0f}; angle = 45.0f; length = 0.5f; mass = 0.01f; }
  pendulum(float *inputOriginPostion, float *inputPendulumPosition, float inputAngle, float inputLength, float inputMass) {
    originPosition = inputOriginPostion; pendulumPosition = inputPendulumPosition; angle = inputAngle; length = inputLength; mass = inputMass;
  }

  void updatePendulumPosition() {
    pendulumPosition[0] = sin((angle/180)*M_PI)*length + originPosition[0];
    pendulumPosition[1] = -cos((angle/180)*M_PI)*length + originPosition[1];
  }

  void render(renderer::colour colour) {
    renderer::standard::line mainLine({
      {
        originPosition[0],
        originPosition[1]
      },
      {
        pendulumPosition[0],
        pendulumPosition[1]
      }
    });
    renderer::standard::circle mainCircle({pendulumPosition[0],pendulumPosition[1]},mass);
    render::standard::line(mainLine, colour);
    render::standard::circle(mainCircle, colour);
  }

};

}

#endif // PENDULUM
