#include "base.hlsli"
#include "Lighting.hlsli"

float4 PS(VertOut pIn) : SV_Target
{
	pIn.normal = normalize(pIn.normal);
	
	float shininess = 1 - gRoughness;
	Material mat = { gDiffuseAlbedo, gFresnelR0, shininess };

	float4 lighting = CalculateLighting(gLights, mat, pIn.posH.xyz, pIn.normal, gEyePos, gDirLightCount);
	float4 ambient = gDiffuseAlbedo * gAmbient;

	float4 litColor = ambient + lighting;
	return litColor;
}