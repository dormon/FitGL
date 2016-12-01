#include "VuNode.h"
#include <algorithm>

using namespace NeiVu;

void Node::removeNode(NodeShared node) {
	auto f = std::find(children.begin(), children.end(), node);
	if (f != children.end())
		children.erase(f);
}
