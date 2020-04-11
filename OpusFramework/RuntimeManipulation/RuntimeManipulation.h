#pragma once

#include "FindOffsetFunction.h"

// #TODO: (Squaresome) Refactor the s_data_reference_legacy API because it is crazy!

// these API's require auto in their templates. 
// copy and pasting this elsewhere in the code could result in your immediate murder.
#ifdef auto 
#define __BYPASS_AUTO_BAN__
#undef auto
#endif

#pragma optimize("", off) // MSVC optimization absolutely destroys function hooking

#include "FunctionHookBase.h"
#include "FunctionHook.h"
#include "FunctionHookEx.h"
#include "FunctionHookVarArgs.h"
#include "FunctionHookVarArgsEx.h"

#pragma optimize("", on)

#ifdef __BYPASS_AUTO_BAN__
#define auto auto_is_banned
#undef __BYPASS_AUTO_BAN__
#endif

/* LEGACY */ #include "opus_legacy/detour_helpers.h"
#include "SystemPatch.h"
#include "DataPatch.h"
