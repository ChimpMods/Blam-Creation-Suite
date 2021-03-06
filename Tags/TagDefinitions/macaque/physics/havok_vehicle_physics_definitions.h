#pragma once

namespace blofeld
{

namespace macaque
{

	extern s_tag_block_definition anti_gravity_point_definition_block;
	extern s_tag_struct_definition anti_gravity_point_definition_block_struct_definition; // anti_gravity_point_definition_block
	extern s_tag_block_definition friction_point_definition_block;
	extern s_tag_struct_definition friction_point_definition_block_struct_definition; // friction_point_definition_block

	extern s_tag_struct_definition havok_vehicle_physics_struct;

	extern c_versioned_string_list anti_gravity_point_definition_flags_strings;
	extern s_string_list_definition anti_gravity_point_definition_flags;
	extern c_versioned_string_list anti_gravity_point_definition_damping_strings;
	extern s_string_list_definition anti_gravity_point_definition_damping;
	extern c_versioned_string_list friction_point_definition_flags_strings;
	extern s_string_list_definition friction_point_definition_flags;
	extern c_versioned_string_list havok_vehicle_physics_definition_flags_strings;
	extern s_string_list_definition havok_vehicle_physics_definition_flags;
	extern c_versioned_string_list physics_update_period_enum_strings;
	extern s_string_list_definition physics_update_period_enum;

} // namespace macaque

} // namespace blofeld

