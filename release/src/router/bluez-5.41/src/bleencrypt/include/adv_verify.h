/*
**	adv_verify.h
**
**
*/
#ifndef ADVVERIFYh
#define ADVVERIFYh
//---------------------------------------------------------------------------
static const unsigned long CRC32_Table[256] = 
{
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

/*
static unsigned long Adv_CRC32(
	unsigned long val, 
	const void *ss, 
	int len)
{
	const unsigned char *s = (unsigned char *)ss;
    	while (--len >= 0)
        	val = CRC32_Table[(val ^ *s++) & 0xff] ^ (val >> 8);
        return val;
}
*/
//---------------------------------------------------------------------------
/*
static unsigned long Adv_CheckSum(
	void *buf, 
	unsigned long len, 
	unsigned long seed)
{
	unsigned long i;
	const char *pBuf = (char *)buf;
	
	for (i=0; i<len; ++i) 
		seed += (unsigned int)(*pBuf++);
	
	return seed;
}
*/

static unsigned short Adv_CRC16(const unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^ ((unsigned short)(x <<5)) ^ ((unsigned short)x);
    }
    return crc;
}

//---------------------------------------------------------------------------
// MD5 	
//#ifndef uint32_t
//typedef unsigned long uint32_t;
//#endif	/* uint32_t */

typedef struct MD5Context {
	unsigned long	buf[4];
	unsigned long	bits[2];
	unsigned char	in[64];
} MD5_CTX;

#ifndef HAVE_MD5
#if __BYTE_ORDER == 1234
#define __byteReverse(buf, len)	/* Nothing */
#else	/* HAVE_MD5 */
/*
 * Note: this code is harmless on little-endian machines.
 */
#ifndef RTCONFIG_LANTIQ
static void __byteReverse(unsigned char *buf, unsigned longs)
{
	unsigned long t;
	do {
		t = (unsigned long) ((unsigned) buf[3] << 8 | buf[2]) << 16 |
			((unsigned) buf[1] << 8 | buf[0]);
		*(unsigned long*) buf = t;
		buf += 4;
	} while (--longs);
}
#endif
#endif /* __BYTE_ORDER */

/* The four core functions - F1 is optimized somewhat */

/* #define F1(x, y, z) (x & y | ~x & z) */
#define __MD5_F1(x, y, z) (z ^ (x & (y ^ z)))
#define __MD5_F2(x, y, z) __MD5_F1(z, x, y)
#define __MD5_F3(x, y, z) (x ^ y ^ z)
#define __MD5_F4(x, y, z) (y ^ (x | ~z))

/* This is the central step in the MD5 algorithm. */
#define __MD5STEP(f, w, x, y, z, data, s) \
( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

/*
 * Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious
 * initialization constants.
 */
/*
static void AdvMD5_Init(
	MD5_CTX *ctx)
{
	ctx->buf[0] = 0x67452301;
	ctx->buf[1] = 0xefcdab89;
	ctx->buf[2] = 0x98badcfe;
	ctx->buf[3] = 0x10325476;

	ctx->bits[0] = 0;
	ctx->bits[1] = 0;
	return;
}
*/

/*
 * The core of the MD5 algorithm, this alters an existing MD5 hash to
 * reflect the addition of 16 longwords of new data.  MD5Update blocks
 * the data and converts bytes into longwords for this routine.
 */
/*
static void __MD5Transform(
	unsigned long buf[4], 
	unsigned long const in[16])
{
	register unsigned long a, b, c, d;

	a = buf[0];
	b = buf[1];
	c = buf[2];
	d = buf[3];

	__MD5STEP(__MD5_F1, a, b, c, d, in[0] + 0xd76aa478, 7);
	__MD5STEP(__MD5_F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
	__MD5STEP(__MD5_F1, c, d, a, b, in[2] + 0x242070db, 17);
	__MD5STEP(__MD5_F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
	__MD5STEP(__MD5_F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
	__MD5STEP(__MD5_F1, d, a, b, c, in[5] + 0x4787c62a, 12);
	__MD5STEP(__MD5_F1, c, d, a, b, in[6] + 0xa8304613, 17);
	__MD5STEP(__MD5_F1, b, c, d, a, in[7] + 0xfd469501, 22);
	__MD5STEP(__MD5_F1, a, b, c, d, in[8] + 0x698098d8, 7);
	__MD5STEP(__MD5_F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
	__MD5STEP(__MD5_F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
	__MD5STEP(__MD5_F1, b, c, d, a, in[11] + 0x895cd7be, 22);
	__MD5STEP(__MD5_F1, a, b, c, d, in[12] + 0x6b901122, 7);
	__MD5STEP(__MD5_F1, d, a, b, c, in[13] + 0xfd987193, 12);
	__MD5STEP(__MD5_F1, c, d, a, b, in[14] + 0xa679438e, 17);
	__MD5STEP(__MD5_F1, b, c, d, a, in[15] + 0x49b40821, 22);

	__MD5STEP(__MD5_F2, a, b, c, d, in[1] + 0xf61e2562, 5);
	__MD5STEP(__MD5_F2, d, a, b, c, in[6] + 0xc040b340, 9);
	__MD5STEP(__MD5_F2, c, d, a, b, in[11] + 0x265e5a51, 14);
	__MD5STEP(__MD5_F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
	__MD5STEP(__MD5_F2, a, b, c, d, in[5] + 0xd62f105d, 5);
	__MD5STEP(__MD5_F2, d, a, b, c, in[10] + 0x02441453, 9);
	__MD5STEP(__MD5_F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
	__MD5STEP(__MD5_F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
	__MD5STEP(__MD5_F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
	__MD5STEP(__MD5_F2, d, a, b, c, in[14] + 0xc33707d6, 9);
	__MD5STEP(__MD5_F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
	__MD5STEP(__MD5_F2, b, c, d, a, in[8] + 0x455a14ed, 20);
	__MD5STEP(__MD5_F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
	__MD5STEP(__MD5_F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
	__MD5STEP(__MD5_F2, c, d, a, b, in[7] + 0x676f02d9, 14);
	__MD5STEP(__MD5_F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

	__MD5STEP(__MD5_F3, a, b, c, d, in[5] + 0xfffa3942, 4);
	__MD5STEP(__MD5_F3, d, a, b, c, in[8] + 0x8771f681, 11);
	__MD5STEP(__MD5_F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
	__MD5STEP(__MD5_F3, b, c, d, a, in[14] + 0xfde5380c, 23);
	__MD5STEP(__MD5_F3, a, b, c, d, in[1] + 0xa4beea44, 4);
	__MD5STEP(__MD5_F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
	__MD5STEP(__MD5_F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
	__MD5STEP(__MD5_F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
	__MD5STEP(__MD5_F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
	__MD5STEP(__MD5_F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
	__MD5STEP(__MD5_F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
	__MD5STEP(__MD5_F3, b, c, d, a, in[6] + 0x04881d05, 23);
	__MD5STEP(__MD5_F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
	__MD5STEP(__MD5_F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
	__MD5STEP(__MD5_F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
	__MD5STEP(__MD5_F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

	__MD5STEP(__MD5_F4, a, b, c, d, in[0] + 0xf4292244, 6);
	__MD5STEP(__MD5_F4, d, a, b, c, in[7] + 0x432aff97, 10);
	__MD5STEP(__MD5_F4, c, d, a, b, in[14] + 0xab9423a7, 15);
	__MD5STEP(__MD5_F4, b, c, d, a, in[5] + 0xfc93a039, 21);
	__MD5STEP(__MD5_F4, a, b, c, d, in[12] + 0x655b59c3, 6);
	__MD5STEP(__MD5_F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
	__MD5STEP(__MD5_F4, c, d, a, b, in[10] + 0xffeff47d, 15);
	__MD5STEP(__MD5_F4, b, c, d, a, in[1] + 0x85845dd1, 21);
	__MD5STEP(__MD5_F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
	__MD5STEP(__MD5_F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
	__MD5STEP(__MD5_F4, c, d, a, b, in[6] + 0xa3014314, 15);
	__MD5STEP(__MD5_F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
	__MD5STEP(__MD5_F4, a, b, c, d, in[4] + 0xf7537e82, 6);
	__MD5STEP(__MD5_F4, d, a, b, c, in[11] + 0xbd3af235, 10);
	__MD5STEP(__MD5_F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
	__MD5STEP(__MD5_F4, b, c, d, a, in[9] + 0xeb86d391, 21);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
	return;
}
*/

/*
 * Update context to reflect the concatenation of another buffer full
 * of bytes.
 */
/*
static void AdvMD5_Update(
	MD5_CTX *ctx, 
	unsigned char const *buf, 
	unsigned len)
{
	unsigned long t;

	// Update bitcount

	t = ctx->bits[0];
	if ((ctx->bits[0] = t + ((unsigned long) len << 3)) < t)
		ctx->bits[1]++;		// Carry from low to high
	ctx->bits[1] += len >> 29;

	t = (t >> 3) & 0x3f;	// Bytes already in shsInfo->data

	// Handle any leading odd-sized chunks

	if (t) {
		unsigned char *p = (unsigned char *) ctx->in + t;

		t = 64 - t;
		if (len < t) {
			memcpy(p, buf, len);
			return;
		}
		memcpy(p, buf, t);
		__byteReverse(ctx->in, 16);
		__MD5Transform(ctx->buf, (unsigned long*) ctx->in);
		buf += t;
		len -= t;
	}
	// Process data in 64-byte chunks

	while (len >= 64) {
		memcpy(ctx->in, buf, 64);
		__byteReverse(ctx->in, 16);
		__MD5Transform(ctx->buf, (unsigned long*) ctx->in);
		buf += 64;
		len -= 64;
	}

	// Handle any remaining bytes of data.

	memcpy(ctx->in, buf, len);
	return;
}
*/

/*
 * Final wrapup - pad to 64-byte boundary with the bit pattern 
 * 1 0* (64-bit count of bits processed, MSB-first)
 */
/*
static void AdvMD5_Final(
	unsigned char digest[16], 
	MD5_CTX *ctx)
{
	unsigned count;
	unsigned char *p;

	// Compute number of bytes mod 64
	count = (ctx->bits[0] >> 3) & 0x3F;

	// Set the first char of padding to 0x80.  This is safe since there is
	   always at least one byte free
	p = ctx->in + count;
	*p++ = 0x80;

	// Bytes of padding needed to make 64 bytes
	count = 64 - 1 - count;

	// Pad out to 56 mod 64
	if (count < 8) {
		// Two lots of padding:  Pad the first block to 64 bytes
		memset(p, 0, count);
		__byteReverse(ctx->in, 16);
		__MD5Transform(ctx->buf, (unsigned long *) ctx->in);

		// Now fill the next block with 56 bytes
		memset(ctx->in, 0, 56);
	} else {
		// Pad block to 56 bytes
		memset(p, 0, count - 8);
	}
	__byteReverse(ctx->in, 14);

	// Append length in bits and transform
	((unsigned long*) ctx->in)[14] = ctx->bits[0];
	((unsigned long*) ctx->in)[15] = ctx->bits[1];

	__MD5Transform(ctx->buf, (unsigned long *) ctx->in);
	__byteReverse((unsigned char *) ctx->buf, 4);
	memcpy(digest, ctx->buf, 16);
	memset((char *) ctx, 0, sizeof(ctx));	// In case it's sensitive
	return;
}
*/
#endif /* !HAVE_MD5 */

#endif	/* ADVVERIFYh */
