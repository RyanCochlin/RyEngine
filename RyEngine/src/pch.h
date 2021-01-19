#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <memory>

//-------------------DirectX------------------//
#include <wrl.h>
#include <d3d12.h>
#include <DXGI1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "Platform/DirectX/d3dx12.h"

#define D3D12_GPU_VIRTUAL_ADDRESS_NULL      ((D3D12_GPU_VIRTUAL_ADDRESS)0)
#define D3D12_GPU_VIRTUAL_ADDRESS_UNKNOWN   ((D3D12_GPU_VIRTUAL_ADDRESS)-1)

#include "Core/Resource.h"

#include "Core/Log.h"
#include "Core/Error.h"
#include "Core/Assert.h"