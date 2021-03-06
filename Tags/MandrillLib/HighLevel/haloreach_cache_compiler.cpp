#include "mandrilllib-private-pch.h"

#define align_value(value, alignment) (((value) + ((alignment) - static_cast<std::remove_reference<decltype(value)>::type>(1))) & ~((alignment) - static_cast<std::remove_reference<decltype(value)>::type>(1)))

#define DEBUG_PADDING 128

using namespace cache_compiler;


h_cache_file_tag_interop_vtable d3d_vertex_buffer_interop_vtable =
{
	"s_tag_d3d_vertex_buffer",
	0x20,
	3
};

h_cache_file_tag_interop_vtable d3d_index_buffer_interop_vtable =
{
	"s_tag_d3d_index_buffer",
	0x10,
	3
};

h_cache_file_tag_interop_vtable d3d_texture_interop_vtable =
{
	"s_tag_d3d_texture",
	0x10,
	3
};

h_cache_file_tag_interop_vtable d3d_texture_interleaved_interop_vtable =
{
	"s_tag_d3d_texture_interleaved",
	0x8,
	3
};

h_cache_file_tag_interop_vtable d3d_vertex_shader_interop_vtable =
{
	"s_tag_d3d_vertex_shader",
	0x8,
	3
};

h_cache_file_tag_interop_vtable d3d_pixel_shader_interop_vtable =
{
	"s_tag_d3d_pixel_shader",
	0x8,
	3
};

h_cache_file_tag_interop_vtable constant_buffer_interop_vtable =
{
	"s_constant_buffer",
	0x8,
	3
};

h_cache_file_tag_interop_vtable structured_buffer_interop_vtable =
{
	"s_structured_buffer",
	0x18,
	3
};

h_cache_file_tag_interop_vtable* cache_file_tag_interop_vtable[k_num_interop_types] =
{
	&d3d_vertex_buffer_interop_vtable,
	&d3d_index_buffer_interop_vtable,
	&d3d_texture_interop_vtable,
	&d3d_texture_interleaved_interop_vtable,
	&d3d_vertex_shader_interop_vtable,
	&d3d_pixel_shader_interop_vtable,
	&constant_buffer_interop_vtable,
	&structured_buffer_interop_vtable,
};

c_haloreach_cache_compiler::c_haloreach_cache_compiler(c_tag_project& tag_project DEBUG_ONLY(, c_haloreach_cache_file* _cache_file)) :
	DEBUG_ONLY(cache_file(_cache_file), )
	tag_project(tag_project),
	string_id_manager(*new c_string_id_manager(17, 8, 7)),
	cache_file_metadata(*new s_cache_file_metadata()),
	section_data(),
	virtual_base_address(),
	tags_section_begin_file_offset(),
	tags_section_tag_data_offset(),
	tags_section_tags_header_offset(),
	tags_section_tag_groups_offset(),
	tags_section_tag_instances_offset(),
	tags_section_tag_global_instances_offset(),
	tags_section_api_interop_fixups_offset(),
	tags_section_api_interop_data_offset(),
	tags_section_structured_buffer_interops_offset(),
	debug_section_file_table_offset(),
	debug_section_file_table_indices_offset(),
	debug_section_string_ids_offset(),
	debug_section_string_id_indices_offset(),
	debug_section_string_id_namespaces_offset(),
	tag_group_count(),
	tag_groups_buffer_size(),
	tag_groups_buffer(),
	tag_groups(),
	tag_data_buffer(),
	tag_data_data_size(),
	tag_data_buffer_size(),
	tag_data_entries(),
	tag_data_entry_count(),
	tag_instances_buffer(),
	tag_instances_data_size(),
	tag_instances_buffer_size(),
	tag_instance_count(),
	tag_global_entries_buffer(),
	tag_global_entries_data_size(),
	tag_global_entries_buffer_size(),
	tag_global_entries_count(),
	effect_structured_buffer(),
	beam_structured_buffer(),
	contrail_structured_buffer(),
	light_volume_structured_buffer(),
	interop_containers(),
	tag_api_interops_buffer(),
	tag_api_interops_data_size(),
	tag_api_interops_buffer_size(),
	tag_api_interop_fixups_buffer(),
	tag_api_interop_fixups_data_size(),
	tag_api_interop_fixups_buffer_size(),
	tag_api_interops_count(),
	tag_file_table_buffer(),
	tag_file_table_buffer_size(),
	tag_file_table_indices_buffer(),
	tag_file_table_indices_buffer_size(),
	tag_file_table_indices_count(),
	resources_buffer(),
	resources_data_size(),
	resources_buffer_size(),
	localization_buffer(),
	localization_data_size(),
	localization_buffer_size(),
	string_ids_count(),
	string_ids_indices_buffer(),
	string_ids_indices_data_size(),
	string_ids_indices_buffer_size(),
	string_ids_buffer(),
	string_ids_data_size(),
	string_ids_buffer_size(),
	string_id_namespace_count(),
	string_id_namespaces_buffer(),
	string_id_namespaces_data_size(),
	string_id_namespaces_buffer_size()
{
	string_id_manager.init_haloreach();
}

c_haloreach_cache_compiler::~c_haloreach_cache_compiler()
{
	delete& cache_file_metadata;
	delete& string_id_manager;
}

void write(const void* _buffer, size_t size, FILE* stream)
{
	const char* buffer = static_cast<const char*>(_buffer);
	size_t bytes_written = 0;
	while (bytes_written < size)
	{
		bytes_written += fwrite(buffer + bytes_written, 1, size - bytes_written, stream);
	}
}

uint32_t c_haloreach_cache_compiler::calculate_size(h_tag& tag)
{
	uint32_t tag_size = 0;
#if DEBUG_PADDING > 0
	tag_size = align_value(tag_size, tag_memory_alignment); // debug alignment
	tag_size += DEBUG_PADDING;
#endif

	tag_size = align_value(tag_size, tag_memory_alignment); // base struct alignment
	tag_size += tag.get_low_level_type_size();
	tag_size = align_value(tag_size, block_memory_alignment); // block memory alignment
	calculate_object_memory_footprint(tag_size, tag);
	tag_size = align_value(tag_size, tag_memory_alignment); // tag alignment

#if DEBUG_PADDING > 0
	tag_size = align_value(tag_size, tag_memory_alignment); // debug alignment
	tag_size += DEBUG_PADDING;
#endif

	return tag_size;
}

void c_haloreach_cache_compiler::calculate_object_memory_footprint(uint32_t& memory_footprint, h_object& object)
{
	for (const blofeld::s_tag_field* const* field_pointer = object.get_blofeld_field_list(); *field_pointer != nullptr; field_pointer++)
	{
		const blofeld::s_tag_field& field = **field_pointer;

		switch (field.field_type)
		{
		case blofeld::_field_block:
		{
			h_block& block_storage = *static_cast<h_block*>(object.get_field_data(field));
			if (block_storage.size() > 0)
			{
				uint32_t type_size = block_storage.get_low_level_type_size();
				uint32_t block_data_size = type_size * block_storage.size();

				memory_footprint = align_value(memory_footprint, block_memory_alignment);
				memory_footprint += align_value(block_data_size, block_memory_alignment);

				for (uint32_t object_index = 0; object_index < block_storage.size(); object_index++)
				{
					h_object& object = block_storage.get(object_index);
					calculate_object_memory_footprint(memory_footprint, object);
				}

				debug_point;
			}
			break;
		}
		case blofeld::_field_data:
		{
			h_data& data_storage = *static_cast<h_data*>(object.get_field_data(field));
			if (data_storage.size() > 0)
			{
				uint32_t data_size = static_cast<uint32_t>(data_storage.size());

				memory_footprint = align_value(memory_footprint, block_memory_alignment);
				memory_footprint += align_value(data_size, block_memory_alignment);

				debug_point;
			}
			break;
		}
		case blofeld::_field_array:
		{
			h_enumerable& child_array = *static_cast<h_enumerable*>(object.get_field_data(field));

			for (uint32_t object_index = 0; object_index < child_array.size(); object_index++)
			{
				h_object& object = child_array.get(object_index);
				calculate_object_memory_footprint(memory_footprint, object);
			}

			debug_point;
			break;
		}
		case blofeld::_field_struct:
		{
			h_object& child_object = *static_cast<h_object*>(object.get_field_data(field));

			calculate_object_memory_footprint(memory_footprint, child_object);

			debug_point;
			break;
		}
		}
	}
}

uint16_t c_haloreach_cache_compiler::get_tag_index(const h_tag* tag) const
{
	if (tag == nullptr)
	{
		return ~0ui16;
	}

	for (uint32_t tag_index = 0; tag_index < tag_data_entry_count; tag_index++)
	{
		s_tag_data_entry& tag_data_entry = tag_data_entries[tag_index];

		if (&tag_data_entry.tag == tag)
		{
			return static_cast<uint16_t>(tag_data_entry.tag_file_table_index);
		}

		debug_point;
	}
	throw;
}

void c_haloreach_cache_compiler::compile_tag(const h_tag& tag, char* tag_data, uint32_t tag_data_size)
{
	char* tag_allocation_postion = tag_data;

#if DEBUG_PADDING > 0
	tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), tag_memory_alignment)); // debug alignment
	c_fixed_string<DEBUG_PADDING>& header = *reinterpret_cast<c_fixed_string<DEBUG_PADDING>*>(tag_allocation_postion);
	tag_allocation_postion += sizeof(header);
	header.format(">>==BEGIN TAG [%s]==", tag.tag_filename.c_str());
	memset(header.data + header.size(), '>', sizeof(header) - header.size());
#endif

	tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), tag_memory_alignment)); // base struct alignment
	char* allocated_tag_data = tag_allocation_postion;
	tag_allocation_postion += tag.get_low_level_type_size();
	tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), block_memory_alignment)); // block memory alignment
	compile_object(tag, tag, allocated_tag_data, tag_allocation_postion);
	tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), tag_memory_alignment)); // tag alignment

#if DEBUG_PADDING > 0
	tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), tag_memory_alignment)); // debug alignment
	c_fixed_string<DEBUG_PADDING>& footer = *reinterpret_cast<c_fixed_string<DEBUG_PADDING>*>(tag_allocation_postion);
	tag_allocation_postion += sizeof(footer);
	footer.format("<<==END TAG [%s]==", tag.tag_filename.c_str());
	memset(footer.data + footer.size(), '<', sizeof(footer) - footer.size());
#endif

	intptr_t used_data_size = tag_allocation_postion - tag_data;
	DEBUG_ASSERT(used_data_size == tag_data_size);
}

void c_haloreach_cache_compiler::compile_object(const h_tag& tag, const h_object& object, char* object_data, char*& tag_allocation_postion)
{
	using namespace blofeld;

	uint32_t data_written = 0;
	char* current_data_position = object_data;
	for (const blofeld::s_tag_field* const* field_pointer = object.get_blofeld_field_list(); *field_pointer != nullptr; field_pointer++)
	{
		const blofeld::s_tag_field& field = **field_pointer;

		const void* high_level_field_data = object.get_field_data(field);

		uint32_t field_size = get_blofeld_field_size(field, tag_project.engine_type, tag_project.platform_type, tag_project.build);

		if (high_level_field_data != nullptr)
		{
			switch (field.field_type)
			{
			case _field_string:
			case _field_long_string:
			case _field_char_integer:
			case _field_short_integer:
			case _field_long_integer:
			case _field_int64_integer:
			case _field_angle:
			case _field_point_2d:
			case _field_rectangle_2d:
			case _field_rgb_color:
			case _field_argb_color:
			case _field_real:
			case _field_real_fraction:
			case _field_real_point_2d:
			case _field_real_point_3d:
			case _field_real_vector_2d:
			case _field_real_vector_3d:
			case _field_real_quaternion:
			case _field_real_euler_angles_2d:
			case _field_real_euler_angles_3d:
			case _field_real_plane_2d:
			case _field_real_plane_3d:
			case _field_real_rgb_color:
			case _field_real_argb_color:
			case _field_real_hsv_color:
			case _field_real_ahsv_color:
			case _field_short_bounds:
			case _field_angle_bounds:
			case _field_real_bounds:
			case _field_real_fraction_bounds:
			case _field_tag:
			case _field_long_block_flags:
			case _field_word_block_flags:
			case _field_byte_block_flags:
			case _field_char_block_index:
			case _field_custom_char_block_index:
			case _field_short_block_index:
			case _field_custom_short_block_index:
			case _field_long_block_index:
			case _field_custom_long_block_index:
			case _field_byte_integer:
			case _field_word_integer:
			case _field_dword_integer:
			case _field_qword_integer:
			case _field_pointer:
			case _field_half:
			{
				memcpy(current_data_position, high_level_field_data, field_size);
				break;
			}
			case _field_char_enum:
			{
				char data = static_cast<char>(*reinterpret_cast<const long*>(high_level_field_data));
				memcpy(current_data_position, &data, sizeof(data));
				break;
			}
			case _field_enum:
			{
				short data = static_cast<short>(*reinterpret_cast<const long*>(high_level_field_data));
				memcpy(current_data_position, &data, sizeof(data));
				break;
			}
			case _field_long_enum:
			{
				long data = *reinterpret_cast<const long*>(high_level_field_data);
				memcpy(current_data_position, &data, sizeof(data));
				break;
			}
			case _field_byte_flags:
			{
				byte data = static_cast<byte>(*reinterpret_cast<const dword*>(high_level_field_data));
				memcpy(current_data_position, &data, sizeof(data));
				break;
			}
			case _field_word_flags:
			{
				word data = static_cast<word>(*reinterpret_cast<const dword*>(high_level_field_data));
				memcpy(current_data_position, &data, sizeof(data));
				break;
			}
			case _field_long_flags:
			{
				dword data = *reinterpret_cast<const dword*>(high_level_field_data);
				memcpy(current_data_position, &data, sizeof(data));
				break;
			}
			case _field_old_string_id:
			case _field_string_id:
			{
				string_id& stringid = *reinterpret_cast<string_id*>(current_data_position);
				const h_string_id& stringid_storage = *reinterpret_cast<decltype(&stringid_storage)>(high_level_field_data);
				stringid = string_id_manager.commit_string(stringid_storage.c_str());
				DEBUG_ASSERT(stringid_storage.empty() || stringid != 0);

				break;
			}
			case _field_block:
			{
				s_tag_block& tag_block = *reinterpret_cast<decltype(&tag_block)>(current_data_position);
				const h_block& block_storage = *reinterpret_cast<decltype(&block_storage)>(high_level_field_data);

				if (block_storage.size() == 0)
				{
					tag_block.count = 0;
					tag_block.address = 0;
					tag_block.definition_address = 0;
				}
				else
				{
					uint32_t type_size = block_storage.get_low_level_type_size();
					uint32_t block_data_size = type_size * block_storage.size();

					tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), block_memory_alignment));
					char* const tag_block_data = tag_allocation_postion;
					tag_allocation_postion += align_value(block_data_size, block_memory_alignment);

					uint32_t relative_offset = get_tag_pointer_relative_offset(tag_block_data);
					uint32_t page_offset = encode_page_offset(relative_offset);

					tag_block.count = static_cast<uint32_t>(block_storage.size());
					tag_block.address = page_offset;
					tag_block.definition_address = 0;

					char* current_tag_block_data_position = tag_block_data;
					for (uint32_t block_index = 0; block_index < block_storage.size(); block_index++)
					{
						const h_object& block_object = block_storage.get(block_index);

						compile_object(tag, block_object, current_tag_block_data_position, tag_allocation_postion);

						current_tag_block_data_position += type_size;
					}
				}

				break;
			}
			case _field_struct:
			{
				const h_object& struct_storage = *reinterpret_cast<decltype(&struct_storage)>(high_level_field_data);
				compile_object(tag, struct_storage, current_data_position, tag_allocation_postion);
				debug_point;
				break;
			}
			case _field_array:
			{
				const h_enumerable& array_storage = *reinterpret_cast<decltype(&array_storage)>(high_level_field_data);
				char* raw_array_data_position = current_data_position;

				uint32_t const array_elements_count = array_storage.size();
				for (uint32_t array_index = 0; array_index < array_elements_count; array_index++)
				{
					const h_object& array_element_storage = array_storage[array_index];

					compile_object(tag, array_element_storage, raw_array_data_position, tag_allocation_postion);

					raw_array_data_position += array_element_storage.get_low_level_type_size();
				}
				break;
			}
			case _field_tag_reference:
			{
				s_tag_reference& tag_reference = *reinterpret_cast<decltype(&tag_reference)>(current_data_position);
				const h_tag* const& tag_reference_storage = *reinterpret_cast<decltype(&tag_reference_storage)>(high_level_field_data);

				if (tag_reference_storage != nullptr)
				{
					tag_reference.group_tag = tag_reference_storage->group->tag_group.group_tag; // #TODO: kinda hacky
					tag_reference.index = get_tag_index(tag_reference_storage);
					tag_reference.datum = static_cast<uint16_t>(tag_reference.index + 0xe175u);
				}
				else
				{
					tag_reference.group_tag = blofeld::INVALID_TAG;
					tag_reference.index = ~0ui16;
					tag_reference.datum = ~0ui16;
				}
				tag_reference.name = 0;
				tag_reference.name_length = 0;
				break;
			}
			case _field_data:
			{
				s_tag_data& tag_data = *reinterpret_cast<decltype(&tag_data)>(current_data_position);
				const h_data& data_storage = *reinterpret_cast<decltype(&data_storage)>(high_level_field_data);

				if (data_storage.size() == 0)
				{
					tag_data.size = 0;
					tag_data.stream_flags = 0;
					tag_data.stream_offset = 0;
					tag_data.address = 0;
					tag_data.definition = 0;
				}
				else
				{
					uint32_t block_data_size = static_cast<uint32_t>(data_storage.size());

					tag_allocation_postion = reinterpret_cast<char*>(align_value(reinterpret_cast<uintptr_t>(tag_allocation_postion), block_memory_alignment));
					char* const tag_data_data = tag_allocation_postion;
					tag_allocation_postion += align_value(block_data_size, block_memory_alignment);

					uint32_t relative_offset = get_tag_pointer_relative_offset(tag_data_data);
					uint32_t page_offset = encode_page_offset(relative_offset);

					tag_data.size = static_cast<uint32_t>(data_storage.size());
					tag_data.stream_flags = 0;
					tag_data.stream_offset = 0;
					tag_data.address = page_offset;
					tag_data.definition = 0;

					memcpy(tag_data_data, data_storage.data(), block_data_size);
				}

				break;
			}
			case _field_pageable:
			{
				s_tag_resource& tag_resource = *reinterpret_cast<decltype(&tag_resource)>(current_data_position);
				const h_resource& resource_storage = *reinterpret_cast<decltype(&resource_storage)>(high_level_field_data);

				tag_resource = resource_storage._original_resource;
				break;
			}
			case _field_api_interop:
			{
				s_tag_interop& tag_interop = *reinterpret_cast<decltype(&tag_interop)>(current_data_position);
				const h_interop& interop_storage = *reinterpret_cast<decltype(&interop_storage)>(high_level_field_data);

				uint32_t relative_offset = get_tag_pointer_relative_offset(current_data_position);
				uint32_t page_offset = encode_page_offset(relative_offset);
				interop_page_offsets[&interop_storage] = page_offset;

				tag_interop = interop_storage._original_interop;

				break;
			}
			case _field_vertex_buffer:
			case _field_pad:
			case _field_useless_pad:
			case _field_skip:
			case _field_non_cache_runtime_value:
			case _field_explanation:
			case _field_custom:
			case _field_terminator:
			{
				// #TODO
			}
			}
		}

		current_data_position += field_size;
		data_written += field_size;
	}
}

void c_haloreach_cache_compiler::create_tag_groups()
{
	TASK_TIMER(__FUNCTION__);

	using namespace blofeld;

	{
		for (s_tag_group_info** haloreach_tag_groups = blofeld::haloreach::haloreach_tag_groups; *haloreach_tag_groups; haloreach_tag_groups++)
		{
			tag_group_count++;
		}

		tag_groups_buffer_size = align_value(tag_group_count * sizeof(s_cache_file_tag_group), 0x10000);
		tag_groups_buffer = new char[tag_groups_buffer_size] {};
		tag_groups = new(tag_groups_buffer) s_cache_file_tag_group[tag_group_count]{};

		for (uint32_t i = 0; i < tag_group_count; i++)
		{
			s_tag_group_info& tag_group_info = *blofeld::haloreach::haloreach_tag_groups[i];
			s_cache_file_tag_group& tag_group = tag_groups[i];

			tag_group.group_tags[0] = tag_group_info.group_tag;
			tag_group.group_tags[1] = tag_group_info.parents[0] != nullptr ? tag_group_info.parents[0]->group_tag : blofeld::INVALID_TAG;
			tag_group.group_tags[2] = tag_group_info.parents[1] != nullptr ? tag_group_info.parents[1]->group_tag : blofeld::INVALID_TAG;
			tag_group.name = string_id_manager.commit_string(tag_group_info.group_name);
		}
	}
}

void c_haloreach_cache_compiler::init_tags()
{
	TASK_TIMER(__FUNCTION__);

	tag_data_entry_count = static_cast<uint32_t>(tag_project.tags.size());
	tag_data_entries = reinterpret_cast<s_tag_data_entry*>(new char[sizeof(s_tag_data_entry) * tag_data_entry_count]{});

	uint32_t file_index = 0;
	for (h_tag* tag : tag_project.tags)
	{
		uint32_t tag_data_size = calculate_size(*tag);
		ASSERT((tag_data_size % 4) == 0);

		uint32_t tag_group_index = ~0u;
		cache_compiler::s_cache_file_tag_group* tag_group = nullptr;
		for (uint32_t _tag_group_index = 0; _tag_group_index < tag_group_count; _tag_group_index++)
		{
			if (tag_groups[_tag_group_index].group_tags[0] == tag->group->tag_group.group_tag) // #TODO: this is kinda crazy?
			{
				tag_group = &tag_groups[_tag_group_index];
				tag_group_index = _tag_group_index;
			}
		}
		ASSERT(tag_group_index != ~0u);

		s_tag_data_entry& data_entry = *new(tag_data_entries + file_index) s_tag_data_entry
		{
			*tag,
			tag_data_data_size,
			tag_data_size,
			nullptr,
			tag_file_table_data_size,
			file_index,
			file_index + 0x6472,
			nullptr,
			tag_group,
			tag_group_index,
			tag->tag_filepath
		};

		PathRemoveExtensionA(data_entry.path.data);

		tag_file_table_data_size += static_cast<uint32_t>(data_entry.path.size()) + 1;
		tag_data_data_size += tag_data_size;
		file_index++;
	}
}

void c_haloreach_cache_compiler::create_tag_file_table()
{
	TASK_TIMER(__FUNCTION__);

	tag_file_table_buffer_size = align_value(tag_file_table_data_size, 0x10000);
	tag_file_table_buffer = new char[tag_file_table_buffer_size] {};

	tag_file_table_indices_count = static_cast<uint32_t>(tag_project.tags.size());
	tag_file_table_indices_buffer_size = align_value(tag_file_table_indices_count * sizeof(uint32_t), 0x10000);
	tag_file_table_indices_buffer = new char[tag_file_table_indices_buffer_size] {};
	uint32_t* tag_file_table_indices = new(tag_file_table_indices_buffer) uint32_t[tag_file_table_indices_count]{};

	for (uint32_t tag_index = 0; tag_index < tag_data_entry_count; tag_index++)
	{
		s_tag_data_entry& tag_data_entry = tag_data_entries[tag_index];

		tag_data_entry.tag_file_table_entry = tag_file_table_buffer + tag_data_entry.tag_file_table_offset;
		tag_file_table_indices[tag_data_entry.tag_file_table_index] = tag_data_entry.tag_file_table_offset;
		strcpy(tag_data_entry.tag_file_table_entry, tag_data_entry.path.data);

		debug_point;
	}
}

void c_haloreach_cache_compiler::pre_compile_tags()
{
	TASK_TIMER(__FUNCTION__);

	tag_data_buffer_size = align_value(tag_data_data_size, 0x10000);
	tag_data_buffer = new char[tag_data_buffer_size] {};

	tag_instance_count = static_cast<uint32_t>(tag_project.tags.size());
	tag_instances_data_size = tag_instance_count * sizeof(s_cache_file_tag_instance);
	tag_instances_buffer_size = align_value(tag_instances_data_size, 0x10000);
	tag_instances_buffer = new char[tag_instances_buffer_size] {};
}

void c_haloreach_cache_compiler::compile_tags()
{
	TASK_TIMER(__FUNCTION__);

	for (uint32_t tag_index = 0; tag_index < tag_data_entry_count; tag_index++)
	{
		s_tag_data_entry& tag_data_entry = tag_data_entries[tag_index];

		tag_data_entry.tag_data = tag_data_buffer + tag_data_entry.tag_data_offset;

		compile_tag(tag_data_entry.tag, tag_data_entry.tag_data, tag_data_entry.tag_data_size); // #TODO: multithread?

		debug_point;
	}
}

void c_haloreach_cache_compiler::compile_string_ids()
{
	string_ids_count = 0;
	string_ids_data_size = 0;

	for (uint32_t set_index = 0; set_index < c_string_id_manager::k_num_sets; set_index++)
	{
		uint32_t engine_set_count = string_id_manager.engine_set_counts[set_index];
		uint32_t set_count = static_cast<uint32_t>(string_id_manager.string_ids[set_index].size());

		string_ids_count += set_count;

		for (std::string& str : string_id_manager.string_ids[set_index])
		{
			string_ids_data_size += static_cast<uint32_t>(str.size() + 1u);
		}
	}

	string_ids_indices_data_size = string_ids_count * sizeof(uint32_t);
	string_ids_indices_buffer_size = align_value(string_ids_indices_data_size, 0x10000);
	string_ids_indices_buffer = new char[string_ids_indices_buffer_size] {};

	string_ids_buffer_size = align_value(string_ids_data_size, 0x10000);
	string_ids_buffer = new char[string_ids_buffer_size] {};

	uint32_t* string_indices = new(string_ids_indices_buffer) uint32_t[string_ids_count]{};
	uint32_t current_string_index = 0;
	uint32_t current_buffer_position = 0;
	for (uint32_t set_index = 0; set_index < c_string_id_manager::k_num_sets; set_index++)
	{
		uint32_t engine_set_count = string_id_manager.engine_set_counts[set_index];

		for (uint32_t string_index = 0; string_index < engine_set_count; string_index++)
		{
			std::string& str = string_id_manager.string_ids[set_index][string_index];

			uint32_t string_size = static_cast<uint32_t>(str.size() + 1u);

			strcpy(string_ids_buffer + current_buffer_position, str.data());

			string_indices[current_string_index] = current_buffer_position;

			current_string_index++;
			current_buffer_position += string_size;
		}
	}

	uint32_t user_ids_begin = string_id_manager.engine_set_counts[0];
	uint32_t user_ids_end = static_cast<uint32_t>(string_id_manager.string_ids[0].size());
	for (uint32_t string_index = user_ids_begin; string_index < user_ids_end; string_index++)
	{
		std::string& str = string_id_manager.string_ids[0][string_index];

		uint32_t string_size = static_cast<uint32_t>(str.size() + 1u);

		strcpy(string_ids_buffer + current_buffer_position, str.data());

		string_indices[current_string_index] = current_buffer_position;

		current_string_index++;
		current_buffer_position += string_size;
	}
	DEBUG_ASSERT(current_buffer_position == string_ids_data_size);

	string_id_namespace_count = 0;
	for (uint32_t set_index = 0; set_index < c_string_id_manager::k_num_sets; set_index++)
	{
		uint32_t engine_set_count = string_id_manager.engine_set_counts[set_index];
		if (engine_set_count > 0)
		{
			string_id_namespace_count = set_index + 1;
		}
	}

	string_id_namespaces_data_size = string_id_namespace_count * sizeof(uint32_t);
	string_id_namespaces_buffer_size = align_value(string_id_namespaces_data_size, 0x10000);
	string_id_namespaces_buffer = new char[string_id_namespaces_buffer_size] {};

	uint32_t* string_id_namespaces = new(string_id_namespaces_buffer) uint32_t[string_id_namespace_count]{};

	for (uint32_t set_index = 0; set_index < string_id_namespace_count; set_index++)
	{
		uint32_t engine_set_count = string_id_manager.engine_set_counts[set_index];
		string_id_namespaces[set_index] = engine_set_count;
	}
}

void c_haloreach_cache_compiler::compile_resources()
{
	using namespace blofeld::haloreach;

	auto& resource_section = cache_file->get_resources_section();
	resources_data_size = resource_section.size;
	resources_buffer_size = align_value(resources_data_size, 0x10000);
	resources_buffer = new char[resources_buffer_size] {};
	memcpy(resources_buffer, resource_section.data, resources_data_size);


#if 0
	auto resource_layout_table_group = cache_file->get_tag_group_interface_by_group_id(blofeld::CACHE_FILE_RESOURCE_LAYOUT_TABLE_TAG);
	DEBUG_ASSERT(resource_layout_table_group != nullptr);
	auto resource_layout_table_tag = resource_layout_table_group->get_tag_interfaces()[0];
	DEBUG_ASSERT(resource_layout_table_tag != nullptr);
	auto resource_layout_table = dynamic_cast<v_tag_interface<s_cache_file_resource_layout_table_block_struct>*>(resource_layout_table_tag->get_virtual_tag_interface());
	DEBUG_ASSERT(resource_layout_table != nullptr);

	//struct s_resource_fixup_info
	//{
	//	uint32_t section_offset;
	//};
	//std::vector<s_resource_fixup_info> resource_fixups;
	//resource_fixups.resize(resource_layout_table->file_pages_block.count);

	char* map_data = cache_file->get_map_data();
	char* resources_data = cache_file->get_resources_section().data;
	uint32_t resource_section_file_offset = resources_data - map_data;

	for (uint32_t file_page_index = 0; file_page_index < resource_layout_table->file_pages_block.count; file_page_index++)
	{
		//s_resource_fixup_info& resource_fixup = resource_fixups[file_page_index];

		auto& file_page = resource_layout_table->file_pages_block[file_page_index];

		if (file_page.shared_file == -1)
		{
			//resource_fixup.section_offset = file_page.file_offset - resource_section_file_offset;
			file_page.file_offset += 1337;
			debug_point;
		}
	}
#endif

	{
		h_group* resource_layout_table_group = tag_project.get_group_by_group_tag(blofeld::CACHE_FILE_RESOURCE_LAYOUT_TABLE_TAG);
		DEBUG_ASSERT(resource_layout_table_group != nullptr);
		DEBUG_ASSERT(resource_layout_table_group->tags.size() > 0);
		h_cache_file_resource_layout_table_block_struct* cache_file_resource_layout_table = dynamic_cast<h_cache_file_resource_layout_table_block_struct*>(resource_layout_table_group->tags.front());
		DEBUG_ASSERT(cache_file_resource_layout_table != nullptr);

		for (auto& file_page : cache_file_resource_layout_table->file_pages_block)
		{
			if (file_page.shared_file == -1)
			{
				file_page.file_offset.value += 40960;
				debug_point;
			}
		}
	}
}

void c_haloreach_cache_compiler::compile_tag_instances()
{
	s_cache_file_tag_instance* tag_instances = new(tag_instances_buffer) s_cache_file_tag_instance[tag_instance_count]{};
	for (uint32_t tag_index = 0; tag_index < tag_data_entry_count; tag_index++)
	{
		s_tag_data_entry& tag_data_entry = tag_data_entries[tag_index];

		uint32_t instance_index = tag_data_entry.tag_file_table_index;
		uint32_t datum_index = tag_data_entry.tag_file_table_datum_index;
		s_cache_file_tag_instance& tag_instance = tag_instances[instance_index];
		tag_instance.identifier = static_cast<uint16_t>(datum_index);
		tag_instance.address = encode_page_offset(tag_data_entry.tag_data_offset + DEBUG_PADDING);
		tag_instance.group_index = tag_data_entry.tag_group_index;

		uint64_t reconstructed_address = decode_page_offset(tag_instance.address);
		DEBUG_ASSERT(reconstructed_address == static_cast<uint64_t>(tag_data_entry.tag_data_offset + DEBUG_PADDING));

		debug_point;
	}
}

void c_haloreach_cache_compiler::pre_compile_global_tag_instances()
{
	using namespace blofeld;
	using namespace blofeld::haloreach;

	tag_global_entries_count = 0;

	for (s_tag_group_info** current_tag_group_info = haloreach_global_tag_groups; *current_tag_group_info != nullptr; current_tag_group_info++)
	{
		s_tag_group_info& tag_group_info = **current_tag_group_info;

		for (uint32_t tag_index = 0; tag_index < tag_data_entry_count; tag_index++)
		{
			s_tag_data_entry& tag_data_entry = tag_data_entries[tag_index];

			if (tag_data_entry.tag_group->group_tags[0] == tag_group_info.group_tag)
			{
				tag_global_entries_count++;
				break; // only one global tag of each group #TODO: assert this
			}
		}
	}

	tag_global_entries_data_size = tag_global_entries_count * sizeof(s_cache_file_tag_global_instance);
	tag_global_entries_buffer_size = align_value(tag_global_entries_data_size, 0x10000);
	tag_global_entries_buffer = new char[tag_global_entries_buffer_size] {};
}

void c_haloreach_cache_compiler::compile_global_tag_instances()
{
	using namespace blofeld;
	using namespace blofeld::haloreach;

	s_cache_file_tag_global_instance* tag_global_instances = new(tag_global_entries_buffer) s_cache_file_tag_global_instance[tag_global_entries_count]{};

	uint32_t global_tag_entry_index = 0;
	for (s_tag_group_info** current_tag_group_info = haloreach_global_tag_groups; *current_tag_group_info != nullptr; current_tag_group_info++)
	{
		s_tag_group_info& tag_group_info = **current_tag_group_info;

		for (uint32_t tag_index = 0; tag_index < tag_data_entry_count; tag_index++)
		{
			s_tag_data_entry& tag_data_entry = tag_data_entries[tag_index];

			if (tag_data_entry.tag_group->group_tags[0] == tag_group_info.group_tag)
			{
				uint32_t instance_index = tag_data_entry.tag_file_table_index;
				uint32_t datum_index = tag_data_entry.tag_file_table_datum_index;
				s_cache_file_tag_global_instance& global_tag_instance = tag_global_instances[global_tag_entry_index];
				global_tag_instance.tag_index = instance_index;
				global_tag_instance.identifier = datum_index;
				global_tag_instance.group_tag = tag_data_entry.tag_group->group_tags[0]; // #TODO: is this hacky?

				c_console::write_line("creating global entry for '%s' '%s'", tag_group_info.group_name, tag_data_entry.path.data);

				global_tag_entry_index++;
				break; // only one global tag of each group
			}
		}
	}
}




class h_structured_buffer_container :
	public c_interop_container
{
public:
	h_structured_buffer_container(c_haloreach_cache_compiler& compiler, c_cache_file* cache_file, uint32_t index) :
		c_interop_container(_tag_interop_type_structured_buffer),
		compiler(compiler)
	{
		using namespace blofeld::haloreach;

		auto scenario_group = cache_file->get_tag_group_interface_by_group_id(blofeld::SCENARIO_TAG);
		DEBUG_ASSERT(scenario_group != nullptr);
		auto scenario_tag = scenario_group->get_tag_interfaces()[0];
		DEBUG_ASSERT(scenario_tag != nullptr);
		auto scenario = dynamic_cast<v_tag_interface<s_scenario_struct_definition>*>(scenario_tag->get_virtual_tag_interface());
		DEBUG_ASSERT(scenario != nullptr);

		auto structured_buffer_interops = scenario->structured_buffer_interops_block[0];

		uint32_t page_offsets[4] =
		{
			structured_buffer_interops.effect.descriptor,
			structured_buffer_interops.beam.descriptor,
			structured_buffer_interops.contrail.descriptor,
			structured_buffer_interops.light_volume.descriptor,
		};
	}

	virtual void compile(void* tag_data)
	{
		using namespace blofeld::haloreach;

		s_structured_buffer& structured_buffer = *reinterpret_cast<decltype(&structured_buffer)>(tag_data);

	}

	virtual uint32_t get_data_size() const
	{
		return data_size;
	}

	char* data_buffer;
	uint32_t data_size;
	c_haloreach_cache_compiler& compiler;
};

class h_constant_buffer_container :
	public c_interop_container
{
public:
	h_constant_buffer_container() :
		c_interop_container(_tag_interop_type_constant_buffer)
	{

	}

	virtual uint32_t get_data_size() const
	{
		return 0;
	}
};

void c_haloreach_cache_compiler::pre_compile_interops()
{
	using namespace blofeld::haloreach;

	{
		effect_structured_buffer = new h_structured_buffer_container(*this, cache_file, 0);
		beam_structured_buffer = new h_structured_buffer_container(*this, cache_file, 1);
		contrail_structured_buffer = new h_structured_buffer_container(*this, cache_file, 2);
		light_volume_structured_buffer = new h_structured_buffer_container(*this, cache_file, 3);

		interop_containers.push_back(effect_structured_buffer);
		interop_containers.push_back(beam_structured_buffer);
		interop_containers.push_back(contrail_structured_buffer);
		interop_containers.push_back(light_volume_structured_buffer);
	}

	{
		auto cache_file_tag_interop_type_fixups = reinterpret_cast<s_cache_file_tag_interop_type_fixup*>(cache_file->tags_buffer + cache_file->convert_virtual_address(cache_file->haloreach_cache_file_tags_header->tag_interop_table.address));
		uint32_t tag_interop_count = cache_file->haloreach_cache_file_tags_header->tag_interop_table.count;
		for (uint32_t interop_type_fixup_index = 0; interop_type_fixup_index < tag_interop_count; interop_type_fixup_index++)
		{
			s_cache_file_tag_interop_type_fixup& cache_file_tag_interop_type_fixup = cache_file_tag_interop_type_fixups[interop_type_fixup_index];

			switch (static_cast<e_tag_interop_type>(cache_file_tag_interop_type_fixup.type_index))
			{
			case _tag_interop_type_constant_buffer:
			{
				s_constant_buffer* constant_buffer = reinterpret_cast<s_constant_buffer*>(cache_file->get_data_with_page_offset(cache_file_tag_interop_type_fixup.page_address));
				debug_point;

				h_constant_buffer_container* constant_buffer_container = new h_constant_buffer_container();
				interop_containers.push_back(constant_buffer_container);

				debug_point;
				break;
			}
			case _tag_interop_type_structured_buffer:
				break;
			default:
				throw;
			}
		}
	}

	//tag_api_interops_count = cache_file->haloreach_cache_file_tags_header->tag_interop_table.count;
	tag_api_interops_count = static_cast<uint32_t>(interop_containers.size());
	tag_api_interops_data_size = 0;
	for (c_interop_container* interop_container : interop_containers)
	{
		uint32_t interop_alignment = 1u << interop_container->interop_vtable->alignment_bits;
		tag_api_interops_data_size = align_value(tag_api_interops_data_size, interop_alignment);
		tag_api_interops_data_size += interop_container->interop_vtable->size;

		uint32_t data_size = interop_container->get_data_size();
		if (data_size > 0)
		{
			tag_api_interops_data_size = align_value(tag_api_interops_data_size, interop_alignment);
			tag_api_interops_data_size += data_size;
		}
	}
	tag_api_interops_buffer_size = align_value(tag_api_interops_data_size, 0x10000);
	tag_api_interops_buffer = new char[tag_api_interops_buffer_size] {};

	tag_api_interop_fixups_data_size = sizeof(s_cache_file_tag_interop_type_fixup) * tag_api_interops_count;
	tag_api_interop_fixups_buffer_size = align_value(tag_api_interop_fixups_data_size, 0x10000);
	tag_api_interop_fixups_buffer = new char[tag_api_interop_fixups_buffer_size] {};
}

void c_haloreach_cache_compiler::compile_interops()
{
	s_cache_file_tag_interop_type_fixup* interop_fixup_table = reinterpret_cast<s_cache_file_tag_interop_type_fixup*>(tag_api_interop_fixups_buffer);
	char* interop_buffer_position = tag_api_interops_buffer;

	uint32_t interop_buffer_offset = 0;
	for (uint32_t interop_index = 0; interop_index < tag_api_interops_count; interop_index++)
	{
		c_interop_container& interop_container = *interop_containers[interop_index];
		s_cache_file_tag_interop_type_fixup& interop_fixup = interop_fixup_table[interop_index];

		uint32_t interop_alignment = 1u << interop_container.interop_vtable->alignment_bits;
		interop_buffer_offset = align_value(interop_buffer_offset, interop_alignment);
		interop_buffer_offset += interop_container.interop_vtable->size;



		//if (&interop_container == effect_structured_buffer)
		//{
		//	interop_fixup.type_index = interop_container.interop_type;
		//	DEBUG_ASSERT(interop_page_offsets.find() != interop_page_offsets.end());
		//	interop_fixup.page_address = interop_page_offsets[];
		//}
		//if (&interop_container == beam_structured_buffer)
		//{

		//}
		//if (&interop_container == contrail_structured_buffer)
		//{

		//}
		//if (&interop_container == light_volume_structured_buffer)
		//{

		//}

		uint32_t data_size = interop_container.get_data_size();
		if (data_size > 0)
		{
			interop_buffer_offset = align_value(interop_buffer_offset, interop_alignment);
			interop_buffer_offset += data_size;
		}

		interop_index++;
	}

	//for (uint32_t interop_index = 0; interop_index < tag_api_interops_count; interop_index++)
	//{
	//	s_cache_file_tag_interop_type_fixup& interop = interop_table[interop_index];

	//	switch (interop_index)
	//	{
	//	case 0:
	//		interop.type_index = 7;
	//		interop.page_address = effect_structured_buffer_api_interop_page_offset;
	//		break;
	//	case 1:
	//		interop.type_index = 7;
	//		interop.page_address = beam_structured_buffer_api_interop_page_offset;
	//		break;
	//	case 2:
	//		interop.type_index = 7;
	//		interop.page_address = contrail_structured_buffer_api_interop_page_offset;
	//		break;
	//	case 3:
	//		interop.type_index = 7;
	//		interop.page_address = light_volume_structured_buffer_api_interop_page_offset;
	//		break;
	//	}
	//}


	//if (const blofeld::haloreach::h_scenario_struct_definition* scenario = dynamic_cast<decltype(scenario)>(&tag))
	//{

	//	if (&scenario->structured_buffer_interops_block[0].effect.value == &interop_storage)
	//	{
	//		effect_structured_buffer_api_interop_page_offset = page_offset;
	//		debug_point;
	//	}
	//	if (&scenario->structured_buffer_interops_block[0].beam.value == &interop_storage)
	//	{
	//		beam_structured_buffer_api_interop_page_offset = page_offset;
	//		debug_point;
	//	}
	//	if (&scenario->structured_buffer_interops_block[0].contrail.value == &interop_storage)
	//	{
	//		contrail_structured_buffer_api_interop_page_offset = page_offset;
	//		debug_point;
	//	}
	//	if (&scenario->structured_buffer_interops_block[0].light_volume.value == &interop_storage)
	//	{
	//		light_volume_structured_buffer_api_interop_page_offset = page_offset;
	//		debug_point;
	//	}
	//}
	//uint32_t effect_structured_buffer_api_interop_page_offset;
	//uint32_t beam_structured_buffer_api_interop_page_offset;
	//uint32_t contrail_structured_buffer_api_interop_page_offset;
	//uint32_t light_volume_structured_buffer_api_interop_page_offset;





	//{
	//	auto v_scenario_group = cache_file->get_tag_group_interface_by_group_id(blofeld::SCENARIO_TAG);
	//	DEBUG_ASSERT(v_scenario_group != nullptr);
	//	auto v_scenario_tag = v_scenario_group->get_tag_interfaces()[0];
	//	DEBUG_ASSERT(v_scenario_tag != nullptr);
	//	auto v_scenario = dynamic_cast<v_tag_interface<s_scenario_struct_definition>*>(v_scenario_tag->get_virtual_tag_interface());
	//	DEBUG_ASSERT(v_scenario != nullptr);

	//	auto structured_buffer_interops = v_scenario->structured_buffer_interops_block[0];
	//	uint32_t const structured_buffer_page_offsets[4] =
	//	{
	//		structured_buffer_interops.effect.descriptor,
	//		structured_buffer_interops.beam.descriptor,
	//		structured_buffer_interops.contrail.descriptor,
	//		structured_buffer_interops.light_volume.descriptor,
	//	};

	//	uint32_t const dataref_section_offset = tags_section_structured_buffer_interops_offset;

	//	char* structured_buffer_interop_datarefs = structured_buffer_interop_buffer;
	//	s_structured_buffer(&datarefs)[4] = *reinterpret_cast<decltype(&datarefs)>(structured_buffer_interop_datarefs);

	//	char* structured_buffer_data_position = structured_buffer_interop_datarefs + align_value(sizeof(s_structured_buffer) * 4, 0x1000);
	//	uint32_t buffer_section_offset = dataref_section_offset + align_value(sizeof(s_structured_buffer) * 4, 0x1000);
	//	for (uint32_t interop_index = 0; interop_index < 4; interop_index++)
	//	{
	//		uint32_t structured_buffer_page_offset = structured_buffer_page_offsets[interop_index];
	//		s_structured_buffer& structured_buffer_data = *reinterpret_cast<s_structured_buffer*>(cache_file->get_data_with_page_offset(structured_buffer_page_offset));

	//		char* interop_data = cache_file->get_data_with_page_offset(structured_buffer_data.data.address);
	//		memcpy(structured_buffer_data_position, interop_data, structured_buffer_data.data.size);

	//		uint32_t dataref_page_offset = encode_page_offset(dataref_section_offset - tags_section_begin_file_offset + sizeof(s_structured_buffer) * interop_index);

	//		datarefs[interop_index].data.size = structured_buffer_data.data.size;
	//		datarefs[interop_index].data.address = encode_page_offset(buffer_section_offset - tags_section_begin_file_offset);

	//		uint32_t buffer_size = align_value(structured_buffer_data.data.size, 0x1000);
	//		structured_buffer_data_position += buffer_size;
	//		buffer_section_offset += buffer_size;

	//		switch (interop_index)
	//		{
	//		case 0:
	//			scenario->structured_buffer_interops_block[0].effect.value.descriptor = dataref_page_offset;
	//			datarefs[interop_index].entry_size = 0x7A0;
	//			break;
	//		case 1:
	//			scenario->structured_buffer_interops_block[0].beam.value.descriptor = dataref_page_offset;
	//			datarefs[interop_index].entry_size = 0x740;
	//			break;
	//		case 2:
	//			scenario->structured_buffer_interops_block[0].contrail.value.descriptor = dataref_page_offset;
	//			datarefs[interop_index].entry_size = 0x770;
	//			break;
	//		case 3:
	//			scenario->structured_buffer_interops_block[0].light_volume.value.descriptor = dataref_page_offset;
	//			datarefs[interop_index].entry_size = 0x700;
	//			break;
	//		}
	//	}

	//	DEBUG_ASSERT(buffer_section_offset <= tags_section_end_file_offset);

	//	debug_point;
	//}
}

void c_haloreach_cache_compiler::compile(const wchar_t* filepath)
{
	using namespace blofeld;
	using namespace blofeld::haloreach;


	h_group* globals_group = tag_project.get_group_by_group_tag(blofeld::GLOBALS_TAG);
	DEBUG_ASSERT(globals_group != nullptr);
	DEBUG_ASSERT(globals_group->tags.size() > 0);

	h_globals_struct_definition* globals_tag = dynamic_cast<h_globals_struct_definition*>(globals_group->tags.front());
	DEBUG_ASSERT(globals_tag != nullptr);

	globals_tag->language_pack1 = {};
	globals_tag->language_pack2 = {};
	globals_tag->language_pack3 = {};
	globals_tag->language_pack4 = {};
	globals_tag->language_pack5 = {};
	globals_tag->language_pack6 = {};
	globals_tag->language_pack7 = {};
	globals_tag->language_pack8 = {};
	globals_tag->language_pack9 = {};
	globals_tag->language_pack10 = {};
	globals_tag->language_pack11 = {};
	globals_tag->language_pack12 = {};

	blofeld::haloreach::h_scenario_struct_definition* scenario = dynamic_cast<decltype(scenario)>(tag_project.tags[8]);
	DEBUG_ASSERT(scenario != nullptr);

	scenario->hs_syntax_datums_block.clear();
	scenario->scripts_block.clear();
	scenario->globals_block.clear();
	//scenario->soft_ceilings_block.clear();
	//scenario->trigger_volumes_block.clear();
	//scenario->scenario_kill_triggers_block.clear();
	//scenario->scenario_safe_zone_triggers_block.clear();
	//scenario->scenario_requisition_triggers_block.clear();
	//scenario->scenario_requisition_triggers_block.clear();
	//scenario->scenario_location_name_triggers_block.clear();

#define push_to_vector(vector, data, size) \
	vector.insert(vector.end(), reinterpret_cast<const char*>(data), reinterpret_cast<const char*>(data) + (size))

	create_tag_groups();
	init_tags();
	create_tag_file_table();
	pre_compile_tags();
	pre_compile_global_tag_instances();
	pre_compile_interops();
	compile_resources();

	auto& localization_section = cache_file->get_localization_section();
	localization_data_size = localization_section.size;
	localization_buffer_size = align_value(localization_data_size, 0x10000);
	localization_buffer = new char[localization_buffer_size] {};
	memcpy(localization_buffer, localization_section.data, localization_data_size);

	uint32_t tags_header_buffer_size = align_value(sizeof(s_cache_file_tags_header), 0x10000);
	char* tags_header_buffer = new char[tags_header_buffer_size] {};
	s_cache_file_tags_header& tags_header = *reinterpret_cast<s_cache_file_tags_header*>(tags_header_buffer);

	struct s_section_info
	{
		uint32_t section_file_offset;
		uint32_t section_offset_from_header;
		uint32_t section_size;
	};
	s_section_info section_infos[gen3::k_number_of_cache_file_sections] = {};



	uint32_t total_cache_file_data = sizeof(s_cache_file_metadata);
	uint32_t total_section_data = 0;
	uint32_t loaded_section_data = 0;
#define get_tag_section_virtual_address(offset) (virtual_base_address + (offset - tags_section_begin_file_offset))

	char* empty_buffer[0x10000] = {};

	gen3::e_cache_file_section_index cache_file_section_indirection[gen3::k_number_of_cache_file_sections] =
	{
		gen3::_cache_file_section_index_resource,
		gen3::_cache_file_section_index_tags,
		gen3::_cache_file_section_index_localization,
		gen3::_cache_file_section_index_debug,
	};
	{

		for (uint32_t section_indirection_index = 0; section_indirection_index < gen3::k_number_of_cache_file_sections; section_indirection_index++)
		{
			gen3::e_cache_file_section_index section_index = cache_file_section_indirection[section_indirection_index];

			switch (section_index)
			{
			case gen3::_cache_file_section_index_resource:
			{
				section_data[section_index].insert(section_data[section_index].end(), resources_buffer, resources_buffer + resources_buffer_size);

				break;
			}
			case gen3::_cache_file_section_index_localization:
			{
				section_data[section_index].insert(section_data[section_index].end(), localization_buffer, localization_buffer + localization_buffer_size);
				break;
			}
			case gen3::_cache_file_section_index_tags:
			{
				tags_section_begin_file_offset = total_cache_file_data;
				tags_section_tag_data_offset = tags_section_begin_file_offset;
				tags_section_tags_header_offset = tags_section_tag_data_offset + tag_data_buffer_size;
				tags_section_tag_groups_offset = tags_section_tags_header_offset + tags_header_buffer_size;
				tags_section_tag_instances_offset = tags_section_tag_groups_offset + tag_groups_buffer_size;
				tags_section_tag_global_instances_offset = tags_section_tag_instances_offset + tag_instances_buffer_size;
				tags_section_api_interop_fixups_offset = tags_section_tag_global_instances_offset + tag_global_entries_buffer_size;
				tags_section_api_interop_data_offset = tags_section_api_interop_fixups_offset + tag_api_interops_buffer_size;
				uint32_t const tags_section_end_file_offset = tags_section_api_interop_data_offset + tag_api_interop_fixups_buffer_size;

				uint32_t const tags_section_size = tags_section_end_file_offset - tags_section_begin_file_offset;

				/* #TODO #THEORY Research Needed */ uint64_t application_base_address = 0x0000000180000000;
				/* #TODO #THEORY Research Needed */ uint32_t file_index = 5;
				/* #TODO #THEORY Research Needed */ uint64_t file_address_space = 0x10000000; // 256M * 4 = 1GB

				uint64_t file_top_address = application_base_address + file_address_space * file_index;
				uint64_t file_bottom_address = file_top_address - tags_section_size;

				virtual_base_address = file_bottom_address;

				compile_tags();
				compile_tag_instances();
				compile_global_tag_instances();
				compile_interops();

				tags_header.checksum = 'poop';
				tags_header.tags_signature = 'tags';

				tags_header.tag_instances.count = tag_instance_count;
				tags_header.tag_instances.address = get_tag_section_virtual_address(tags_section_tag_instances_offset);

				tags_header.tag_groups.count = tag_group_count;
				tags_header.tag_groups.address = get_tag_section_virtual_address(tags_section_tag_groups_offset);

				tags_header.tag_global_instance.count = tag_global_entries_count;
				tags_header.tag_global_instance.address = get_tag_section_virtual_address(tags_section_tag_global_instances_offset);

				tags_header.tag_interop_table.count = tag_api_interops_count;
				tags_header.tag_interop_table.address = get_tag_section_virtual_address(tags_section_api_interop_fixups_offset);

				section_data[section_index].insert(section_data[section_index].end(), tag_data_buffer, tag_data_buffer + tag_data_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tags_header_buffer, tags_header_buffer + tags_header_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tag_groups_buffer, tag_groups_buffer + tag_groups_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tag_instances_buffer, tag_instances_buffer + tag_instances_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tag_global_entries_buffer, tag_global_entries_buffer + tag_global_entries_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tag_api_interop_fixups_buffer, tag_api_interop_fixups_buffer + tag_api_interop_fixups_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tag_api_interops_buffer, tag_api_interops_buffer + tag_api_interops_buffer_size);

				const char* debug_tags_section_tag_data_offset = section_data[section_index].data() + tags_section_tag_data_offset - tags_section_begin_file_offset;
				const char* debug_tags_section_tags_header_offset = section_data[section_index].data() + tags_section_tags_header_offset - tags_section_begin_file_offset;
				const char* debug_tags_section_tag_groups_offset = section_data[section_index].data() + tags_section_tag_groups_offset - tags_section_begin_file_offset;
				const char* debug_tags_section_tag_instances_offset = section_data[section_index].data() + tags_section_tag_instances_offset - tags_section_begin_file_offset;
				const char* debug_tags_section_tag_global_instances_offset = section_data[section_index].data() + tags_section_tag_global_instances_offset - tags_section_begin_file_offset;
				const char* debug_tags_section_api_interops_offset = section_data[section_index].data() + tags_section_api_interop_fixups_offset - tags_section_begin_file_offset;

				s_tag_data_entry& tag_data_entry = tag_data_entries[4];
				uint32_t tag_page_offset = encode_page_offset(tag_data_entry.tag_data_offset + DEBUG_PADDING);
				uint64_t tag_virtual_address = decode_page_offset(tag_page_offset);
				const char* tag_data = debug_tags_section_tag_data_offset + tag_virtual_address;
				uint32_t tag_resources_page_offset = *reinterpret_cast<const uint32_t*>(tag_data + 0x68);
				uint64_t tag_resources_virtual_address = decode_page_offset(tag_resources_page_offset);
				const char* tag_resources_data = debug_tags_section_tag_data_offset + tag_resources_virtual_address;

				// #TODO: build these correctly
				// tag_post_link_buffer
				// tag_language_dependent_read_only_buffer
				// tag_language_dependent_read_write_buffer
				// tag_language_neutral_read_write_buffer
				// tag_language_neutral_write_combined_buffer
				// tag_language_neutral_read_only_buffer

				cache_file_metadata.tag_post_link_buffer.size = static_cast<uint32_t>(section_data[section_index].size());
				//cache_file_metadata.tag_language_dependent_read_only_buffer.size = static_cast<uint32_t>(section_data[section_index].size());
				//cache_file_metadata.tag_language_dependent_read_write_buffer.size = static_cast<uint32_t>(section_data[section_index].size());
				//cache_file_metadata.tag_language_neutral_read_write_buffer.size = static_cast<uint32_t>(section_data[section_index].size());
				////cache_file_metadata.tag_language_neutral_write_combined_buffer.size = static_cast<uint32_t>(section_data[section_index].size());
				//cache_file_metadata.tag_language_neutral_read_only_buffer.size = static_cast<uint32_t>(section_data[section_index].size());
				cache_file_metadata.tag_post_link_buffer.elements = get_tag_section_virtual_address(tags_section_begin_file_offset);
				//cache_file_metadata.tag_language_dependent_read_only_buffer.elements = get_tag_section_virtual_address(tags_section_file_offset);
				//cache_file_metadata.tag_language_dependent_read_write_buffer.elements = get_tag_section_virtual_address(tags_section_file_offset);
				//cache_file_metadata.tag_language_neutral_read_write_buffer.elements = get_tag_section_virtual_address(tags_section_file_offset);
				////cache_file_metadata.tag_language_neutral_write_combined_buffer.elements = get_tag_section_virtual_address(tags_section_file_offset);
				//cache_file_metadata.tag_language_neutral_read_only_buffer.elements = get_tag_section_virtual_address(tags_section_file_offset);

				cache_file_metadata.tag_buffer_offset = tags_section_tag_data_offset;
				cache_file_metadata.tag_buffer_size = static_cast<uint32_t>(section_data[section_index].size());

				break;
			}
			case gen3::_cache_file_section_index_debug:
			{
				compile_string_ids();

				debug_section_file_table_offset = total_cache_file_data;
				debug_section_file_table_indices_offset = debug_section_file_table_offset + tag_file_table_buffer_size;
				debug_section_string_ids_offset = debug_section_file_table_indices_offset + tag_file_table_indices_buffer_size;
				debug_section_string_id_indices_offset = debug_section_string_ids_offset + string_ids_buffer_size;
				debug_section_string_id_namespaces_offset = debug_section_string_id_indices_offset + string_ids_indices_buffer_size;
				uint32_t debug_section_unused_next_offset = debug_section_string_id_namespaces_offset + string_id_namespaces_buffer_size;

				cache_file_metadata.file_count = tag_file_table_indices_count;
				cache_file_metadata.file_table_offset = debug_section_file_table_offset;
				cache_file_metadata.file_table_length = tag_file_table_data_size;
				cache_file_metadata.file_table_indices_offset = debug_section_file_table_indices_offset;

				cache_file_metadata.string_count = string_ids_count;
				cache_file_metadata.string_table_length = string_ids_data_size;
				cache_file_metadata.string_table_indices_offset = debug_section_string_id_indices_offset;
				cache_file_metadata.string_table_offset = debug_section_string_ids_offset;

				cache_file_metadata.string_ids_namespace_table_count = string_id_namespace_count;
				cache_file_metadata.string_ids_namespace_table_offset = debug_section_string_id_namespaces_offset;

				section_data[section_index].insert(section_data[section_index].end(), tag_file_table_buffer, tag_file_table_buffer + tag_file_table_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), tag_file_table_indices_buffer, tag_file_table_indices_buffer + tag_file_table_indices_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), string_ids_buffer, string_ids_buffer + string_ids_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), string_ids_indices_buffer, string_ids_indices_buffer + string_ids_indices_buffer_size);
				section_data[section_index].insert(section_data[section_index].end(), string_id_namespaces_buffer, string_id_namespaces_buffer + string_id_namespaces_buffer_size);

				break;
			}
			}

			uint32_t section_size = static_cast<uint32_t>(section_data[section_index].size());

			section_infos[section_index].section_file_offset = total_cache_file_data;
			section_infos[section_index].section_offset_from_header = total_cache_file_data;
			section_infos[section_index].section_size = section_size;

			total_section_data += section_size;
			total_cache_file_data += section_size;

			// #TODO: is this controlled by the cache file flags somehow?
			if (section_index != gen3::_cache_file_section_index_debug) // debug section is not loaded by game
			{
				loaded_section_data += section_size;
			}
		}
	}

	{
		memset(&cache_file_metadata._padding, 0, sizeof(cache_file_metadata._padding));
		cache_file_metadata.footer = k_cache_footer_signature;
	}
	{
		cache_file_metadata.scenario_type = _scenario_type_multiplayer;

		//time_t now = time(NULL);
		//struct tm* t = localtime(&now);
		//strftime(info.build, sizeof(info.build), "Mandrill - %b %d %Y %T", t);
		strcpy(cache_file_metadata.build, "Oct 15 2020 18:23:50");

		c_fixed_path name = scenario->tag_filename;
		PathRemoveExtensionA(name.data);
		c_fixed_path scenario_path = scenario->tag_filepath;
		PathRemoveExtensionA(scenario_path.data);
		snprintf(cache_file_metadata.name, sizeof(cache_file_metadata.name), "%s", name.c_str());
		snprintf(cache_file_metadata.scenario_path, sizeof(cache_file_metadata.scenario_path), "%s", scenario_path.c_str());

		cache_file_metadata.flags = static_cast<e_cache_file_flags>(4);
		cache_file_metadata.virtual_base_address = virtual_base_address; // taken from 20_sword_slayer

		cache_file_metadata.tags_header_address = get_tag_section_virtual_address(tags_section_tags_header_offset);
	}
	{
		cache_file_metadata.header_signature = k_cache_header_signature;
		cache_file_metadata.file_version = 13;
		cache_file_metadata.file_length = total_cache_file_data;
		cache_file_metadata.file_compressed_length = 0;
		cache_file_metadata.__unknown0 = 1;
		cache_file_metadata.__unknown1 = 1;
		cache_file_metadata.__unknown2 = 4;

		for (uint32_t i = 0; i < gen3::k_number_of_cache_file_sections; i++)
		{
			cache_file_metadata.file_length += cache_file_metadata.section_table.sections[i].size;
		}

		for (uint32_t i = 0; i < gen3::k_number_of_cache_file_sections; i++)
		{
			s_section_info& section_info = section_infos[i];

			cache_file_metadata.section_table.sections[i].offset = section_info.section_offset_from_header;
			cache_file_metadata.section_table.sections[i].size = section_info.section_size;
			cache_file_metadata.section_table.offset_masks[i] = 0;
		}
	}

	FILE* file = _wfopen(filepath, L"wb");
	//if (file)
	//{
	write(&cache_file_metadata, sizeof(cache_file_metadata), file);
	for (uint32_t section_indirection_index = 0; section_indirection_index < gen3::k_number_of_cache_file_sections; section_indirection_index++)
	{
		gen3::e_cache_file_section_index section_index = cache_file_section_indirection[section_indirection_index];
		write(section_data[section_index].data(), section_data[section_index].size(), file);
	}
	fclose(file);
	//}
	//else
	//{
	//	c_console::write_line("FAILED TO OUTPUT CUSTOM MAP!!!!!");
	//}
}

uint32_t c_haloreach_cache_compiler::get_tag_pointer_relative_offset(const char* tag_data)
{
	uint32_t relative_offset = static_cast<uint32_t>(tag_data - tag_data_buffer);
	return relative_offset;
}

uint32_t c_haloreach_cache_compiler::encode_page_offset(uint64_t offset)
{
	DEBUG_ASSERT(virtual_base_address != 0);
	uint32_t page_offset = static_cast<uint32_t>(((offset)+(virtual_base_address - page_address_offset_relative)) / 4ull);
	DEBUG_ASSERT((page_offset & 0xF0000000) != 0xF0000000); // internally used for custom address schema
	return page_offset;
}

uint64_t c_haloreach_cache_compiler::decode_page_offset(uint32_t page_offset)
{
	DEBUG_ASSERT(virtual_base_address != 0);
	DEBUG_ASSERT((page_offset & 0xF0000000) != 0xF0000000); // internally used for custom address schema
	return (static_cast<uint64_t>(page_offset) * 4ull) - (virtual_base_address - page_address_offset_relative);
}
