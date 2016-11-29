#pragma once
#include "Shared.h"
namespace fgl {
  classShared(Shape);
  classShared(Sphere);
  classShared(Box);
};

#include <glm/glm.hpp>
#include <Geometry.h>

class fgl::Shape {
public:
  virtual GeometryS toGeometry() = 0;
};

class fgl::Box : public fgl::Shape {
public:
  Box(glm::vec3 size = glm::vec3(1, 1, 1), glm::vec3 center = glm::vec3(0, 0, 0));
  virtual GeometryS toGeometry() = 0;

  glm::vec3 center;
  glm::vec3 size;
};

class fgl::Sphere : public fgl::Shape {
public:
  Sphere(float radius = 1, glm::vec3 center = glm::vec3(0, 0, 0));
  virtual GeometryS toGeometry() { return toGeometry(20,20); }
  virtual GeometryS toGeometry(int tesX, int tesY);

  glm::vec3 center;
  float radius;
};