#include <tagdefinitions-private-pch.h>
#include <blofeld_field_type_override.h>

namespace blofeld
{

	V5_TAG_GROUP_FROM_BLOCK(medal_challenge_aggregator_list, MEDAL_CHALLENGE_AGGREGATOR_LIST_TAG, medal_challenge_aggregator_list_block_block );

	V5_TAG_GROUP_FROM_BLOCK(medal_commendation_aggregator_list, MEDAL_COMMENDATION_AGGREGATOR_LIST_TAG, medal_commendation_aggregator_list_block_block );

	V5_TAG_BLOCK(medalAggregatorEntry, MedalAggregatorEntry::k_medalAggregatorEntryMaxCount)
	{
		{ _field_legacy, _field_string_id, "medal name^" },
		{ _field_legacy, _field_terminator }
	};

	V5_TAG_BLOCK(medalChallengeAggregator, MedalChallengeAggregator::k_medalChallengeAggregatorMaxCount)
	{
		{ _field_legacy, _field_string_id, "challenge to increment^" },
		{ _field_legacy, _field_struct, "medals", &medalAggregator_struct_definition },
		{ _field_legacy, _field_terminator }
	};

	V5_TAG_BLOCK_FROM_STRUCT(medal_challenge_aggregator_list_block, 1, medal_challenge_aggregator_list_struct_definition_struct_definition );

	V5_TAG_BLOCK(medalCommendationAggregator, MedalCommendationAggregator::k_medalCommendationAggregatorMaxCount)
	{
		{ _field_legacy, _field_string_id, "commendation to award^" },
		{ _field_legacy, _field_struct, "medals", &medalAggregator_struct_definition },
		{ _field_legacy, _field_terminator }
	};

	V5_TAG_BLOCK_FROM_STRUCT(medal_commendation_aggregator_list_block, 1, medal_commendation_aggregator_list_struct_definition_struct_definition );

	V5_TAG_STRUCT(medal_challenge_aggregator_list_struct_definition)
	{
		{ _field_legacy, _field_block, "lists", &medalChallengeAggregator_block },
		{ _field_legacy, _field_terminator }
	};

	V5_TAG_STRUCT(medalAggregator)
	{
		{ _field_legacy, _field_string_id, "display name^" },
		{ _field_legacy, _field_custom, "allowed game modes" },
		{ _field_legacy, _field_struct, "allowed game modes", &game_mode_flags_struct_struct_definition },
		{ _field_legacy, _field_custom },
		{ _field_legacy, _field_block, "contributing medals", &medalAggregatorEntry_block },
		{ _field_legacy, _field_terminator }
	};

	V5_TAG_STRUCT(medal_commendation_aggregator_list_struct_definition)
	{
		{ _field_legacy, _field_block, "lists", &medalCommendationAggregator_block },
		{ _field_legacy, _field_terminator }
	};

} // namespace blofeld

