#include <windows.h>
#include "Engine.h"



int WINAPI WinMain(HINSTANCE hInstance,	//Main windows function
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{	
	Engine engine;
	engine.createWindow(hInstance,hPrevInstance,lpCmdLine,nShowCmd);
	engine.Start();
	engine.Update();


	return 0;
}










