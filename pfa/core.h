/* SPDX-License-Identifier: GPL-2.0-or-later */

/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

#ifndef __PFA_CORE_H
#define __PFA_CORE_H

#include <sys/types.h>
#include <sys/queue.h>
#include <stdbool.h>
#include <time.h>

#include "rbtree.h"
#include "types.h"

#define PAGE_SHIFT		12
#define PAGE_SIZE		BIT(PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE - 1))

struct config {
	char			*name;
	unsigned long		val;
};

enum otype {
	OFFLINE_OFF,
	OFFLINE_ACCOUNT,
	OFFLINE_SOFT,
	OFFLINE_HARD,
	OFFLINE_SOFT_THEN_HARD,
};

enum pstate {
	PAGE_ONLINE,
	PAGE_OFFLINE,
	PAGE_OFFLINE_FAILED,
};

struct page_record {
	struct rb_node		entry;
	unsigned long long	addr;
	time_t			start;
	enum pstate		offlined;
	unsigned long		count;
	unsigned long		excess;
};

struct isolation {
	char			*name;
	char			*env;
	const struct config	*units;
	unsigned long		val;
	bool			overflow;
	char			*unit;
};

struct page_addr {
	LIST_ENTRY(page_addr)	entry;
	unsigned long long	addr;
	enum pstate		offlined;
	int			count;
	time_t			start;
};

void ras_page_account_init(void);
void ras_record_page_error(unsigned long long addr,
			   unsigned int count, time_t time);
void ras_hw_threshold_pageoffline(unsigned long long addr);
void ras_row_account_init(void);
void ras_record_row_error(const char *detail, unsigned int count, time_t time,
			  unsigned long long addr);
void row_record_infos_free(void);

#endif
