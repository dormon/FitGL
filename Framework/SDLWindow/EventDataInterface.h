#pragma once
class EventDataInterface {
public:
	virtual ~EventDataInterface() {}
};
using EventDataPointer = EventDataInterface const*;
