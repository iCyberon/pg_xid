# pg_xid - globally Unique ID Generator for PostgreSQL

pg_xid is a globally unique ID generator extension for PostgreSQL.

It is using Mongo Object ID algorithm to generate globally unique ids:
https://docs.mongodb.org/manual/reference/object-id/

- 4-byte value representing the seconds since the Unix epoch,
- 3-byte machine identifier,
- 2-byte process id, and
- 3-byte counter, starting with a random value.

The binary representation of the id is compatible with Mongo 12 bytes Object IDs.

UUIDs are 16 bytes (128 bits) and 36 chars as string representation. Twitter Snowflake
ids are 8 bytes (64 bits) but require machine/data-center configuration and/or central
generator servers. xid stands in between with 12 bytes (96 bits). No configuration or central generator server
is required so it can be used directly.

| Name        | Binary Size | Features
|-------------|-------------|----------------
| [UUID]      | 16 bytes    | configuration free, not sortable
| [shortuuid] | 16 bytes    | configuration free, not sortable
| [Snowflake] | 8 bytes     | needs machin/DC configuration, needs central server, sortable
| [MongoID]   | 12 bytes    | configuration free, sortable
| xid         | 12 bytes    | configuration free, sortable

[UUID]: https://en.wikipedia.org/wiki/Universally_unique_identifier
[shortuuid]: https://github.com/stochastic-technologies/shortuuid
[Snowflake]: https://blog.twitter.com/2010/announcing-snowflake
[MongoID]: https://docs.mongodb.org/manual/reference/object-id/

Features:

- Size: 12 bytes (96 bits), smaller than UUID, larger than snowflake
- Non configured, you don't need set a unique machine and/or data center id
- K-ordered
- Embedded time with 1 second precision
- Unicity guaranteed for 16,777,216 (24 bits) unique ids per second and per host/process
- Lock-free (i.e.: unlike UUIDv1 and v2)

References:

- https://github.com/rs/xid
- http://www.slideshare.net/davegardnerisme/unique-id-generation-in-distributed-systems
- https://en.wikipedia.org/wiki/Universally_unique_identifier
- https://blog.twitter.com/2010/announcing-snowflake
- Python port by [Graham Abbott](https://github.com/graham): https://github.com/graham/python_xid

Inspired by [Olivier Poitrey](https://github.com/rs)'s Xid.

## Build

    make
    make install
    
## Install

     CREATE EXTENSION pg_xid;

## Usage

```SQL
SELECT xid(); -- returns: BYTEA

SELECT encode(xid(),'hex'); -- returns: TEXT
```

## Licenses

All source code is licensed under the [MIT License](https://raw.github.com/icyberon/pg_xid/master/LICENSE).
