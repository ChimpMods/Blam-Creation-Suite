#include <tagdefinitions-private-pch.h>
#include <macaque_field_type_override.h>

namespace blofeld
{

namespace macaque
{

	TAG_GROUP(
		camo_group,
		CAMO_TAG,
		nullptr,
		INVALID_TAG,
		camo_block );

	TAG_BLOCK_FROM_STRUCT(
		camo_block,
		"camo_block",
		1,
		camo_struct_definition);

	#define CAMO_STRUCT_DEFINITION_ID { 0xFD484E57, 0xEAA0419E, 0x95BA9341, 0xED21A3D5 }
	TAG_STRUCT(
		camo_struct_definition,
		"camo_struct_definition",
		"s_camo_effect_definition",
		SET_UNKNOWN0 | SET_UNKNOWN1 | SET_HAS_INLINED_CHILDREN_WITH_PLACEMENT_NEW | SET_UNKNOWN5 | SET_DELETE_RECURSIVELY | 
		SET_POSTPROCESS_RECURSIVELY | SET_HAS_LEVEL_SPECIFIC_FIELDS,
		TAG_MEMORY_ATTRIBUTES(MEMORY_ALLOCATION_NODE, TAG_MEMORY_USAGE_READ_ONLY),
		CAMO_STRUCT_DEFINITION_ID)
	{
		{ _field_explanation, "Active Camo Controls", "" },
		{ _field_word_flags, "Flags", &camo_flags },
		FIELD_PAD("KKHKKHKKL", nullptr, 2),
		{ _field_struct, "Active Camo Amount", &camo_scalar_function_struct },
		{ _field_struct, "Shadow Amount", &camo_scalar_function_struct },
		{ _field_terminator }
	};

	#define CAMO_SCALAR_FUNCTION_STRUCT_ID { 0xD6E8140F, 0xB16B4278, 0x9B69A726, 0x95B494AC }
	TAG_STRUCT(
		camo_scalar_function_struct,
		"camo_scalar_function_struct",
		"s_camo_function",
		SET_UNKNOWN0 | SET_UNKNOWN1 | SET_HAS_INLINED_CHILDREN_WITH_PLACEMENT_NEW | SET_UNKNOWN5 | SET_DELETE_RECURSIVELY | 
		SET_POSTPROCESS_RECURSIVELY | SET_HAS_LEVEL_SPECIFIC_FIELDS,
		TAG_MEMORY_ATTRIBUTES(MEMORY_ALLOCATION_NODE, TAG_MEMORY_USAGE_READ_ONLY),
		CAMO_SCALAR_FUNCTION_STRUCT_ID)
	{
		{ _field_string_id, "Input Variable", _field_id_function_input_scalar },
		{ _field_string_id, "Range Variable", _field_id_function_input_range },
		FIELD_CUSTOM(nullptr, nullptr, _field_id_default),
		{ _field_struct, "Mapping", &mapping_function },
		{ _field_terminator }
	};

	STRINGS(camo_flags)
	{
		"also apply to object children"
	};
	STRING_LIST(camo_flags, camo_flags_strings, _countof(camo_flags_strings));

} // namespace macaque

} // namespace blofeld

