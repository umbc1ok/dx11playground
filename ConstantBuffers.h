#pragma once
#include <DirectXMath.h>
#include "DataStructures.h"
struct cbPerObject
{
	DirectX::XMMATRIX WVP; // wvp = world view projection
	DirectX::XMMATRIX World;
};

struct cbPerFrame
{
	Light light;
};