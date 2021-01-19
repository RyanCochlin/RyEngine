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