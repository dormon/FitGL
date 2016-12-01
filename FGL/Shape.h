#pragma once
#include "Shared.h"
namespace fgl {
  classShared(Shape);
  classShared(Quad);
  classShared(Box);
  classShared(Sphere);
};

#include <glm/glm.hpp>
#include <Geometry.h>

class fgl::Shape {
public:
  virtual GeometryS toGeometry() = 0;
};

class fgl::Quad : public fgl::Shape {
public:
  Quad(glm::vec2 size = glm::vec2(1, 1), glm::vec3 normal = glm::vec3(0, 1, 0));
  virtual GeometryS toGeometry();

  glm::vec3 normal;
  glm::vec2 size;
};

class fgl::Box : public fgl::Shape {
public:
  Box(glm::vec3 size = glm::vec3(1, 1, 1), glm::vec3 center = glm::vec3(0, 0, 0));
  virtual GeometryS toGeometry();

  glm::vec3 center;
  glm::vec3 size;
};

class fgl::Sphere : public fgl::Shape {
public:
  Sphere(float radius = 1, glm::vec3 center = glm::vec3(0, 0, 0));
  virtual GeometryS toGeometry() { return toGeometry(40,40); }
  virtual GeometryS toGeometry(int tesX, int tesY);

  glm::vec3 center;
  float radius;
};