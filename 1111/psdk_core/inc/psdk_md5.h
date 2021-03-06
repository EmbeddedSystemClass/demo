/**
 * crypto-algorithms
 * =================
 *
 * About
 * ---
 * These are basic implementations of standard cryptography algorithms, written by Brad Conte (brad@bradconte.com) from
 * scratch and without any cross-licensing. They exist to provide publically accessible, restriction-free implementations
 * of popular cryptographic algorithms, like AES and SHA-1. These are primarily intended for educational and pragmatic
 * purposes (such as comparing a specification to actual implementation code, or for building an internal application
 * that computes test vectors for a product). The algorithms have been tested against standard test vectors.
 * This code is released into the public domain free of any restrictions. The author requests acknowledgement if the code
 * is used, but does not require it. This code is provided free of any liability and without any quality claims by the
 * author.
 * Note that these are *not* cryptographically secure implementations. They have no resistence to side-channel attacks
 * and should not be used in contexts that need cryptographically secure implementations.
 * These algorithms are not optimized for speed or space. They are primarily designed to be easy to read, although some
 * basic optimization techniques have been employed.
 * Building
 * ---
 * The source code for each algorithm will come in a pair of a source code file and a header file. There should be no
 * inter-header file dependencies, no additional libraries, no platform-specific header files, or any other complicating
 * matters. Compiling them should be as easy as adding the relevent source code to the project.
 * @note DJI has modified some symbols' name.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PSDK_MD5_H
#define PSDK_MD5_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>

/* Exported constants --------------------------------------------------------*/
#define MD5_BLOCK_SIZE 16               // MD5 outputs a 16 byte digest

/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

typedef struct {
   BYTE data[64];
   WORD datalen;
   unsigned long long bitlen;
   WORD state[4];
} MD5_CTX;

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void md5_init(MD5_CTX *ctx);
void md5_update(MD5_CTX *ctx, const BYTE data[], size_t len);
void md5_final(MD5_CTX *ctx, BYTE hash[]);

/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif   // MD5_H
