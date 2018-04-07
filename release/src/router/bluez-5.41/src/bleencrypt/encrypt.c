/*
**	encrypt.c 
**	
** 	RSA, AES encryption & decryption with OpenSSL
**	
** 
*/

#include "encrypt.h"
#ifndef __MBEDTLS__
#define	RSA_PADDING		RSA_PKCS1_PADDING 
#endif	/* !__MBEDTLS__ */
#define AES_BLOCK_SIZE	16
//---------------------------------------------------------------------------
#ifndef __MBEDTLS__
extern char *openssl_version_text(char version_text[32]);

char *openssl_version_text(
	char version_text[32])
{
#if (SSLEAY_VERSION_NUMBER >= 0x905000)
	char sub[2];
	unsigned long ssleay_value;
	
	sub[1] = '\0';
	ssleay_value = SSLeay();
	if (ssleay_value < 0x906000)
	{
		ssleay_value = SSLEAY_VERSION_NUMBER;
		sub[0] = '\0';
	}
	else 
	{
		if (ssleay_value & 0xff0)
		{
			sub[0] = (char)((ssleay_value >> 4) & 0xff) + 'a' - 1;
		}
		else
		{
			sub[0] = '\0';
		}
	}
	
	snprintf(version_text, 31, "OpenSSL/%lx.%lx.%lx%s", 
		(ssleay_value >> 28) & 0xf,
		(ssleay_value >> 20) & 0xff,
		(ssleay_value >> 12) & 0xff,
		sub);
			
#else	/* SSLEAY_VERSION_NUMBER >= 0x905000 */
	
#if (SSLEAY_VERSION_NUMBER >= 0x900000)
	snprintf(version_text, 31, "OpenSSL/%lx.%lx.%lx", 
		(SSLEAY_VERSION_NUMBER >> 28) & 0xff,
		(SSLEAY_VERSION_NUMBER >> 20) & 0xff,
		(SSLEAY_VERSION_NUMBER >> 12) & 0xf);

#else 	/* SSLEAY_VERSION_NUMBER >= 0x900000 */
    char sub[2];
    sub[1]='\0';
    if(SSLEAY_VERSION_NUMBER &0x0f) 
    {
      sub[0] = (SSLEAY_VERSION_NUMBER & 0x0f) + 'a' -1;
    }
    else
    {
      sub[0]='\0';
	}
	
    snprintf(version_text, 31, "SSL/%x.%x.%x%s",
            (SSLEAY_VERSION_NUMBER >> 12) & 0xff,
            (SSLEAY_VERSION_NUMBER >> 8) & 0xf,
            (SSLEAY_VERSION_NUMBER >> 4) & 0xf, sub);
#endif	/* SSLEAY_VERSION_NUMBER >= 0x900000 */
#endif	/* SSLEAY_VERSION_NUMBER >= 0x905000 */	

	return version_text;
}	
#endif	/* __MBEDTLS__ */
//---------------------------------------------------------------------------
char *ssl_version_text(
	char version_text[32])
{
#ifdef __MBEDTLS__
	char s[33];
	memset(s, 0, sizeof(s));
	mbedtls_version_get_string_full(s);
	snprintf(version_text, 31, "%s", s);
#else	/* __MBEDTLS__ */
	// >>> Remove by MAX 20170111
	// snprintf(version_text, 31, "%s", SSLeay_version(SSLEAY_VERSION));
	// <<< Remove by MAX 20170111
	openssl_version_text(version_text);
#endif	/* __MBEDTLS__ */	
	return version_text;
}
//---------------------------------------------------------------------------
#ifndef __MBEDTLS__
RSA* rsa_create(
	unsigned char *key, 
	int public)
{
	RSA *rsa = NULL;
	BIO *keybio = NULL;
	
	if (key == NULL)
	{
		return NULL;
	}

	if ((keybio = BIO_new_mem_buf(key, -1)) == NULL)
	{
		printf("%s(%d):Failed to BIO_new_mem_buf() !!\n", __func__, __LINE__);
		return NULL;
	}
		
	if (public == 0)
	{
		rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
	}

	BIO_free(keybio);
	return rsa;
}
#endif	/* !__MBEDTLS__ */
//---------------------------------------------------------------------------
#ifndef __MBEDTLS__
RSA* rsa_create_with_file(
	char *filename,
	int public)
{
	RSA *rsa = NULL;
	FILE *fp = NULL;
	
	if (filename == NULL || strlen(filename) <= 0)
	{
		return NULL;
	}
	
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("%s(%d):Unable to open file : %s\n", __func__, __LINE__, filename);
		return NULL;
	}
	
	rsa = RSA_new();
	
	if (public == 0)
	{
		rsa = PEM_read_RSAPrivateKey(fp, &rsa, NULL, NULL);
	}
	else
	{
		rsa = PEM_read_RSA_PUBKEY(fp, &rsa, NULL, NULL);
	}
	
	fclose(fp);
	return rsa;
}
#endif	/* !__MBEDTLS__ */
//---------------------------------------------------------------------------
size_t rsa_decrypt(
	unsigned char *enc_data, 
	size_t data_len, 
	unsigned char *key,
	size_t key_len,
	unsigned char *decrypted,
	size_t decrypted_size,
	int public)
{
#ifdef __MBEDTLS__

	int retcode = 0;
	const char *pers = "mbedtls_pk_decrypt";
	size_t len = 0;
	mbedtls_pk_context pk;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	char strerr[513];
	
	if (enc_data == NULL || data_len <= 0 || key == NULL || key_len <= 0 || decrypted == NULL || decrypted_size <= 0)
	{
		return 0;
	}
	
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	memset(strerr, 0, sizeof(strerr));
	if ((retcode = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_ctr_drbg_seed() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_decrypt_err;
	}
	
	mbedtls_pk_init(&pk);
	if (public == 1)
		retcode = mbedtls_pk_parse_public_key(&pk, key, key_len);
	else
		retcode = mbedtls_pk_parse_key(&pk, key, key_len, NULL, 0);
		
	if (retcode != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		if (public==1)
			printf("%s(%d):Failed to mbedtls_pk_parse_public_key() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		else
			printf("%s(%d):Failed to mbedtls_pk_parse_key() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);			
		goto rsa_decrypt_err;
	}
		
	if ((retcode = mbedtls_pk_decrypt(&pk, enc_data, data_len, decrypted, &len, decrypted_size, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_pk_decrypt() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_decrypt_err;
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return len;
	
rsa_decrypt_err:
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return 0;	

#else /* __MBEDTLS__ */
	
	size_t len = 0;
	RSA *rsa = NULL;

	if (enc_data == NULL || data_len <= 0 || key == NULL || decrypted == NULL)
	{
		return 0;
	}

	rsa = rsa_create(key, public);
	if (rsa == NULL)
	{
		printf("%s(%d):Failed to RSA_create() !!\n", __func__, __LINE__);	
		return 0;
	}
		
	if (public == 0)
	{
		len = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PADDING);
	}
	else
	{
		len = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, RSA_PADDING);
	}
	
	RSA_free(rsa);
	return len;

#endif	/* __MBEDTLS__ */
}
//---------------------------------------------------------------------------
size_t rsa_decrypt_with_file(
	unsigned char *enc_data, 
	size_t data_len, 
	char *filename,
	unsigned char *decrypted,
	size_t decrypted_size, 
	int public)
{
#ifdef __MBEDTLS__
	int retcode = 0;
	const char *pers = "mbedtls_pk_decrypt";
	size_t len = 0;
	mbedtls_pk_context pk;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	char strerr[513];
	
	if (enc_data == NULL || data_len <= 0 || filename == NULL || strlen(filename) <= 0 || decrypted == NULL || decrypted_size <= 0)
	{
		return 0;
	}
	
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	memset(strerr, 0, sizeof(strerr));
	if ((retcode = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_ctr_drbg_seed() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_decrypt_with_file_err;
	}
	
	mbedtls_pk_init(&pk);
	if (public == 1)
		retcode = mbedtls_pk_parse_public_keyfile(&pk, filename);
	else
		retcode = mbedtls_pk_parse_keyfile(&pk, filename, "");
		
	if (retcode != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		if (public == 1)
			printf("%s(%d):Failed to mbedtls_pk_parse_public_key() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		else
			printf("%s(%d):Failed to mbedtls_pk_parse_keyfile() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_decrypt_with_file_err;
	}
		
	if ((retcode = mbedtls_pk_decrypt(&pk, enc_data, data_len, decrypted, &len, decrypted_size, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_pk_decrypt() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_decrypt_with_file_err;
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return len;
	
rsa_decrypt_with_file_err:
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return 0;	

#else	/* __MBEDTLS__ */	

	size_t len = 0;
	RSA *rsa = NULL;
	
	if (enc_data == NULL || data_len <= 0 || filename == NULL || decrypted == NULL)
	{
		return 0;
	}
	
	rsa = rsa_create_with_file(filename, public);
	if (rsa == NULL)
	{
		printf("%s(%d):Failed to RSA_create_with_file() !!\n", __func__, __LINE__);	
		return 0;
	}
	
	if (public == 0)
	{
		len = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, RSA_PADDING);
	}
	else
	{
		len = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, RSA_PADDING);
	}
	
	RSA_free(rsa);
	return len;
	
#endif	/* __MBEDTLS__ */	
}


//---------------------------------------------------------------------------
size_t rsa_encrypt(
	unsigned char *data, 
	size_t data_len,
	unsigned char *key,
	size_t key_len,
	unsigned char *encrypted, 
	size_t encrypted_size,
	int public)
{
#ifdef __MBEDTLS__
	int retcode = 0;
	const char *pers = "mbedtls_pk_encrypt";
	size_t len = 0, max_data_len = 0;
	mbedtls_pk_context pk;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	char strerr[513];
	
	if (data == NULL || data_len <= 0 || key == NULL || key_len <= 0 || encrypted == NULL || encrypted_size <= 0)
	{
		return 0;
	}
	
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	memset(strerr, 0, sizeof(strerr));
	if ((retcode = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_ctr_drbg_seed() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_encrypt_err;
	}
	
	mbedtls_pk_init(&pk);
	if (public == 1)
		retcode = mbedtls_pk_parse_public_key(&pk, key, key_len);
	else
		retcode = mbedtls_pk_parse_key(&pk, key, key_len, NULL, 0);
		
	if (retcode != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		if (public==1)
			printf("%s(%d):Failed to mbedtls_pk_parse_public_key() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		else
			printf("%s(%d):Failed to mbedtls_pk_parse_key() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);			
		goto rsa_encrypt_err;
	}
		
	max_data_len = (mbedtls_pk_get_bitlen(&pk) / 8) - 11; /* RSA_PKCS1_PADDING modulus minus 11 bytes */
	if (data_len > max_data_len)
	{
		printf("%s(%d):data_len too long. maximum data size : %zu bytes\n", __func__, __LINE__, max_data_len);
		goto rsa_encrypt_err;
	}
	
	if ((retcode = mbedtls_pk_encrypt(&pk, data, data_len, encrypted, &len, encrypted_size, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_pk_encrypt() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_encrypt_err;
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return len;
	
rsa_encrypt_err:
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return 0;	
	
#else	/* __MBEDTLS__ */	
			
	size_t len = 0, max_data_len = 0;
	RSA *rsa = NULL;
	
		
	if (data == NULL || data_len <= 0 || key == NULL || encrypted == NULL)
	{
		return 0;
	}

	rsa = rsa_create(key, public);
	if (rsa == NULL)
	{
		printf("%s(%d):Failed to RSA_create() !!\n", __func__, __LINE__);
		return 0;
	}

	max_data_len = RSA_size(rsa) - 11;
	if (data_len > max_data_len)
	{
		printf("%s(%d):data_len too long. maximum data size : %zu bytes\n", __func__, __LINE__, max_data_len);
		RSA_free(rsa);
		return 0;
	}

	if (public == 0)
	{
		len = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PADDING);	
	}
	else
	{
		len = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PADDING);
	}

	RSA_free(rsa);
	return len;	

#endif	/* __MBEDTLS__ */
}

//---------------------------------------------------------------------------
size_t rsa_encrypt_with_file(
	unsigned char *data, 
	size_t data_len,
	char *filename,
	unsigned char *encrypted,
	size_t encrypted_size,
	int public)
{
#ifdef __MBEDTLS__	
	int retcode = 0;
	const char *pers = "mbedtls_pk_encrypt";
	size_t len = 0, max_data_len = 0;
	mbedtls_pk_context pk;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	char strerr[513];
	
	if (data == NULL || data_len <= 0 || filename == NULL || strlen(filename) <= 0 || encrypted == NULL || encrypted_size <= 0)
	{
		return 0;
	}
	
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);

	memset(strerr, 0, sizeof(strerr));
	if ((retcode = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers))) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_ctr_drbg_seed() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_encrypt_with_file_err;
	}
	
	mbedtls_pk_init(&pk);
	if (public == 1)
		retcode = mbedtls_pk_parse_public_keyfile(&pk, filename);
	else
		retcode = mbedtls_pk_parse_keyfile(&pk, filename, "");
		
	if (retcode != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_pk_parse_public_key() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_encrypt_with_file_err;
	}
	
	max_data_len = (mbedtls_pk_get_bitlen(&pk) / 8) - 11; /* RSA_PKCS1_PADDING modulus minus 11 bytes */
	if (data_len > max_data_len)
	{
		printf("%s(%d):data_len too long. maximum data size : %zu bytes\n", __func__, __LINE__, max_data_len);
		goto rsa_encrypt_with_file_err;
	}	
	
	if ((retcode = mbedtls_pk_encrypt(&pk, data, data_len, encrypted, &len, encrypted_size, mbedtls_ctr_drbg_random, &ctr_drbg)) != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_pk_encrypt() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto rsa_encrypt_with_file_err;
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return len;
	
rsa_encrypt_with_file_err:
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return 0;	
		
#else /* __MBEDTLS__ */	

	size_t len = 0, max_data_len = 0;
	RSA *rsa = NULL;
	
	if (data == NULL || data_len <= 0 || filename == NULL || encrypted == NULL)
	{
		return 0;
	}
	
	rsa = rsa_create_with_file(filename, public);
	if (rsa == NULL)
	{
		printf("%s(%d):Failed to RSA_create_with_file() !!\n", __func__, __LINE__);
		return 0;
	}
	
	max_data_len = RSA_size(rsa) - 11; /* RSA_PKCS1_PADDING modulus minus 11 bytes */
	if (data_len > max_data_len)
	{
		printf("%s(%d):data_len too long. maximum data size : %zu bytes\n", __func__, __LINE__, max_data_len);
		RSA_free(rsa);
		return 0;
	}
		
	if (public == 0)
	{
		len = RSA_private_encrypt(data_len, data, encrypted, rsa, RSA_PADDING);	
	}
	else
	{
		len = RSA_public_encrypt(data_len, data, encrypted, rsa, RSA_PADDING);
	}

	RSA_free(rsa);
	return len;	
	
#endif	/* __MBEDTLS__ */	
}

//---------------------------------------------------------------------------
unsigned char *aes_encrypt(
	unsigned char *key,
	unsigned char *data,
	size_t data_len,
	size_t *out_len)
{
#ifdef __MBEDTLS__

	char strerr[133];
	int ret = 0;
	size_t pad_len = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
	size_t alloc_size = data_len + pad_len;
	size_t len = data_len, offset_size = 0;
	unsigned char b[AES_BLOCK_SIZE], *out = NULL, *o = NULL, *s = &data[0];
	mbedtls_aes_context ctx;
	
	memset(strerr, 0, sizeof(strerr));
	mbedtls_aes_init(&ctx);
	
	out = (unsigned char *)malloc(alloc_size);
	if (out == NULL)
	{
		printf("%s(%d):Failed to malloc() !!\n", __func__, __LINE__);
		goto aes_encrypt_err;
	}
	
	ret = mbedtls_aes_setkey_enc(&ctx, key, 256);
	if (ret != 0)
	{
		mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_aes_setkey_enc() returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
		goto aes_encrypt_err;
	}
	
	memset(out, 0, alloc_size);
	o = out;

	while (len > AES_BLOCK_SIZE)
	{
		memcpy(b, s, AES_BLOCK_SIZE);
		if ((mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, b, b)) != 0)
		{
			mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
			printf("%s(%d):Failed to mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
			goto aes_encrypt_err;
		}
		memcpy(o, b, AES_BLOCK_SIZE);
		s += AES_BLOCK_SIZE;
		o += AES_BLOCK_SIZE;
		len -= AES_BLOCK_SIZE;
		offset_size += AES_BLOCK_SIZE;
	}
	
	if (len > 0)
	{
		// set up data including padding
		memcpy(b, s, len);
		memset(b + len, AES_BLOCK_SIZE - len, AES_BLOCK_SIZE - len);
		if ((mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, b, b)) != 0)
		{
			mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
			printf("%s(%d):Failed to mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
			goto aes_encrypt_err;
		}
		memcpy(o, b, AES_BLOCK_SIZE);
		o += AES_BLOCK_SIZE;
		offset_size += AES_BLOCK_SIZE;		
	}

	if (alloc_size - offset_size > 0)
	{
		// set up data including padding
		memset(b, alloc_size - offset_size, AES_BLOCK_SIZE);
		if ((mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, b, b)) != 0)
		{
			mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
			printf("%s(%d):Failed to mbedtls_aes_crypt_ecb(MBEDTLS_AES_ENCRYPT) returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
			goto aes_encrypt_err;
		}
		memcpy(o, b, AES_BLOCK_SIZE);	
	}
	
	*out_len = alloc_size;
	mbedtls_aes_free(&ctx);
	return out;
	
aes_encrypt_err:
	if (out != NULL) free(out);
	mbedtls_aes_free(&ctx);	
	return NULL;	

#else	/* __MBEDTLS__ */	
	
	EVP_CIPHER_CTX *e_ctx = EVP_CIPHER_CTX_new();
	size_t i = data_len, alloc_size = 0;
	unsigned char *s = data, *o = NULL, *out = NULL;
	size_t enc_size = 0;
	
	if (e_ctx == NULL)
	{
		printf("%s(%d):Failed to EVP_CIPHER_CTX_new()!!\n", __func__, __LINE__);
		return NULL;
	}
	
	if (!EVP_EncryptInit_ex(e_ctx, EVP_aes_256_ecb(), NULL, key, NULL))
	{
		printf("%s(%d):EVP_EncryptInit_ex()!!\n", __func__, __LINE__);
		EVP_CIPHER_CTX_free(e_ctx);
		return NULL;
	}
	
	*out_len = 0;
	alloc_size = data_len+EVP_CIPHER_CTX_block_size(e_ctx);
	out = (unsigned char *)malloc(alloc_size);
	if (out == NULL)
	{
		printf("%s(%d):Failed to malloc() !!\n", __func__, __LINE__);
		EVP_CIPHER_CTX_free(e_ctx);
		return NULL;
	}
	
	memset(out , 0, alloc_size);
	o = out;
	while (i > AES_BLOCK_SIZE)
	{
		if (!EVP_EncryptUpdate(e_ctx, o, (int*)&enc_size, s, AES_BLOCK_SIZE))
		{
			printf("%s(%d):Failed to EVP_EncryptUpdate() !!\n", __func__, __LINE__);
			free(out);
			EVP_CIPHER_CTX_free(e_ctx);
			return NULL;
		}	
		
		i -= AES_BLOCK_SIZE;	
		s += AES_BLOCK_SIZE;
		o += enc_size;
		*out_len += enc_size;
	}
	
	if (i > 0)
	{
		if (!EVP_EncryptUpdate(e_ctx, o, (int*)&enc_size, s, i))
		{
			printf("%s(%d):Failed to EVP_EncryptUpdate() !!\n", __func__, __LINE__);
			free(out);
			EVP_CIPHER_CTX_free(e_ctx);
			return NULL;
		}
		o += enc_size;
		*out_len += enc_size;
	}

	if (!EVP_EncryptFinal_ex(e_ctx, o, (int*)&enc_size))
	{
		printf("%s(%d):EVP_EncryptUpdate() !!\n", __func__, __LINE__);
		free(out);
		EVP_CIPHER_CTX_free(e_ctx);
		return NULL;
	}
	
	*out_len += enc_size;
	EVP_CIPHER_CTX_free(e_ctx);
	return out;	
	
#endif	/* __MBEDTLS__ */	
}

//---------------------------------------------------------------------------
unsigned char *aes_decrypt(
	unsigned char *key,
	unsigned char *enc_data,
	size_t data_len,
	size_t *out_len)
{
#ifdef __MBEDTLS__

	char strerr[133];
	int ret = 0;
	size_t len = data_len, alloc_size = data_len;
	mbedtls_aes_context ctx;
	unsigned char b[AES_BLOCK_SIZE], *o = NULL, *out = NULL, *s = &enc_data[0];
	
	memset(strerr, 0, sizeof(strerr));
	mbedtls_aes_init(&ctx);
	
	out = (unsigned char *)malloc(alloc_size);
	if (out == NULL)
	{
		printf("%s(%d):Failed to malloc() !!\n", __func__, __LINE__);
		goto aes_decrypt_err;
	}
	
	ret = mbedtls_aes_setkey_dec(&ctx, key, 256);
	if (ret != 0)
	{
		mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
		printf("%s(%d): Failed to aes_setkey_dec() returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
		goto aes_decrypt_err;
	}
	
	memset(out, 0, alloc_size);
	o = out;
	
	while (len > AES_BLOCK_SIZE)
	{
		memset(b, 0, sizeof(b));
		memcpy(b, s, AES_BLOCK_SIZE);
		if ((ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, b, b)) != 0)
		{
			mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
			printf("%s(%d):Failed to mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
			goto aes_decrypt_err;
		}
		memcpy(o, b, AES_BLOCK_SIZE);
		len -= AES_BLOCK_SIZE;
		s += AES_BLOCK_SIZE;
		o += AES_BLOCK_SIZE;	
	}
	
	if (len > 0)
	{
		memset(b, 0, sizeof(b));
		memcpy(b, s, len);
		if ((ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, b, b)) != 0)
		{
			mbedtls_strerror(ret, strerr, sizeof(strerr)-1);
			printf("%s(%d):Failed to mbedtls_aes_crypt_ecb(MBEDTLS_AES_DECRYPT) returned : (0x%04x)%s\n", __func__, __LINE__, -ret, strerr);
			goto aes_decrypt_err;
		}
		memcpy(o, b, AES_BLOCK_SIZE);
	}
	
	*out_len = data_len - out[alloc_size-1];
	mbedtls_aes_free(&ctx);
	return out;
	
aes_decrypt_err:
	if (out != NULL) free(out);
	mbedtls_aes_free(&ctx);
	return NULL;	
	
#else	/* __MBEDTLS__ */
	
	EVP_CIPHER_CTX *d_ctx = EVP_CIPHER_CTX_new();
	size_t i = data_len, alloc_size = 0;
	unsigned char *s = enc_data, *o = NULL, *out = NULL;
	size_t dec_size = 0;
	
	if (d_ctx == NULL)
	{
		printf("%s(%d):Failed to EVP_CIPHER_CTX_new() !!\n", __func__, __LINE__);		
		return NULL;
	}

	if (!EVP_DecryptInit_ex(d_ctx, EVP_aes_256_ecb(), NULL, key, NULL))
	{
		printf("%s(%d):Failed to EVP_DecryptInit_ex() !!\n", __func__, __LINE__);
		EVP_CIPHER_CTX_free(d_ctx);
		return NULL;
	}
	
	*out_len = 0;
	alloc_size = data_len+EVP_CIPHER_CTX_block_size(d_ctx);
	out = (unsigned char *)malloc(alloc_size);
	if (out == NULL)
	{
		printf("%s(%d):Failed to malloc() !!\n", __func__, __LINE__);
		EVP_CIPHER_CTX_free(d_ctx);
		return NULL;
	}

	memset(out, 0, alloc_size);	
	o = out;
	while (i > AES_BLOCK_SIZE)
	{
		if (!EVP_DecryptUpdate(d_ctx, o, (int*)&dec_size, s, AES_BLOCK_SIZE))
		{
			printf("%s(%d):Failed to EVP_DecryptUpdate()!!\n", __func__, __LINE__);		
			free(out);
			EVP_CIPHER_CTX_free(d_ctx);
			return NULL;
		}	
		
		i -= AES_BLOCK_SIZE;	
		s += AES_BLOCK_SIZE;
		o += dec_size;
		*out_len += dec_size;
	}

	if (i > 0)
	{
		if (!EVP_DecryptUpdate(d_ctx, o, (int*)&dec_size, s, i))
		{			
			printf("%s(%d):Failed to EVP_DecryptUpdate()!!\n", __func__, __LINE__);		
			free(out);
			EVP_CIPHER_CTX_free(d_ctx);
			return NULL;
		}
		*out_len += dec_size;
		o += dec_size;
	}

	if (!EVP_DecryptFinal_ex(d_ctx, o, (int*)&dec_size))
	{
		printf("%s(%d):Failed to EVP_DecryptFinal_ex()!!\n", __func__, __LINE__);		
		free(out);
		EVP_CIPHER_CTX_free(d_ctx);
		return NULL;
	}
	
	*out_len += dec_size;
	EVP_CIPHER_CTX_free(d_ctx);
	return out;

#endif	/* __MBEDTLS__ */	
}
		
//---------------------------------------------------------------------------
unsigned char *gen_rand(
	size_t *out_len)
{
#ifdef __MBEDTLS__ 

	time_t ti;
	char strerr[133], pers[33];
	int retcode = 0;
	unsigned char need[32] /* SHA256 */, *out = NULL;
	size_t alloc_size = 32;	/* SHA256 */
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_entropy_context entropy;

	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);
	memset(strerr, 0, sizeof(strerr));
	
	time(&ti);
	memset(pers, 0, sizeof(pers));
	snprintf(pers, sizeof(pers)-1, "%ld", ti);
	
	out = (unsigned char *)malloc(alloc_size);
	if (out == NULL)
	{
		printf("%s(%d):Memory allocate failed ...\n", __func__, __LINE__);
		goto gen_rand_err;
	}
	memset(out, 0, alloc_size);
	
	retcode = mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
	if (retcode != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_ctr_drbg_seed() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto gen_rand_err;
	}
	
	memset(need, 0, sizeof(need));	
	retcode = mbedtls_ctr_drbg_random(&ctr_drbg, need, sizeof(need));
	if (retcode != 0)
	{
		mbedtls_strerror(retcode, strerr, sizeof(strerr)-1);
		printf("%s(%d):Failed to mbedtls_ctr_drbg_random() returned : (0x%04x)%s\n", __func__, __LINE__, -retcode, strerr);
		goto gen_rand_err;
	}

	mbedtls_sha256(need, sizeof(need), out, 0 /* is224 */);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	if (out_len != NULL) *out_len = alloc_size;
	return out;

gen_rand_err:
	if (out_len != NULL) *out_len = 0;
	if (out != NULL) free(out);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	return NULL;

#else	/* __MBEDTLS__ */	

	unsigned char need[SHA256_DIGEST_LENGTH], *out = NULL;
	size_t alloc_size = SHA256_DIGEST_LENGTH;	
	SHA256_CTX ctx;
	
	out = (unsigned char *)malloc(alloc_size);
	if (out == NULL)
	{
		printf("%s(%d):Memory allocate failed ...\n", __func__, __LINE__);
		return NULL;
	}
	memset(out, 0, alloc_size);
		
	memset(need, 0, sizeof(need));
	if (RAND_bytes(need, sizeof(need)) == 0)
	{
		free(out);
		printf("%s(%d):RAND_bytes() failed ...\n", __func__, __LINE__); 
		return NULL;
	}

	if (!SHA256_Init(&ctx))
	{
		free(out);
		printf("%s(%d):SHA256_Init() failed ...\n", __func__, __LINE__);
		return NULL;
	}

	if (!SHA256_Update(&ctx, need, sizeof(need)))
	{
		free(out);
		printf("%s(%d):SHA256_Update() failed ...\n", __func__, __LINE__);
		return NULL;
	}
	
	if (!SHA256_Final(out, &ctx))
	{
		free(out);
		printf("%s(%d):SHA256_Final() failed ...\n", __func__, __LINE__);
		return NULL;
	}
	
	if (out_len != NULL) *out_len = SHA256_DIGEST_LENGTH;
	return out;

#endif	/* __MBEDTLS__ */	
}
//---------------------------------------------------------------------------
unsigned char *gen_session_key(
	unsigned char *km,
	size_t km_len, 
	unsigned char *ns,
	size_t ns_len,
	unsigned char *nc,
	size_t nc_len,
	size_t *out_len)
{
#ifdef __MBEDTLS__ 

	unsigned char *need = NULL, *md = NULL;
	size_t need_len = 0, md_len = 32;	/* SHA256 */
	
	if (km == NULL || ns == NULL || nc == NULL)
	{
		goto gen_session_key_err;
	}
	
	if (km_len <= 0 || ns_len <= 0 || nc_len <= 0)
	{
		goto gen_session_key_err;
	}
	
	need_len = km_len + ns_len + nc_len;
	need = (unsigned char *)malloc(need_len);
	if (need == NULL)
	{
		printf("%s(%d):Memory allocate failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}
	
	memset(need, 0, need_len);
	memcpy((unsigned char *)&need[0], (unsigned char *)&km[0], km_len);
	memcpy((unsigned char *)&need[km_len], (unsigned char *)&ns[0], ns_len);
	memcpy((unsigned char *)&need[km_len+ns_len], (unsigned char *)&nc[0], nc_len);
	
	md = (unsigned char *)malloc(md_len);
	if (md == NULL)
	{
		printf("%s(%d):Memory allocate failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}
	
	memset(md, 0, md_len);
	mbedtls_sha256(need, need_len, md, 0 /* is224 */);
	free(need);
	if (out_len != NULL) *out_len = md_len;
	return md;

gen_session_key_err:
	if (out_len != NULL) *out_len = 0;
	if (need != NULL) free(need);
	if (md != NULL) free(md);
	return NULL;

#else	/* __MBEDTLS__ */

	unsigned char *need = NULL, *md = NULL;
	size_t need_len = 0, md_len = SHA256_DIGEST_LENGTH; 
	SHA256_CTX ctx;
	
	if (km == NULL || ns == NULL || nc == NULL)
	{
		goto gen_session_key_err;
	}
	if (km_len <= 0 || ns_len <= 0 || nc_len <= 0)
	{
		goto gen_session_key_err;
	}
	
	need_len = km_len + ns_len + nc_len;
	need = (unsigned char *)malloc(need_len);
	if (need == NULL)
	{
		printf("%s(%d):Memory allocate failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}
	
	memset(need, 0, need_len);
	memcpy((unsigned char *)&need[0], (unsigned char *)&km[0], km_len);
	memcpy((unsigned char *)&need[km_len], (unsigned char *)&ns[0], ns_len);
	memcpy((unsigned char *)&need[km_len+ns_len], (unsigned char *)&nc[0], nc_len);
	
	md = (unsigned char *)malloc(md_len);
	if (md == NULL)
	{
		printf("%s(%d):Memory allocate failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}
	
	memset(md, 0, md_len);
	if (!SHA256_Init(&ctx))
	{
		printf("%s(%d):SHA256_Init() failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}

	if (!SHA256_Update(&ctx, need, need_len))
	{
		printf("%s(%d):SHA256_Update() failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}
	
	if (!SHA256_Final(md, &ctx))
	{
		printf("%s(%d):SHA256_Final() failed ...\n", __func__, __LINE__);
		goto gen_session_key_err;
	}
	
	free(need);
	if (out_len != NULL) *out_len = md_len;
	return md;
	
gen_session_key_err:
	if (need != NULL) free(need);
	if (md != NULL) free(md);
	if (out_len != NULL) *out_len = 0;
	return NULL;

#endif	/* __MBEDTLS__ */	
	
}
//---------------------------------------------------------------------------

