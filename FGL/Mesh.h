#pragma once
#include "Shared.h"
namespace fgl {
  classShared(Mesh);
};

#include <geGLshared.h>

#include <Material.h>
#include <Geometry.h>
#include <vector>
#include <tuple>
#include <functional>

class fgl::Mesh: ge::gl::Context{
public:
  Mesh() {}
  virtual ~Mesh() {}

  void addDrawable(fgl::GeometryS geo, fgl::MaterialS mat);
  void removeDrawable(fgl::GeometryS geo, fgl::MaterialS mat);
  void forEach(std::function<void(fgl::GeometryS geo, fgl::MaterialS mat)> const&callback);

  MaterialS getMaterial(int i) { return std::get<1>(drawables.at(i)); }
  GeometryS getGeometry(int i) { return std::get<0>(drawables.at(i)); }
protected:
	std::vector<std::tuple<fgl::GeometryS, fgl::MaterialS>> drawables;
};