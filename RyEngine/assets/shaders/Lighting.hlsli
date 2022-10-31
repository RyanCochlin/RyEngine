#ifndef __LIGHTING_HLSLI__
#define __LIGHTING_HLSLI__

static const uint gMaxLights = 4;

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

float3 Fresnel(float3 R0, float3 normal, float3 lightVec)
{
    float angle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - angle;
    float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

    return reflectPercent;
}

float3 BlinnPhong(float3 strength, float3 toLight, float3 normal, float3 eye, Material mat)
{
    float m = mat.Shininess * 256.0f;
    float3 halfVec = normalize(eye + toLight);

    float roughness = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
    float3 fresnel = Fresnel(mat.FresnelR0, halfVec, toLight);

    float3 specAlbedo = fresnel * roughness;
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (mat.DiffuseAlbedo.rgb + specAlbedo) * strength;
}

float3 ComputeDirectionalLight(Light light, Material mat, float3 normal, float3 eye)
{
    float3 toLight = -light.Direction;

    float nDotL = max(dot(toLight, normal), 0.0f);
    float3 strength = light.Strength * nDotL;

    return BlinnPhong(strength, toLight, normal, eye, mat);
}

// Most of the lighting calculations taken from DX12 book for now
float4 CalculateLighting(Light lights[gMaxLights], Material mat, float3 pos, float3 normal, float3 eye,
    int dirLightCount)
{
    int i = 0;
    float3 result = 0.0f;

    float3 toEye = pos - eye;

    for (i = 0; i < gMaxLights; i++)
    {
        if (i < dirLightCount)
            result += ComputeDirectionalLight(lights[i], mat, normal, toEye);
    }

    return float4(result, 0.0f);
}

#endif