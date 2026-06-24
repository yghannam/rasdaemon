// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>
#include <string.h>

#include "vendor.h"

enum amd_location_field_index {
	AMD_ChipSelect,
	AMD_BankGroup,
	AMD_Bank,
	AMD_Row,
	AMD_RankMul,
	AMD_SubChannel,
	/* Fields above identify a DRAM row; Column does not. */
	AMD_Column,
	AMD_FIELD_NUM
};

static const struct memory_location_field amd_fields[] = {
	[AMD_ChipSelect] = {
		.name = "ChipSelect",
		.anchor_str = "ChipSelect:",
		.value_base = 16
	},
	[AMD_BankGroup] = {
		.name = "BankGroup",
		.anchor_str = "BankGroup:",
		.value_base = 16
	},
	[AMD_Bank] = {
		.name = "Bank",
		.anchor_str = "Bank:",
		.value_base = 16
	},
	[AMD_Row] = {
		.name = "Row",
		.anchor_str = "Row:",
		.value_base = 16
	},
	[AMD_RankMul] = {
		.name = "RankMul",
		.anchor_str = "RankMul:",
		.value_base = 16
	},
	[AMD_SubChannel] = {
		.name = "SubChannel",
		.anchor_str = "SubChannel:",
		.value_base = 16
	},
	[AMD_Column] = {
		.name = "Column",
		.anchor_str = "Column:",
		.value_base = 16
	},
};

static bool amd_detect(const char *detail)
{
	return strstr(detail, "[AMDv1]") != NULL;
}

const struct pfa_vendor_ops pfa_amd_ops = {
	.name		= "AMD",
	.fields		= amd_fields,
	.field_num	= AMD_FIELD_NUM,
	.row_field_num	= AMD_Column,
	.detect		= amd_detect,
};
