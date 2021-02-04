#pragma once

namespace blofeld
{

namespace macaque
{

	extern s_tag_block_definition global_error_report_categories_block;
	extern s_tag_struct_definition global_error_report_categories_block_struct_definition; // global_error_report_categories_block
	extern s_tag_block_definition error_reports_block;
	extern s_tag_struct_definition error_reports_block_struct_definition; // error_reports_block
	extern s_tag_block_definition error_report_vertices_block;
	extern s_tag_struct_definition error_report_vertices_block_struct_definition; // error_report_vertices_block
	extern s_tag_block_definition error_report_vectors_block;
	extern s_tag_struct_definition error_report_vectors_block_struct_definition; // error_report_vectors_block
	extern s_tag_block_definition error_report_lines_block;
	extern s_tag_struct_definition error_report_lines_block_struct_definition; // error_report_lines_block
	extern s_tag_block_definition error_report_triangles_block;
	extern s_tag_struct_definition error_report_triangles_block_struct_definition; // error_report_triangles_block
	extern s_tag_block_definition error_report_quads_block;
	extern s_tag_struct_definition error_report_quads_block_struct_definition; // error_report_quads_block
	extern s_tag_block_definition error_report_comments_block;
	extern s_tag_struct_definition error_report_comments_block_struct_definition; // error_report_comments_block

	extern s_tag_struct_definition error_point_node_index_array_struct_definition;
	extern s_tag_array_definition error_point_node_index_array;
	extern s_tag_struct_definition error_point_node_weight_array_struct_definition;
	extern s_tag_array_definition error_point_node_weight_array;
	extern s_tag_struct_definition error_report_line_point_array_struct_definition;
	extern s_tag_array_definition error_report_line_point_array;
	extern s_tag_struct_definition error_report_triangle_point_array_struct_definition;
	extern s_tag_array_definition error_report_triangle_point_array;
	extern s_tag_struct_definition error_report_quad_point_array_struct_definition;
	extern s_tag_array_definition error_report_quad_point_array;
	extern s_tag_struct_definition error_report_point_definition;

	extern c_versioned_string_list error_report_types_strings;
	extern s_string_list_definition error_report_types;
	extern c_versioned_string_list error_report_source_strings;
	extern s_string_list_definition error_report_source;
	extern c_versioned_string_list error_report_flags_strings;
	extern s_string_list_definition error_report_flags;

} // namespace macaque

} // namespace blofeld
