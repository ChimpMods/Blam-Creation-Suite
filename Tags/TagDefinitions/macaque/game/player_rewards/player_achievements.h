#pragma once

namespace blofeld
{

namespace macaque
{

	constexpr unsigned long ACHIEVEMENTS_TAG = 'achi';
	extern s_tag_group achievements_group; // achievements_block


	extern s_tag_block_definition achievements_block;
	extern s_tag_block_definition single_achievement_definition_block;
	extern s_tag_struct_definition single_achievement_definition_block_struct_definition; // single_achievement_definition_block
	extern s_tag_block_definition single_achievement_restricted_level_block;
	extern s_tag_struct_definition single_achievement_restricted_level_block_struct_definition; // single_achievement_restricted_level_block

	extern s_tag_struct_definition achievements_struct_definition; // tag group

	extern c_versioned_string_list global_achievement_enum_strings;
	extern s_string_list_definition global_achievement_enum;

} // namespace macaque

} // namespace blofeld

