#include "DirectX.h"

namespace RyEngine
{
	void DirectX::Init()
	{
		_init = new DirectXInit();
		_init->Init();
	}

	void DirectX::Release()
	{

	}
}