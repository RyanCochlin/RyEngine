#include "base.hlsli"

VertOut VS(VertIn vIn)
{
	VertOut vOut;

	vOut.posH = float4(vIn.pos, 1);
	vOut.color = vIn.color;

	return vOut;
}