#pragma once

#include<SDLWindow/EventDataInterface.h>

class SDLEventData : public EventDataInterface {
public:
	SDL_Event event;
};
