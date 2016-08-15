#pragma once

#include<SDLWindow/EventDataInterface.h>

#include <functional>

//typedef std::function<void(EventDataPointer const&)> EventCallbackInterface;


class EventCallbackInterface {
public:
	virtual ~EventCallbackInterface() {}
	virtual bool operator()(EventDataPointer const&) = 0;
};
