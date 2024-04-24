#ifndef _RPMPGP_INTERNAL_H
#define _RPMPGP_INTERNAL_H

#include <rpm/rpmpgp.h>

typedef enum rpmpgpRC_e {
    RPMPGP_OK				= 0,
    RPMPGP_ERROR_INTERNAL		= 10,
    RPMPGP_ERROR_CORRUPT_PGP_PACKET	= 11,
    RPMPGP_ERROR_UNEXPECTED_PGP_PACKET	= 12,
    RPMPGP_ERROR_UNSUPPORTED_VERSION	= 13,
    RPMPGP_ERROR_UNSUPPORTED_ALGORITHM	= 14,
    RPMPGP_ERROR_UNSUPPORTED_CURVE	= 15,
    RPMPGP_ERROR_NO_CREATION_TIME	= 16,
    RPMPGP_ERROR_DUPLICATE_DATA		= 17,
    RPMPGP_ERROR_UNKNOWN_CRITICAL_PKT	= 18,
    RPMPGP_ERROR_BAD_PUBKEY_STRUCTURE	= 19,
    RPMPGP_ERROR_MISSING_SELFSIG	= 20,
    RPMPGP_ERROR_SELFSIG_VERIFICATION	= 21,
    RPMPGP_ERROR_SIGNATURE_VERIFICATION	= 22,
    RPMPGP_ERROR_BAD_PUBKEY		= 23,
    RPMPGP_ERROR_BAD_SIGNATURE		= 24
} rpmpgpRC;

typedef struct pgpDigAlg_s * pgpDigAlg;

typedef rpmpgpRC (*setmpifunc)(pgpDigAlg digp, int num, const uint8_t *p, int mlen);
typedef rpmpgpRC (*verifyfunc)(pgpDigAlg pgpkey, pgpDigAlg pgpsig,
                          uint8_t *hash, size_t hashlen, int hash_algo);
typedef void (*freefunc)(pgpDigAlg digp);

struct pgpDigAlg_s {
    setmpifunc setmpi;
    verifyfunc verify;
    freefunc free;
    int curve;
    int mpis;
    void *data;			/*!< algorithm specific private data */
};

/*
 * Values parsed from OpenPGP signature/pubkey packet(s).
 */
struct pgpDigParams_s {
    uint8_t tag;
    char * userid;		/*!< key user id */
    uint8_t key_flags;		/*!< key usage flags */

    uint8_t version;		/*!< key/signature version number. */
    uint32_t time;		/*!< key/signature creation time. */
    uint8_t pubkey_algo;	/*!< key/signature public key algorithm. */

    uint8_t hash_algo;		/*!< signature hash algorithm */
    uint8_t sigtype;
    uint8_t * hash;
    uint32_t hashlen;
    uint8_t signhash16[2];
    pgpKeyID_t signid;		/*!< key id of pubkey or signature */
    uint32_t key_expire;	/*!< key expire time. */
    uint32_t sig_expire;	/*!< signature expire time. */
    int revoked;		/*!< is the key revoked? */
    uint8_t saved;		/*!< Various flags. */
#define	PGPDIG_SAVED_TIME	(1 << 0)
#define	PGPDIG_SAVED_ID		(1 << 1)
#define	PGPDIG_SAVED_KEY_FLAGS	(1 << 2)
#define	PGPDIG_SAVED_KEY_EXPIRE	(1 << 3)
#define	PGPDIG_SAVED_PRIMARY	(1 << 4)
#define	PGPDIG_SAVED_VALID	(1 << 5)
#define	PGPDIG_SAVED_SIG_EXPIRE	(1 << 6)
    uint8_t * embedded_sig;	/* embedded signature */
    size_t embedded_sig_len;	/* length of the embedded signature */
    pgpKeyID_t mainid;		/* key id of main key if this is a subkey */

    size_t mpi_offset;		/* start of mpi data */
    pgpDigAlg alg;		/*!< algorithm specific data like MPIs */
};

RPM_GNUC_INTERNAL
pgpDigAlg pgpDigAlgNewPubkey(int algo, int curve);

RPM_GNUC_INTERNAL
pgpDigAlg pgpDigAlgNewSignature(int algo);

#endif /* _RPMPGP_INTERNAL_H */
