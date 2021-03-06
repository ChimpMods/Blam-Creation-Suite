#pragma once

namespace blofeld
{

namespace macaque
{

	constexpr unsigned long LIGHT_CONE_TAG = 'licn';
	extern s_tag_group light_cone_group; // light_cone_block


	extern s_tag_block_definition light_cone_block;

	extern s_tag_struct_definition light_cone_struct_definition; // tag group

	extern c_versioned_string_list lightConeFlags_strings;
	extern s_string_list_definition lightConeFlags;

} // namespace macaque

} // namespace blofeld

