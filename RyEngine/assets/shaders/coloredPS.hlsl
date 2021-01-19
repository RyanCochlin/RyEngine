#include "base.hlsli"

float4 PS(VertOut pIn) : SV_Target
{
	return pIn.color;
}