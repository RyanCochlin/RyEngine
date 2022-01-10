#include "base.hlsli"

VertOut VS(VertIn vIn)
{
	VertOut vOut;

	float4 pos = float4(vIn.pos, 1);
	pos = mul(pos, gWorld);
	vOut.posH = mul(pos, gViewProj);
	vOut.color = vIn.color;

	return vOut;
}