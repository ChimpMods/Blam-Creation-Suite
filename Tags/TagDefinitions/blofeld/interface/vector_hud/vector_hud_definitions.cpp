#include <tagdefinitions-private-pch.h>
#include <blofeld_field_type_override.h>

namespace blofeld
{

	V5_TAG_GROUP_FROM_BLOCK(vector_hud_definition, VECTOR_HUD_DEFINITION_TAG, vector_hud_definition_block_block );

	V5_TAG_BLOCK_FROM_STRUCT(vector_hud_definition_block, 1, vector_hud_definition_struct_definition_struct_definition );

	V5_TAG_STRUCT(vector_hud_definition_struct_definition)
	{
		{ _field_legacy, _field_data, "raw hud data" },
		{ _field_legacy, _field_terminator }
	};

} // namespace blofeld

