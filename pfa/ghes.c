// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdbool.h>
#include <string.h>

#include "vendor.h"

enum apei_location_field_index {
	APEI_NODE,
	APEI_CARD,
	APEI_MODULE,
	APEI_RANK,
	APEI_DEVICE,
	APEI_BANK,
	APEI_ROW,
	APEI_FIELD_NUM
};

static const struct memory_location_field apei_fields[] = {
	[APEI_NODE] = {
		.name = "node",
		.anchor_str = "node:",
		.value_base = 10
	},
	[APEI_CARD] = {
		.name = "card",
		.anchor_str = "card:",
		.value_base = 10
	},
	[APEI_MODULE] = {
		.name = "module",
		.anchor_str = "module:",
		.value_base = 10
	},
	[APEI_RANK] = {
		.name = "rank",
		.anchor_str = "rank:",
		.value_base = 10
	},
	[APEI_DEVICE] = {
		.name = "device",
		.anchor_str = "device:",
		.value_base = 10
	},
	[APEI_BANK] = {
		.name = "bank",
		.anchor_str = "bank:",
		.value_base = 10
	},
	[APEI_ROW] = {
		.name = "row",
		.anchor_str = "row:",
		.value_base = 10
	},
};

static bool ghes_detect(const char *detail)
{
	return strstr(detail, "APEI location") != NULL;
}

const struct pfa_vendor_ops pfa_ghes_ops = {
	.name		= "GHES",
	.fields		= apei_fields,
	.field_num	= APEI_FIELD_NUM,
	.row_field_num	= APEI_FIELD_NUM,
	.detect		= ghes_detect,
};
