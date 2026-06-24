// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>
#include <string.h>

#include "vendor.h"

enum dsm_location_field_index {
	DSM_ProcessorSocketId,
	DSM_MemoryControllerId,
	DSM_ChannelId,
	DSM_DimmSlotId,
	DSM_PhysicalRankId,
	DSM_ChipId,
	DSM_BankGroup,
	DSM_Bank,
	DSM_Row,
	DSM_FIELD_NUM
};

static const struct memory_location_field dsm_fields[] = {
	[DSM_ProcessorSocketId] = {
		.name = "ProcessorSocketId",
		.anchor_str = "ProcessorSocketId:",
		.value_base = 16
	},
	[DSM_MemoryControllerId] = {
		.name = "MemoryControllerId",
		.anchor_str = "MemoryControllerId:",
		.value_base = 16
	},
	[DSM_ChannelId] = {
		.name = "ChannelId",
		.anchor_str = "ChannelId:",
		.value_base = 16
	},
	[DSM_DimmSlotId] = {
		.name = "DimmSlotId",
		.anchor_str = "DimmSlotId:",
		.value_base = 16
	},
	[DSM_PhysicalRankId] = {
		.name = "PhysicalRankId",
		.anchor_str = "PhysicalRankId:",
		.value_base = 16
	},
	[DSM_ChipId] = {
		.name = "ChipId",
		.anchor_str = "ChipId:",
		.value_base = 16
	},
	[DSM_BankGroup] = {
		.name = "BankGroup",
		.anchor_str = "BankGroup:",
		.value_base = 16
	},
	[DSM_Bank] = {
		.name = "Bank",
		.anchor_str = "Bank:",
		.value_base = 16
	},
	[DSM_Row] = {
		.name = "Row",
		.anchor_str = "Row:",
		.value_base = 16
	},
};

static bool dsm_detect(const char *detail)
{
	return strstr(detail, "ProcessorSocketId:") != NULL;
}

const struct pfa_vendor_ops pfa_dsm_ops = {
	.name		= "DSM",
	.fields		= dsm_fields,
	.field_num	= DSM_FIELD_NUM,
	.row_field_num	= DSM_FIELD_NUM,
	.detect		= dsm_detect,
};
