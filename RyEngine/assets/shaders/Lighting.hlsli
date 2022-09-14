struct Material
{
	float4 DiffuseAlbedo;
	float3 FresnelR0;
	float Shininess;
};

struct Light
{
    float3 Strength;
    float FalloffStart;
    float3 Direction;
    float FalloffEnd;
    float3 Position;
    float SpotPower;
};

// Most of the lighting calculations taken from DX12 book for now
float4 CalculateLighting()
{

}