#pragma once
#include <DirectXMath.h>
#include <dxgi.h>
struct Vertex    //Overloaded Vertex Structure
{
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz)
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}

	// research why the hell do we use these "pad1" and "pad2" variables
	DirectX::XMFLOAT3 dir;
	float pad1;
	DirectX::XMFLOAT3 pos;
	float range;
	DirectX::XMFLOAT3 att;
	float pad2;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};