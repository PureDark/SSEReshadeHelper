#include <ReShadePlugin.h>


struct ReshadeHooks
{
	struct Main_DrawWorld_MainDraw
	{
		static void thunk(INT64 BSGraphics_Renderer, int unk)
		{
			static int index = 0;
			func(BSGraphics_Renderer, unk);
			if (m_runtime != nullptr && RenderBeforeUI) {
				m_runtime->render_effects(m_cmdlist, m_rtv, m_rtv_srgb);
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;
	};

	static void Install()
	{
		// Pre-UI Hook
		stl::write_thunk_call<Main_DrawWorld_MainDraw>(REL::RelocationID(79947, 82084).address() + REL::Relocate(0x16F, 0x17A));  // EBF510 (EBF67F), F05BF0 (F05D6A)
	}
};

void InstallUpscalerHooks()
{
	ReshadeHooks::Install();
}

static void on_reshade_begin_effects(effect_runtime *runtime, command_list *cmd_list, resource_view rtv, resource_view rtv_srgb)
{
	m_runtime = runtime;
	m_cmdlist = cmd_list;
	m_rtv = rtv;
	m_rtv_srgb = rtv_srgb;
}

void register_addon_events()
{
	reshade::register_event<reshade::addon_event::reshade_begin_effects>(on_reshade_begin_effects);
}

void unregister_addon_events()
{
	reshade::unregister_event<reshade::addon_event::reshade_begin_effects>(on_reshade_begin_effects);
}

extern "C" __declspec(dllexport) const char* NAME = "SSE ReShade Helper";
extern "C" __declspec(dllexport) const char* DESCRIPTION = "Make ReShade renders before UI";

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		if (!reshade::register_addon(hModule))
			return FALSE;
		register_addon_events();
		break;
	case DLL_PROCESS_DETACH:
		unregister_addon_events();
		reshade::unregister_addon(hModule);
		break;
	}

	return TRUE;
}
