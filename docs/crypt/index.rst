
Encryption
########################################

All these encryption methods use primarily software. No hardware drivers implemented, at least not yet.

AES
****************************************

Software AES support thanks to `AndrewCarterUK <https://twitter.com/AndrewCarterUK>`_ from `SmarterDM <https://github.com/SmarterDM/micro-aes>`_.

* Supported key sizes are 128, 192 and 256 bits (10/12/14 rounds for each)
* Block size is 128 bits

To enable AES set ``USE_AES``.

.. c:function:: void aes_128_init(aes_128_context_t *context, uint8_t key[16])

.. c:function:: void aes_128_encrypt(aes_128_context_t *context, uint8_t block[16])

.. c:function:: void aes_128_decrypt(aes_128_context_t *context, uint8_t block[16])

.. c:function:: void aes_192_init(aes_192_context_t *context, uint8_t key[24])

.. c:function:: void aes_192_encrypt(aes_192_context_t *context, uint8_t block[16])

.. c:function:: void aes_192_decrypt(aes_192_context_t *context, uint8_t block[16])

.. c:function:: void aes_256_init(aes_256_context_t *context, uint8_t key[32])

.. c:function:: void aes_256_encrypt(aes_256_context_t *context, uint8_t block[16])

.. c:function:: void aes_256_decrypt(aes_256_context_t *context, uint8_t block[16])

Defines
========================================

.. cpp:enumerator:: AES_128_ROUNDS = 10
.. cpp:enumerator:: AES_192_ROUNDS = 12
.. cpp:enumerator:: AES_256_ROUNDS = 14

RC5
****************************************

Should be only really used in very resource constrained platforms.

* Key size is 128 bits as default
* Block size is fixed to 64 bits
* Rounds is 12 as default

To enable RC5 set ``USE_RC5``.

.. c:function:: void rc5_init(rc5_context_t *rc5, void *key)

	:param rc5: pointer to preallocated memory
	:type rc5: rc5_context_t *
	:param key: pointer to memory holding binary key, must be at least 16 bytes
	:type key: void *

.. c:function:: void rc5_encrypt(rc5_context_t *rc5, void *block)

	:param rc5: previously initialized context
	:type rc5: rc5_context_t *
	:param block: pointer to memory holding binary data to be encrypted, must be at least 8 bytes
	:type block: void *

.. c:function:: void rc5_decrypt(rc5_context_t *rc5, void *block)

	:param rc5: previously initialized context
	:type rc5: rc5_context_t *
	:param block: pointer to memory holding binary data to be decrypted, must be at least 8 bytes
	:type block: void *

Defines
========================================

.. cpp:enumerator:: RC5_KEY_LEN = 16

	Key length in bytes. Can be overridden by defining other value globally.

.. cpp:enumerator:: RC5_ROUNDS = 12

	Rounds. Can be overridden by defining other value globally.

XTEA
****************************************

Should be only really used in very resource constrained platforms.

 .. note:: This implementation has not been properly evaluated against others since there are so few. It is anyways almost a direct copy-paste from wikipedia.

* Key size is 128 bits
* Block size is 64 bits
* Rounds is 32

To enable XTEA set ``USE_XTEA``.

.. c:function:: void xtea_encrypt(void *block, void *key)

.. c:function:: void xtea_decrypt(void *block, void *key)

XXTEA
****************************************

Should be only really used in very resource constrained platforms.

 .. note:: This implementation has not been properly evaluated against others since there are so few. It is anyways almost a direct copy-paste from wikipedia.

* Key size is 128 bits
* Block size must be at least 64 bits (8 bytes) and multiples of 32 bits (4 bytes)
* Rounds is 32

To enable XXTEA set ``USE_XXTEA``.

.. c:function:: void xxtea_encrypt(void *data, size_t size, void *key)

.. c:function:: void xxtea_decrypt(void *data, size_t size, void *key)
