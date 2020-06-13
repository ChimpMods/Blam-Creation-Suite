﻿#include "halo3odstlib-private-pch.h"

/* ---------- private constants */
/* ---------- private macros */
/* ---------- private types */
/* ---------- private classes */
/* ---------- globals */

c_game_runtime* c_halo3odst_game_host::g_halo3odst_game_runtime;
static c_halo3odst_engine_state_command* g_halo3odst_engine_state_command;

/* ---------- private prototypes */
/* ---------- public code */

#include "halo3odst_game_host.testing.inl"

void register_halo3odstlib()
{

}

c_halo3odst_game_host::c_halo3odst_game_host(e_engine_type engine_type, e_build build) :
	c_opus_game_engine_host(engine_type, build, get_game_runtime())
{
	c_console::write_line_verbose("Init Halo3OdstGameHost");

	init_runtime_modifications(g_halo3odst_game_runtime->get_build());

	if (game_engine == nullptr)
	{
		__int64 create_game_engine_result = get_game_runtime().CreateGameEngine(&game_engine);
	}
	ASSERT(game_engine != nullptr);

	if (g_halo3odst_engine_state_command != nullptr)
	{
		g_halo3odst_engine_state_command->set_game_engine(game_engine);
	}
}

c_halo3odst_game_host::~c_halo3odst_game_host()
{
	c_console::write_line_verbose("Deinit Halo3OdstGameHost");

	c_mandrill_gui::set_get_tag_selection_address_function(nullptr); // #TODO: This is kinda hacky
	c_mandrill_gui::set_get_tag_pointer_function(nullptr); // #TODO: This is kinda hacky

	deinit_runtime_modifications(g_halo3odst_game_runtime->get_build());

	c_game_runtime& halo3odst_game_runtime = get_game_runtime();
	halo3odst_game_runtime.~c_game_runtime();
	new(&halo3odst_game_runtime) c_game_runtime(_engine_type_halo3odst, "halo3odst", "Halo3Odst\\halo3odst.dll");
}

void c_halo3odst_game_host::FrameEnd(IDXGISwapChain* swap_chain, _QWORD unknown1)
{
	if (GetAsyncKeyState(VK_F10))
	{
		get_game_engine()->UpdateEngineState(eEngineState::EndGame);
	}

	c_opus_game_engine_host::FrameEnd(swap_chain, unknown1);
}

void c_halo3odst_game_host::render_ui() const
{
}

IGameEngine* c_halo3odst_game_host::get_game_engine() const
{
	return game_engine;
}

c_game_runtime& c_halo3odst_game_host::get_game_runtime()
{
	if (g_halo3odst_game_runtime == nullptr)
	{
		g_halo3odst_game_runtime = new c_game_runtime(_engine_type_halo3odst, "halo3odst", "Halo3Odst\\halo3odst.dll");
	}

	return *g_halo3odst_game_runtime;
}

void c_halo3odst_game_host::init_runtime_modifications(e_build build)
{
	g_halo3odst_engine_state_command = new c_halo3odst_engine_state_command();

	init_detours();
	c_global_reference::init_global_reference_tree(_engine_type_halo3odst, build);
	c_data_patch_base::init_data_patch_tree(_engine_type_halo3odst, build);
	c_function_hook_base::init_function_hook_tree(_engine_type_halo3odst, build);
	end_detours();
}

void c_halo3odst_game_host::deinit_runtime_modifications(e_build build)
{
	delete g_halo3odst_engine_state_command;

	init_detours();
	c_function_hook_base::deinit_function_hook_tree(_engine_type_halo3odst, build);
	c_data_patch_base::deinit_data_patch_tree(_engine_type_halo3odst, build);
	c_global_reference::deinit_global_reference_tree(_engine_type_halo3odst, build);
	end_detours();
}

/* ---------- private code */