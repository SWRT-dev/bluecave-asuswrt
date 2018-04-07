/*
 *
 * stat_helper.h - Counter and Statistic Module header file.
 *
 */

#ifndef STAT_HELPER_H
#define STAT_HELPER_H

#define MODULE_GET      try_module_get(THIS_MODULE)
#define MODULE_PUT      module_put(THIS_MODULE)

typedef enum {
	STAT_HELPER_FAILURE = -1,
	STAT_HELPER_SUCCESS = 0,
} status;

#endif /* STAT_HELPER_H */
