#pragma once
#include <d3d11.h>
#include "ConstantBuffers.h"

class Game
{
public:
	Game() {};
	~Game() {};
	void DrawScene(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, ID3D11Buffer* cbPerObjectBuffer);

	bool InitScene(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, HRESULT &hr, ID3D11Buffer* cbPerObjectBuffer);
	void UpdateScene(double time);
	void CleanUp();

private:
	Light light; // this is temporarily, I guess it could be a list of lights in the future, we will see
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3DBlob* VS_blob;
	ID3DBlob* PS_blob;
	ID3D11InputLayout* vertLayout;

	ID3D11Buffer* squareIndexBuffer;
	ID3D11Buffer* squareVertBuffer;

	ID3D11ShaderResourceView* CubesTexture;
	ID3D11SamplerState* CubesTexSamplerState;

	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;

	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX camProjection;

	DirectX::XMVECTOR camPosition;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;


	ID3D11BlendState* Transparency;
	ID3D11RasterizerState* CCWcullMode;
	ID3D11RasterizerState* CWcullMode;

	cbPerObject cbPerObj;
	cbPerFrame constbuffPerFrame;

	ID3D11Buffer* cbPerFrameBuffer;




	// to transfer later
	DirectX::XMMATRIX cube1World;
	DirectX::XMMATRIX cube2World;

	DirectX::XMMATRIX Rotation;
	DirectX::XMMATRIX Scale;
	DirectX::XMMATRIX Translation;
	float rot = 0.01f;



};

