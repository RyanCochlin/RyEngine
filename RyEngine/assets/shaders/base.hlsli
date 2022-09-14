// TODO flip these once I get per object and per pass buffers working
cbuffer cbObj : register(b0)
{
	float4x4 gViewProj;
	float4 gAmbient;
}

cbuffer cObjBuf : register(b1)
{
	float4x4 gWorld;
}

cbuffer cMatBuf : register(b2)
{
	float4 gDiffuseAlbedo;
	float3 gFresnelR0;
	float  gRoughness;
}

struct VertIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VertOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};