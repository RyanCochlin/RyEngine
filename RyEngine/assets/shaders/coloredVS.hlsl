#include "base.hlsli"

VertOut VS(VertIn vIn)
{
	VertOut vOut;

	vOut.posH = mul(float4(vIn.pos, 1), gWorldViewProj);
	vOut.color = vIn.color;

	return vOut;
}