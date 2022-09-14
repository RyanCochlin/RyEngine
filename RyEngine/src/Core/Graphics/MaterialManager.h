#pragma once

#include <vector>
#include "Material.h"

namespace RE
{

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	Material* GetDefaultMaterial(const char* name);
	
private:
	std::vector<Material*> _mMatList;
};

}