#include "pch.h"
#include "MaterialManager.h"
#include "Core/Color.h"

namespace RE
{

MaterialManager::MaterialManager() : 
    _mMatList{}
{}

MaterialManager::~MaterialManager()
{
    for (auto mat : _mMatList)
    {
        delete mat;
    }
}

Material* RE::MaterialManager::GetDefaultMaterial(const char* name)
{
    Material* mat = new Material;
    mat->name = name;
    mat->materialData.diffuse = ColorToVector(RE_WHITE);
    mat->materialData.fresnel = { 0.5f, 0.5f, 0.5f };
    mat->materialData.rough = 0.2f;
    return mat;
}
}