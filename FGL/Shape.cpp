#include "Shape.h"

using namespace fgl;
using namespace glm;

#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

Box::Box(glm::vec3 size, glm::vec3 center):size(size),center(center){
}

GeometryS Box::toGeometry(){
  auto g = newGeometry();
  


  return g;
}

Sphere::Sphere(float radius, glm::vec3 center):radius(radius),center(center){
}

GeometryS fgl::Sphere::toGeometry(int tesX, int tesY){
  auto g = newGeometry();
  for (int h = 0; h < tesX + 1; h++) {
    for (int v = 0; v < tesY + 1; v++) {
      float x, y, z;
      float radH = float(h) / tesX * pi<float>() * 2;
      float radV = float(v) / tesY *pi<float>();
      x = sin(radV) * cos(radH);
      y = -cos(radV);
      z = -sin(radV) * sin(radH);
      g->pos.emplace_back(radius*glm::vec3(x, y, z) + center);
      g->norm.emplace_back(glm::vec3(x, y, z));
      g->tc.emplace_back(glm::vec2(float(h) / tesX, float(v) / tesY));
    }
  }
  for (int h = 0; h < tesX; h++) {
    g->elements.emplace_back(h * (tesY + 1));
    g->elements.emplace_back((h + 1) % (tesY + 1) * (tesY + 1) + 1);
    g->elements.emplace_back(h * (tesY + 1) + 1);
    //side
    for (int v = 0; v < tesY - 2; v++) {
      g->elements.emplace_back(h * (tesY + 1) + 1 + v);
      g->elements.emplace_back((h + 1) % (tesX + 1) * (tesY + 1) + 1 + v + 1);
      g->elements.emplace_back(h * (tesY + 1) + 1 + v + 1);

      g->elements.emplace_back(h * (tesY + 1) + 1 + v);
      g->elements.emplace_back((h + 1) % (tesX + 1) * (tesY + 1) + 1 + v);
      g->elements.emplace_back((h + 1) % (tesX + 1) * (tesY + 1) + 1 + v + 1);
    }
    //bottom
    g->elements.emplace_back(h * (tesY + 1) + tesY - 1);
    g->elements.emplace_back((h + 1) % (tesX + 1) * (tesY + 1) + tesY - 1);
    g->elements.emplace_back(h * (tesY + 1) + tesY);
  }
  g->updateData();
  return g;
}

fgl::Quad::Quad(glm::vec2 size, glm::vec3 normal):size(size),normal(normal){
}

GeometryS fgl::Quad::toGeometry(){
  vec3 dir1, dir2;
  if (normal == vec3(0, 1, 0)) {
    dir1 = size.x*vec3(1, 0, 0);
    dir2 = size.y*vec3(0, 0, 1);
  } else {
    vec3 up(0, 1, 0);
    dir2 = size.x*normalize(cross(normal, up));
    dir1 = size.y*normalize(cross(normal, dir2));
  }
  auto g = newGeometry();
  
  g->pos.emplace_back(dir1 + dir2);
  g->pos.emplace_back(dir1 - dir2);
  g->pos.emplace_back(-dir1 - dir2);
  g->pos.emplace_back(-dir1 + dir2);
  g->norm.emplace_back(normal);
  g->norm.emplace_back(normal);
  g->norm.emplace_back(normal);
  g->norm.emplace_back(normal);
  g->tc.emplace_back(1,1);
  g->tc.emplace_back(1, 0);
  g->tc.emplace_back(0, 0);
  g->tc.emplace_back(0, 1);
  g->elements = { 2,3,0,2,0,1 };

  g->updateData();
  return g;
}
