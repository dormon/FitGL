#pragma once

#include "BaseApp.h"

class Callbacks: public BaseApp{
public:
	Callbacks();
protected:
	virtual void init();
	virtual void draw();      


	virtual void onResize(int  w , int  h );
	virtual void onMouseMove(int  dx , int  dy , int  x , int  y );
	virtual void onMousePress(Uint8  button , int  x , int  y );
	virtual void onMouseRelease(Uint8  button , int  x , int  y );
	virtual void onMouseWheel(int  delta );
	virtual void onKeyPress(SDL_Keycode  key , Uint16  mod );
	virtual void onKeyRelease(SDL_Keycode  key , Uint16  mod );

};