#include <tagdefinitions-private-pch.h>
#include <blofeld_field_type_override.h>

namespace blofeld
{

	V5_TAG_GROUP_FROM_BLOCK(player_grade_globals_definition, PLAYER_GRADE_GLOBALS_DEFINITION_TAG, player_grade_globals_definition_block_block );

	V5_TAG_BLOCK(player_grade_definition_block, k_maxPlayerGradeCount)
	{
		{ _field_legacy, _field_long_integer, "xp threshold #total earned XP needed to reach this rank" },
		{ _field_legacy, _field_string_id, "name" },
		{ _field_legacy, _field_short_integer, "sprite index" },
		{ _field_legacy, _field_pad, "pad0", 2 },

		{ _field_legacy, _field_version_greater, _engine_type_haloreach, 2 },
		{ _field_legacy, _field_real, "time played multiplier#This is an additional multiplier to rewards given for time played for players at this grade" },
		{ _field_legacy, _field_block, "level up rewards#These rewards will be given to the player when they reach this grade", &rewardBlock_block },

		{ _field_legacy, _field_terminator }
	};

	V5_TAG_BLOCK_FROM_STRUCT(player_grade_globals_definition_block, 1, player_grade_globals_definition_struct_definition_struct_definition );

	V5_TAG_BLOCK(player_rank_definition_block, k_maxPlayerGradeCount)
	{
		{ _field_legacy, _field_block, "player grades", & player_grade_definition_block_block },

		{ _field_legacy, _field_terminator }
	};

	V5_TAG_STRUCT(player_grade_globals_definition_struct_definition)
	{
		{ _field_legacy, _field_version_less_or_equal, _engine_type_haloreach, 1 },
		{ _field_legacy, _field_block, "player ranks", &player_rank_definition_block_block },

		{ _field_legacy, _field_version_greater, _engine_type_haloreach, 1 },
		{ _field_legacy, _field_block, "player grades", &player_grade_definition_block_block },

		{ _field_legacy, _field_terminator }
	};

} // namespace blofeld

