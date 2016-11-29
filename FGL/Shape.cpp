#include "Shape.h"

using namespace fgl;
using namespace glm;

#include <glm/gtc/constants.hpp>

Box::Box(glm::vec3 size, glm::vec3 center):size(size),center(center){
}

GeometryS Box::toGeometry(){
  auto g = newGeometry();
  g->pos;

}

Sphere::Sphere(float radius, glm::vec3 center):radius(radius),center(center){
}

GeometryS fgl::Sphere::toGeometry(int tesX, int tesY){
  auto g = newGeometry();
  const int vertiesPerFace = 6;
  int sphereVertices = tesX*tesX*vertiesPerFace;
  for (int y = 0; y < tesY; ++y) {
    for (int x = 0; x < tesX; ++x) {
      for (int k = 0; k < vertiesPerFace; ++k) {
        const int xOffset[] = { 0,1,0,0,1,1 };
        const int yOffset[] = { 0,0,1,1,0,1 };
        float u = (float)(x + xOffset[k]) / tesX;
        float v = (float)(y + yOffset[k]) / tesY;
        float xAngle = -u*two_pi<float>();
        float yAngle = v*pi<float>();
        vec3 normal(cos(xAngle)*sin(yAngle), -cos(yAngle), sin(xAngle)*sin(yAngle));
        vec3 position = normal*radius;
        vec2 texCoord(u, v);
        g->pos.emplace_back(position);
        g->norm.emplace_back(normal);
        g->tc.emplace_back(texCoord);
      }
    }
  }
  return g;
}