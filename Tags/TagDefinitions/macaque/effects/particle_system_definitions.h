#pragma once

namespace blofeld
{

namespace macaque
{

	extern s_tag_block_definition particle_system_definition_block_new_block;
	extern s_tag_struct_definition particle_system_definition_block_new_block_struct_definition; // particle_system_definition_block_new_block
	extern s_tag_block_definition particle_system_emitter_definition_block;
	extern s_tag_struct_definition particle_system_emitter_definition_block_struct_definition; // particle_system_emitter_definition_block
	extern s_tag_block_definition emitterGlobalForceBlock_block;
	extern s_tag_struct_definition emitterGlobalForceBlock_block_struct_definition; // emitterGlobalForceBlock_block
	extern s_tag_block_definition emitterClipSphereBlock_block;
	extern s_tag_struct_definition emitterClipSphereBlock_block_struct_definition; // emitterClipSphereBlock_block
	extern s_tag_block_definition gpu_property_block;
	extern s_tag_struct_definition gpu_property_block_struct_definition; // gpu_property_block
	extern s_tag_block_definition gpu_function_block;
	extern s_tag_struct_definition gpu_function_block_struct_definition; // gpu_function_block
	extern s_tag_block_definition gpu_color_block;
	extern s_tag_struct_definition gpu_color_block_struct_definition; // gpu_color_block

	extern s_tag_struct_definition gpu_property_sub_array_struct_definition;
	extern s_tag_array_definition gpu_property_sub_array;
	extern s_tag_struct_definition gpu_function_sub_array_struct_definition;
	extern s_tag_array_definition gpu_function_sub_array;
	extern s_tag_struct_definition gpu_color_sub_array_struct_definition;
	extern s_tag_array_definition gpu_color_sub_array;
	extern s_tag_struct_definition gpu_property_function_color_struct;

	extern c_versioned_string_list coordinate_system_enum_strings;
	extern s_string_list_definition coordinate_system_enum;
	extern c_versioned_string_list particle_system_flags_strings;
	extern s_string_list_definition particle_system_flags;
	extern c_versioned_string_list emission_shape_enum_strings;
	extern s_string_list_definition emission_shape_enum;
	extern c_versioned_string_list emission_axis_enum_strings;
	extern s_string_list_definition emission_axis_enum;
	extern c_versioned_string_list emission_reference_axis_enum_strings;
	extern s_string_list_definition emission_reference_axis_enum;
	extern c_versioned_string_list emitter_flags_strings;
	extern s_string_list_definition emitter_flags;
	extern c_versioned_string_list visible_emitter_flags_strings;
	extern s_string_list_definition visible_emitter_flags;

} // namespace macaque

} // namespace blofeld
