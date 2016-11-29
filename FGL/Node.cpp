#include "Node.h"
#include <algorithm>

using namespace fgl;
void Node::addNode(NodeS node){
  children.emplace_back(node);
}

void Node::removeNode(NodeS node){
  auto f = std::find(children.begin(), children.end(), node);
  if (f != children.end())
    children.erase(f);
}

void fgl::Node::addMesh(MeshS & m){
  meshes.emplace_back(m);
}

void fgl::Node::removeMesh(MeshS & m){
  auto f = std::find(meshes.begin(), meshes.end(), m);
  if (f != meshes.end())
    meshes.erase(f);
}
