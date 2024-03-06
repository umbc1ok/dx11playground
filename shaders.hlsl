struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 worldPos : POSITION;
    float2 coords : TEXCOORD;
    float3 normal : NORMAL;
};

cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
};


struct Light
{
    float3 dir;
    float3 pos;
    float range;
    float3 att;
    float4 ambient;
    float4 diffuse;
};

cbuffer cbPerFrame
{
    Light light;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

VS_OUTPUT VS(float4 inPos : POSITION, float2 texCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);
    
    output.worldPos = mul(inPos, World);
    output.normal = mul(normal, World);
    output.coords = texCoord;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.coords);
    
    
    float3 finalColor = float3(0, 0, 0);
    float3 lightToPixel = light.pos - input.worldPos;
    
    float d = length(lightToPixel);
    
    float3 finalAmbient = diffuse * light.ambient;
    if(d > light.range)
    {
        return float4(finalAmbient, diffuse.a);
    }
    lightToPixel /= d;
    
    float howMuchLight = dot(lightToPixel, input.normal);
    
    if(howMuchLight > 0.0f)
    {
        finalColor += howMuchLight * light.diffuse * diffuse;
        finalColor /= light.att[0] + light.att[1] * d + light.att[2] * d * d;
    }
    finalColor = saturate(finalColor + finalAmbient);

    return float4(finalColor, diffuse.a);
}