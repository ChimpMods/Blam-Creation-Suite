
class s_data_reference_dynamic_ui_data
{
public:
	c_cache_file& cache_file;
	DataReference* field_data;
	MemoryEditor memory_editor;

	s_data_reference_dynamic_ui_data(DataReference* field_data, c_cache_file& cache_file) :
		cache_file(cache_file),
		field_data(field_data),
		memory_editor()
	{
		memory_editor.OptShowOptions = false;
	}
};
static constexpr size_t x = sizeof(s_data_reference_dynamic_ui_data);

void render_dataref_gui(DataReference* field_data, const c_reflection_field& reflection_field)
{
	bool unknownItemsVisible = c_mantle_gui::get_unknown_fields_visibility();
	if (!unknownItemsVisible && reflection_field.is_hidden_by_default) return; // skip hidden fields
	DEBUG_ASSERT(field_data != nullptr);
	ImGui::PushID(field_data);
	{
		ImGui::Columns(3, NULL, false);
		ImGui::SetColumnOffset(1, c_mantle_tag_gui_tab::g_current_recursion_padding);
		ImGui::SetColumnWidth(1, 400);
		ImGui::SetColumnWidth(2, 740);
		ImGui::NextColumn(); // padding

		ImGui::Text(reflection_field.nice_name);
		ImGui::NextColumn();

		static_assert(sizeof(s_data_reference_dynamic_ui_data) <= sizeof(c_mantle_tag_gui_tab::c_imgui_dynamic_data::second), "s_data_reference_dynamic_data is too large");
		bool wasAllocated;
		s_data_reference_dynamic_ui_data& data_reference_ui_data = c_mantle_tag_gui_tab::g_current_mantle_tag_tab->get_dynamic_data<s_data_reference_dynamic_ui_data>(field_data, wasAllocated);
		if (wasAllocated)
		{
			new(&data_reference_ui_data) s_data_reference_dynamic_ui_data(field_data, c_mantle_tag_gui_tab::g_current_mantle_tag_tab->get_cache_file());
		}

		ImGui::Dummy(ImVec2(0.0f, 3.0f));
		static ImVec2 const data_reference_editor_size(800, ImGui::GetTextLineHeight() * 9.5f);
		if (ImGui::BeginChild(reinterpret_cast<ImGuiID>(field_data), data_reference_editor_size, false))
		{
			char* data_reference_data = nullptr;
			if (field_data->size)
			{
				data_reference_data = data_reference_ui_data.cache_file.get_data_reference_data(*field_data);
			}
			data_reference_ui_data.memory_editor.DrawContents(data_reference_data, field_data->size);
		}
		ImGui::EndChild();
		ImGui::Dummy(ImVec2(0.0f, 3.0f));

		ImGui::Columns(1);
	}

	ImGui::PopID();
}

