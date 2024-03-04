#include "Game.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "DataStructures.h"
#include "defines.h"
#include <WICTextureLoader.h>


void Game::DrawScene(ID3D11Device* d3d11Device,
	ID3D11DeviceContext* d3d11DevCon,
	IDXGISwapChain* SwapChain,
	ID3D11RenderTargetView* renderTargetView,
	ID3D11DepthStencilView* depthStencilView,
	ID3D11Buffer* cbPerObjectBuffer
	)
{
	//Clear our backbuffer
	float bgColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };

	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	///////////////**************new**************////////////////////
	//Refresh the Depth/Stencil view
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	///////////////**************new**************////////////////////


	constbuffPerFrame.light = light;
	d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);


	//"fine-tune" the blending equation
	float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };
	//Set the default blend state (no blending) for opaque objects
	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);


	// RENDER OPAQUE OBJECTS FIRST
	// DO IT HERE HERE HERE HERE HERE HERE HERE HERE
	// UPDATE WVP MATRIX and send it to constant buffer
	WVP = cube1World * camView * camProjection;
	cbPerObj.World = XMMatrixTranspose(cube1World);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &CubesTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	d3d11DevCon->RSSetState(CWcullMode);
	d3d11DevCon->DrawIndexed(36, 0, 0);


	// UPDATE WVP MATRIX and send it to constant buffer
	WVP = cube2World * camView * camProjection;
	cbPerObj.World = XMMatrixTranspose(cube2World);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &CubesTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
	//Draw the second cube
	d3d11DevCon->RSSetState(CWcullMode);
	d3d11DevCon->DrawIndexed(36, 0, 0);



	//

	d3d11DevCon->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	DirectX::XMVECTOR cubePos = DirectX::XMVectorZero();
	cubePos = XMVector3TransformCoord(cubePos, cube1World);

	float distX = DirectX::XMVectorGetX(cubePos) - DirectX::XMVectorGetX(camPosition);
	float distY = DirectX::XMVectorGetY(cubePos) - DirectX::XMVectorGetY(camPosition);
	float distZ = DirectX::XMVectorGetZ(cubePos) - DirectX::XMVectorGetZ(camPosition);

	float cube1Dist = distX * distX + distY * distY + distZ * distZ;

	cubePos = DirectX::XMVectorZero();

	cubePos = XMVector3TransformCoord(cubePos, cube2World);

	distX = DirectX::XMVectorGetX(cubePos) - DirectX::XMVectorGetX(camPosition);
	distY = DirectX::XMVectorGetY(cubePos) - DirectX::XMVectorGetY(camPosition);
	distZ = DirectX::XMVectorGetZ(cubePos) - DirectX::XMVectorGetZ(camPosition);

	float cube2Dist = distX * distX + distY * distY + distZ * distZ;

	//If the first cubes distance is less than the second cubes
	if (cube1Dist < cube2Dist)
	{
		//Switch the order in which the cubes are drawn
		DirectX::XMMATRIX tempMatrix = cube1World;
		cube1World = cube2World;
		cube2World = tempMatrix;
	}

	/*
	// UPDATE WVP MATRIX and send it to constant buffer
	WVP = cube1World * camView * camProjection;
	cbPerObj.World = XMMatrixTranspose(cube1World);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &CubesTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);


	d3d11DevCon->RSSetState(CCWcullMode);
	d3d11DevCon->DrawIndexed(36, 0, 0);

	d3d11DevCon->RSSetState(CWcullMode);
	d3d11DevCon->DrawIndexed(36, 0, 0);


	// UPDATE WVP MATRIX and send it to constant buffer
	WVP = cube2World * camView * camProjection;
	cbPerObj.World = XMMatrixTranspose(cube2World);
	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &CubesTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
	//Draw the second cube
	d3d11DevCon->RSSetState(CCWcullMode);
	d3d11DevCon->DrawIndexed(36, 0, 0);

	d3d11DevCon->RSSetState(CWcullMode);
	d3d11DevCon->DrawIndexed(36, 0, 0);
	//Present the backbuffer to the screen
	*/
	SwapChain->Present(0, 0);
}
void Game::UpdateScene(double time)
{
	//Keep the cubes rotating
	rot += 1.0f * time;
	if (rot > 6.28f)
		rot = 0.0f;

	//Reset cube1World
	cube1World = DirectX::XMMatrixIdentity();

	//Define cube1's world space matrix
	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = DirectX::XMMatrixRotationAxis(rotaxis, rot);
	Translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	//Set cube1's world space using the transformations
	cube1World = Translation * Rotation;

	//Reset cube2World
	cube2World = DirectX::XMMatrixIdentity();

	//Define cube2's world space matrix
	Rotation = DirectX::XMMatrixRotationAxis(rotaxis, -rot);
	Scale = DirectX::XMMatrixScaling(1.3f, 1.3f, 1.3f);

	//Set cube2's world space matrix
	cube2World = Rotation * Scale;
}

bool Game::InitScene(ID3D11Device* d3d11Device, ID3D11DeviceContext* d3d11DevCon, HRESULT &hr, ID3D11Buffer* cbPerObjectBuffer)
{
	hr = D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &VS_blob, nullptr);	// might be wrong, check with https://learn.microsoft.com/en-us/windows/win32/api/d3dcompiler/nf-d3dcompiler-d3dcompilefromfile
	hr = D3DCompileFromFile(L"shaders.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &PS_blob, nullptr);

	//Create the Shader Objects
	hr = d3d11Device->CreateVertexShader(VS_blob->GetBufferPointer(), VS_blob->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreatePixelShader(PS_blob->GetBufferPointer(), PS_blob->GetBufferSize(), NULL, &PS);

	//Set Vertex and Pixel Shaders
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	

	light.dir = DirectX::XMFLOAT3(0.25f, 0.5f, -1.0f);
	light.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//Create the vertex buffer
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		// Back Face
		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		// Top Face
		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		// Bottom Face
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		// Left Face
		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		// Right Face
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;



	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);

	d3d11DevCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 24;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);

	//Set the vertex buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);


	///////////////////**************WARNING**************////////////////////
	//////// MOVE IT OUT OF THERE, WHEN YOU KNOW WHERE TO PUT IT//////////////
	///////////////////**************WARNING**************////////////////////
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = ARRAYSIZE(layout);


	//Create the Input Layout
	hr = d3d11Device->CreateInputLayout(layout, numElements, VS_blob->GetBufferPointer(),
		VS_blob->GetBufferSize(), &vertLayout);

	//Set the Input Layout
	d3d11DevCon->IASetInputLayout(vertLayout);

	//Set Primitive Topology
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = WIDTH;
	viewport.Height = HEIGHT;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	d3d11DevCon->RSSetViewports(1, &viewport);

	// Setup camera
	camPosition = DirectX::XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
	camProjection = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)WIDTH / HEIGHT, 1.0f, 1000.0f);

	World = DirectX::XMMatrixIdentity();
	WVP = World * camView * camProjection;

	cbPerObj.WVP = DirectX::XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

	// D3DX11CreateShaderResourceViewFromFile is deprecated, use DirectX::CreateWICTextureFromFile instead
	hr = DirectX::CreateWICTextureFromFile(d3d11Device, L"image2.png",
		nullptr, &CubesTexture);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);


	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	d3d11Device->CreateBlendState(&blendDesc, &Transparency);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);


	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);

	return true;
}

void Game::CleanUp()
{
	// auto generated, not sure if ok
	VS_blob->Release();
	PS_blob->Release();
	VS->Release();
	PS->Release();
	vertLayout->Release();
	squareIndexBuffer->Release();
	squareVertBuffer->Release();
	CubesTexture->Release();
	CubesTexSamplerState->Release();
	Transparency->Release();
	CCWcullMode->Release();
	CWcullMode->Release();
	cbPerFrameBuffer->Release();
}
