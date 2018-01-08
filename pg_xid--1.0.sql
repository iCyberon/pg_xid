-- use CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_xid" to load this file. \quit

-- v1
CREATE OR REPLACE FUNCTION xid() RETURNS BYTEA
  AS 'pg_xid', 'xid' LANGUAGE C;

CREATE OR REPLACE FUNCTION xid_encoded() RETURNS TEXT
  AS 'pg_xid', 'xid_encoded' LANGUAGE C;
