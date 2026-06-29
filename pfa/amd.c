// SPDX-License-Identifier: GPL-2.0-or-later

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core.h"
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
	AMD_SocketId,
	AMD_IPID,
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
	[AMD_SocketId] = {
		.name = "SocketId",
		.anchor_str = "SocketId:",
		.value_base = 16
	},
	[AMD_IPID] = {
		.name = "IPID",
		.anchor_str = "IPID:",
		.value_base = 16
	},
};

#define DFS_BASE "/sys/kernel/debug/amd_atl_prm/"

#define NUM_COL		(UINT16_MAX + 1)

static int debugfs_write_int(const char *path, unsigned long long val)
{
	char buf[32];
	int fd, len, ret = 0;

	fd = open(path, O_WRONLY);
	if (fd < 0)
		return -errno;

	len = snprintf(buf, sizeof(buf), "%llu", val);
	if (write(fd, buf, len) != len)
		ret = -errno;

	close(fd);
	return ret;
}

static int debugfs_read_u64(const char *path, unsigned long long *val)
{
	char buf[32];
	int fd, len;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return -errno;

	len = read(fd, buf, sizeof(buf) - 1);
	close(fd);

	if (len <= 0)
		return -EIO;

	buf[len] = '\0';
	*val = strtoull(buf, NULL, 0);
	return 0;
}

static int amd_gather_pages(const unsigned long long *location_fields,
			    unsigned long long *addr_list, int max)
{
	unsigned long long sys_addr = 0;
	int count = 0;

	if (access(DFS_BASE, F_OK))
		return 0;

	if (debugfs_write_int(DFS_BASE "socket", location_fields[AMD_SocketId]) ||
	    debugfs_write_int(DFS_BASE "umc_bank_inst_id", location_fields[AMD_IPID]) ||
	    debugfs_write_int(DFS_BASE "cs", location_fields[AMD_ChipSelect]) ||
	    debugfs_write_int(DFS_BASE "bank_group", location_fields[AMD_BankGroup]) ||
	    debugfs_write_int(DFS_BASE "bank_address", location_fields[AMD_Bank]) ||
	    debugfs_write_int(DFS_BASE "row_address", location_fields[AMD_Row]) ||
	    debugfs_write_int(DFS_BASE "rank_multiplier", location_fields[AMD_RankMul]) ||
	    debugfs_write_int(DFS_BASE "sub_channel", location_fields[AMD_SubChannel]))
		return 0;

	for (int col = 0; col < NUM_COL; col++) {
		if (count >= max)
			break;

		if (debugfs_write_int(DFS_BASE "col_address", col))
			continue;

		if (debugfs_write_int(DFS_BASE "do_dram_to_sys", 1))
			continue;

		if (debugfs_read_u64(DFS_BASE "sys_addr", &sys_addr))
			continue;

		sys_addr &= PAGE_MASK;

		int found = 0;

		for (int i = 0; i < count; i++) {
			if (addr_list[i] == sys_addr) {
				found = 1;
				break;
			}
		}

		if (!found)
			addr_list[count++] = sys_addr;
	}

	return count;
}

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
	.gather_pages	= amd_gather_pages,
};
