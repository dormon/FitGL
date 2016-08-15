#pragma once

#include<SDLWindow/EventDataInterface.h>
#include <functional>

typedef std::function<bool(EventDataPointer)> EventHandlerInterface;
/*
class EventHandlerInterface {
public:
	virtual ~EventHandlerInterface() {}
	virtual bool operator()(EventDataPointer const&) = 0;
};*/
