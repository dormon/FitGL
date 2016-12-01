#pragma once

#include <memory>
// ClassS = std::shared_ptr<Class>
// ClassW = std::weak_ptr<Class>
// newClass(...) = std::make_shared<Class>(...)
#define classShared(x) class x; typedef std::shared_ptr<x> x##S; typedef std::weak_ptr<x> x##W;  template<typename...args> x##S new##x(args...a){return std::make_shared<x>(a...);};
#define classForward(x) class x; typedef std::shared_ptr<x> x##S; typedef std::weak_ptr<x> x##W;