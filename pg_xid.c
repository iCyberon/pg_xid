#include "postgres.h"
#include "fmgr.h"

#include "funcapi.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "utils/builtins.h"

#include "xid.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

extern void _PG_init(void);
extern Datum xid( PG_FUNCTION_ARGS );

/*
 * Module initialization function
 */
void
_PG_init(void) {
  xid_init();
}

/*
 * Generate ID
 */
PG_FUNCTION_INFO_V1( xid );
Datum
xid( PG_FUNCTION_ARGS )
{
  // Declaration
  bytea *result;
  unsigned char *id;

  id = xid_generate();
  result = (bytea *)palloc(XID_RAW_LEN + VARHDRSZ);

  SET_VARSIZE(result, XID_RAW_LEN + VARHDRSZ);
  memcpy(VARDATA(result), id, XID_RAW_LEN);

  free(id);
  PG_RETURN_BYTEA_P(result);
}