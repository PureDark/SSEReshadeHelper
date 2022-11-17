#pragma once

#include <reshade.hpp>
#include <cmath>
#include <cstring>
#include <d3d11.h>
#include <PCH.h>

using namespace reshade::api;

static effect_runtime* m_runtime = nullptr;
static command_list*   m_cmdlist = nullptr;
static resource_view   m_rtv;
static resource_view   m_rtv_srgb;
static bool RenderBeforeUI = true;

void InstallUpscalerHooks();
