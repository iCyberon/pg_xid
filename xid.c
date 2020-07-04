#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <openssl/md5.h>
#include "postgres.h"
#include "xid.h"

/* Custom Base32 */
static const char * encoding = "0123456789abcdefghijklmnopqrstuv";

/* Global ID Counter */
static uint32_t id_counter;
static pid_t pid;
static unsigned char hostname[3];

/* Swap Endianness */
static inline uint32_t
swap_endian(uint32_t val) {
	return (val<<24) | ((val<<8) & 0x00ff0000) | ((val>>8) & 0x0000ff00) | (val>>24);
}

unsigned int random_unsigned_int(void)
{
  unsigned int r;
  if (!pg_strong_random(&r, sizeof(r)))
  {
    elog(ERROR, "pg_xid: pg_strong_random failed!");
    return 0;
  }
  return r;
}

extern void
xid_init() {
	char name[XID_HOSTNAME_MAX];
	unsigned char * name_md5;

	name_md5 = malloc(MD5_DIGEST_LENGTH);
	id_counter = random_unsigned_int();

	// Hostname md5
	gethostname((char*)name, XID_HOSTNAME_MAX);
	MD5((unsigned char*)&name, XID_HOSTNAME_MAX, name_md5);
	
	memcpy(&hostname, name_md5, 3);
	free(name_md5);

	// PID
	pid = getpid();
}

/* Generate ID */
extern unsigned char * 
xid_generate() {
	unsigned char id[XID_RAW_LEN];
	unsigned char* result;

	result = malloc(sizeof(id));
	
	// Timestamp
	uint32_t t = time(NULL);
	t = swap_endian(t);
	memcpy(&id, &t, 4);
	
	// Hostname
	memcpy(&id[4], hostname, 3);
	
	// PID
	id[7] = (unsigned char)(pid >> 8);
	id[8] = (unsigned char)(pid);
	
	// Counter
	id_counter++;
	id[9] = (unsigned char)(id_counter >> 16);
	id[10] = (unsigned char)(id_counter >> 8);
	id[11] = (unsigned char)(id_counter);
	
	memcpy(result, &id, XID_RAW_LEN);

	return result;
}

/* Generate ID */
extern unsigned char * 
xid_encode(unsigned char * id) {
	unsigned char dst[XID_ENCODED_LEN];
	unsigned char * result;

	result = malloc(XID_ENCODED_LEN);

	dst[0] = encoding[ id[0]>>3 ];
	dst[1] = encoding[ ((id[1]>>6) & 0x1F) | ((id[0]<<2) & 0x1F) ];
	dst[2] = encoding[ (id[1]>>1) & 0x1F];
	dst[3] = encoding[ ((id[2]>>4) & 0x1F) | ((id[1]<<4) & 0x1F) ];
	dst[4] = encoding[ id[3]>>7 | ((id[2]<<1) & 0x1F) ];
	dst[5] = encoding[ (id[3]>>2) & 0x1F ];
	dst[6] = encoding[ id[4]>>5 | ((id[3]<<3) & 0x1F) ];
	dst[7] = encoding[ id[4] & 0x1F ];
	dst[8] = encoding[ id[5]>>3 ];
	dst[9] = encoding[ ((id[6]>>6) & 0x1F) | ((id[5]<<2) & 0x1F) ];
	dst[10] = encoding[ (id[6]>>1) & 0x1F ];
	dst[11] = encoding[ ((id[7]>>4) & 0x1F) | ((id[6]<<4) & 0x1F) ];
	dst[12] = encoding[ id[8]>>7 | ((id[7]<<1) & 0x1F) ];
	dst[13] = encoding[ (id[8]>>2) & 0x1F];
	dst[14] = encoding[ (id[9]>>5) | ((id[8]<<3) & 0x1F) ];
	dst[15] = encoding[ id[9] & 0x1F ];
	dst[16] = encoding[ id[10]>>3 ];
	dst[17] = encoding[ ((id[11]>>6) & 0x1F) | ((id[10]<<2) & 0x1F) ];
	dst[18] = encoding[ (id[11]>>1) & 0x1F ];
	dst[19] = encoding[ (id[11]<<4) & 0x1F ];

	memcpy(result, &dst, XID_ENCODED_LEN);

	return result;
}
