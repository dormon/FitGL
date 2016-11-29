#pragma once

#include <memory>
#define classShared(x) class x; typedef std::shared_ptr<x> x##S; typedef std::weak_ptr<x> x##W;  template<typename...args> x##S new##x(args...a){return std::make_shared<x>(a...);};
#define classForward(x) class x; typedef std::shared_ptr<x> x##S; typedef std::weak_ptr<x> x##W;