#include "base.hlsli"

float4 PS(VertOut pIn) : SV_Target
{
	return gDiffuseAlbedo * gAmbient;
	//return pIn.color * gDiffuseAlbedo;
}