MODULE_big = pg_xid
OBJS       = pg_xid.o xid.o

EXTENSION = pg_xid
DATA = pg_xid--1.0.sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)