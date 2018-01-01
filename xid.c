#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "xid.h"

/* Global ID Counter */
static uint32_t id_counter;
static pid_t pid;
static unsigned char hostname[3];

/* Swap Endianness */
static inline uint32_t
swap_endian(uint32_t val) {
	return (val<<24) | ((val<<8) & 0x00ff0000) | ((val>>8) & 0x0000ff00) | (val>>24);
}

extern void
xid_init() {
	char name[XID_HOSTNAME_MAX];
	unsigned char * name_md5;

	name_md5 = malloc(MD5_DIGEST_LENGTH);
	id_counter = arc4random();

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