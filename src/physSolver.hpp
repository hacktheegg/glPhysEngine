#ifndef PHYSSOLVER
#define PHYSSOLVER

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "renderer.hpp"





namespace physSolver {

struct physObject {

  float positionOld[2] = { 0.0f, 0.0f };
  float position[2] = { 0.0f, 0.0f };
  float radius = 0.1f;

  float acceleration[2] = { 0.0f, 0.0f };

};

std::vector<physObject> objects = {};

physObject constraint;



void setConstraint(std::vector<float> position = { 0.0f, 0.0f }, float radius = 0.9f) {

  constraint.radius = radius;
  constraint.position[0] = position[0];
  constraint.position[1] = position[1];

}

void step(double deltaTime) {

  // Apply Physics
  for (auto &obj : objects) {
    for (int i = 0; i < 2; i++) {

      float velocity = ( obj.position[i] - obj.positionOld[i] );

      if (i == 0) {
        velocity = ( obj.position[i] - obj.positionOld[i] ) * 0.995;
      }

      obj.positionOld[i] = obj.position[i];

      obj.position[i] += velocity + obj.acceleration[i] * deltaTime * deltaTime;

      obj.acceleration[i] = 0.0f;

    }

  }

}

void constrain() {

  for (auto &obj : objects) {

    float posDifference[2] = {
      obj.position[0] - constraint.position[0],
      obj.position[1] - constraint.position[1]
    };

    float distance = sqrtf((posDifference[0]*posDifference[0]) + (posDifference[1]*posDifference[1]));

    if ((distance + obj.radius) > constraint.radius) {

      float movementLine[2] = {
        posDifference[0] / distance,
        posDifference[1] / distance
      };

      obj.position[0] = constraint.position[0] + movementLine[0] * (constraint.radius - obj.radius);
      obj.position[1] = constraint.position[1] + movementLine[1] * (constraint.radius - obj.radius);

    }

  }

}

void collide() {

  for (auto &obj1 : objects) {

    for (auto &obj2 : objects) {

      if (&obj1 != &obj2) {

        float posDifference[2] = {
          obj1.position[0] - obj2.position[0],
          obj1.position[1] - obj2.position[1]
        };

        float distance = sqrtf((posDifference[0]*posDifference[0]) + (posDifference[1]*posDifference[1]));

        float movementLine[2] = {
          posDifference[0] / distance,
          posDifference[1] / distance
        };

        if (distance < obj1.radius + obj2.radius) {

          obj1.position[0] += movementLine[0] * 0.5f * ( (obj1.radius + obj2.radius) - distance );
          obj1.position[1] += movementLine[1] * 0.5f * ( (obj1.radius + obj2.radius) - distance );

          obj2.position[0] -= movementLine[0] * 0.5f * ( (obj1.radius + obj2.radius) - distance );
          obj2.position[1] -= movementLine[1] * 0.5f * ( (obj1.radius + obj2.radius) - distance );

        }

      }

    }

  }

}

void gravitate() {

  for (auto &obj1 : objects) {

    if (obj1.position[1] < 0.25f && std::abs(obj1.position[0]) < 0.075f) {
      obj1.acceleration[1] -= -9.8f / 1.5f;

      obj1.acceleration[0] -= obj1.position[0] * 75.0f;

    } else if (obj1.position[1] >= 0.25f) {
      if (obj1.position[0] < 0) { obj1.acceleration[0] += -0.1f; }
      else if (obj1.position[0] >= 0) { obj1.acceleration[0] += 0.1f; }
    } else {
      obj1.acceleration[1] += -9.8f;
    }

    /*
    for (auto &obj2 : objects) {

      float posDifference[2] = {
        obj1.position[0] - obj2.position[0],
        obj1.position[1] - obj2.position[1]
      };

      float distance = sqrtf((posDifference[0]*posDifference[0]) + (posDifference[1]*posDifference[1]));

      float movementLine[2] = {
        posDifference[0] / distance,
        posDifference[1] / distance
      };

      if (&obj1 != &obj2) {

          obj1.acceleration[0] -= movementLine[0] * ( 1 / ( 50 * ( distance )));
          obj1.acceleration[1] -= movementLine[1] * ( 1 / ( 50 * ( distance )));

      }

    }
    */

  }

}

void update(double deltaTime) {

  int substeps = 16;

  for (int i = 0; i < substeps; i++) {

    gravitate();
    step(deltaTime/substeps);
    constrain();
    collide();

  }

}

int newPhysObj(std::vector<float> position = { 0.0f, 0.0f }, float radius = 0.1f) {

  physObject object;
  object.positionOld[0] = position[0];
  object.positionOld[1] = position[1];
  object.position[0] = position[0];
  object.position[1] = position[1];
  object.radius = radius;

  objects.push_back(object);

  return objects.size()-1;

}

void renderObjects() {

  renderer::colour backgroundColour;
  backgroundColour.colour[0] = 0.4f;
  backgroundColour.colour[1] = 0.3f;
  backgroundColour.colour[2] = 0.2f;

  renderer::colour defaultColour;
  defaultColour.colour[0] = 0.3f;
  defaultColour.colour[1] = 0.6f;
  defaultColour.colour[2] = 0.9f;

  renderer::circle tmpCircle;
  tmpCircle.origin[0] = constraint.position[0];
  tmpCircle.origin[1] = constraint.position[1];
  tmpCircle.radius = constraint.radius;

  render::circle(tmpCircle, backgroundColour);

  for (auto &obj : objects) {
  
    tmpCircle.origin[0] = obj.position[0];
    tmpCircle.origin[1] = obj.position[1];
    tmpCircle.radius = obj.radius;

    render::circle(tmpCircle, defaultColour);

  }

}

}

#endif // PHYSSOLVER
