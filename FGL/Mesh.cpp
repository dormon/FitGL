#include "Mesh.h"
#include <algorithm>

using namespace fgl;

void Mesh::addDrawable(fgl::GeometryS geo, fgl::MaterialS mat) {
  drawables.emplace_back(std::make_tuple(geo, mat));
}

void Mesh::removeDrawable(fgl::GeometryS geo, fgl::MaterialS mat) {
  drawables.erase(
    std::remove(drawables.begin(), drawables.end(), std::make_tuple(geo, mat)),
    drawables.end());
}

void Mesh::forEach(std::function<void(fgl::GeometryS geo, fgl::MaterialS mat)> const & callback) {
  for (auto &dr : drawables) {
    callback(std::get<0>(dr), std::get<1>(dr));
  }
}
