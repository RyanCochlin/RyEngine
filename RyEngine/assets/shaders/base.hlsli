// TODO flip these once I get per object and per pass buffers working
cbuffer cbObj : register(b0)
{
	float4x4 gWorldViewProj;
}

cbuffer cObjBuf : register(b1)
{

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