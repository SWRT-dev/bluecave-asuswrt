/*
**	encrypt.h 
**	
** 	RSA, AES encryption & decryption
**	 
*/
#ifndef __ENCRYPTh__
#define __ENCRYPTh__

#ifdef __WIN32__
#if _MSC_VER < 1900
#ifndef snprintf
#define snprintf _snprintf
#endif
#endif	/* _MSC_VER < 1900 */
#endif  /* __WIN32__ */

#ifdef __WIN32__
#define __func__ __FUNCTION__
#include <Winsock2.h>
#endif	/* __WIN32__ */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#ifdef __MBEDTLS__
#include <mbedtls/rsa.h>
#include <mbedtls/error.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#include <mbedtls/pk.h>
#include <mbedtls/aes.h>
#include <mbedtls/version.h>
#include <mbedtls/sha256.h>
#else	/* __MBEDTLS__ */
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#endif	/* __MBEDTLS__ */

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */ 

#ifndef __MBEDTLS__
extern RSA* rsa_create(unsigned char *key, int public);
extern RSA* rsa_create_with_file(char *filename, int public);
#endif // __MBEDTLS__
extern char *ssl_version_text(char version_text[32]);
extern size_t rsa_decrypt(unsigned char *enc_data, size_t data_len, unsigned char *key, size_t key_len, unsigned char *decrypted, size_t decrypted_size, int public);
extern size_t rsa_decrypt_with_file(unsigned char *enc_data, size_t data_len, char *filename, unsigned char *decrypted, size_t decrypted_size, int public);
extern size_t rsa_encrypt(unsigned char *data, size_t data_len, unsigned char *key, size_t key_len, unsigned char *encrypted, size_t encrypted_size, int public);
extern size_t rsa_encrypt_with_file(unsigned char *data, size_t data_len, char *filename, unsigned char *encrypted, size_t encrypted_size, int public);
extern unsigned char *aes_encrypt(unsigned char *key, unsigned char *data, size_t data_len, size_t *out_len);
extern unsigned char *aes_decrypt(unsigned char *key, unsigned char *enc_data, size_t data_len, size_t *out_len);
extern unsigned char *gen_rand(size_t *out_len);
extern unsigned char *gen_session_key(unsigned char *km, size_t km_len, unsigned char *ns, size_t ns_len, unsigned char *nc, size_t nc_len, size_t *out_len);

#ifdef __cplusplus
}
#endif	/* __cplusplus */ 
#endif	/* __ENCRYPTh__ */

