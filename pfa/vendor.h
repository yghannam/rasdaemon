/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef __PFA_VENDOR_H
#define __PFA_VENDOR_H

#include <stdbool.h>

struct memory_location_field {
	const char	*name;
	const char	*anchor_str;
	const int	value_base;
};

struct pfa_vendor_ops {
	const char			*name;
	const struct memory_location_field	*fields;
	int				field_num;
	int				row_field_num;
	bool				(*detect)(const char *detail);
	int				(*gather_pages)(const unsigned long long *location_fields,
							unsigned long long *addr_list,
							int max);
};

#define PFA_MAX_FIELDS	9

extern const struct pfa_vendor_ops pfa_ghes_ops;
extern const struct pfa_vendor_ops pfa_amd_ops;
extern const struct pfa_vendor_ops pfa_dsm_ops;

#endif
