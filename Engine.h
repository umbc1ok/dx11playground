#pragma once
#include <Windows.h>
#include <dxgitype.h>
#include <dxgi.h>
#include <WICTextureLoader.h>
#include "defines.h"
#include "Game.h"
class Engine
{
public: 

	Engine() {};
	~Engine() {};

	void createWindow(HINSTANCE hInstance,	//Main windows function
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nShowCmd);
	void Start();
	void Update();

	
	
private:
	bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, bool windowed);
	bool InitializeDirect3d11App(HINSTANCE hInstance);
	void CleanUp();
	int messageloop();


	void StartTimer();
	double GetTime();
	double GetFrameTime();

	Game game;

	HWND hwnd;
	HINSTANCE hInstance;
	HRESULT hr;
	IDXGISwapChain* SwapChain;
	ID3D11Device* d3d11Device;
	ID3D11DeviceContext* d3d11DevCon;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RasterizerState* rasterizerState;
	ID3D11Buffer* cbPerObjectBuffer;

	// time related variables
	double countsPerSecond = 0.0;
	__int64 CounterStart = 0;

	int frameCount = 0;
	int fps = 0;

	__int64 frameTimeOld = 0;
	double frameTime;

	// this should not be here, probably is also specific to one type of object I guess



};