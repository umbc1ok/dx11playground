struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 coords : TEXCOORD;
};

cbuffer cbPerObject
{
    float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

VS_OUTPUT VS(float4 inPos : POSITION, float2 texCoord : TEXCOORD)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);
    output.coords = texCoord;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return ObjTexture.Sample(ObjSamplerState, input.coords);
}