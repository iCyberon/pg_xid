#ifndef XID_H
#define XID_H 1

#include <stdlib.h>

/* version constants */
#define XID_VERSION "1.0"
#define XID_VERSION_MAJOR 1
#define XID_VERSION_MINOR 0
#define XID_VERSION_PATCH 0

/* constants */
#define XID_HOSTNAME_MAX 30
#define XID_RAW_LEN 12

/* exported function definitions */
extern void
xid_init(void);

extern unsigned char *
xid_generate(void);

#endif