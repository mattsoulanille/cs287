
// from https://github.com/Fruneng/opencl_sha_al_im/blob/master/sha1/sha1.cl
#define SWAP32(a)	(as_uint(as_uchar4(a).wzyx))


#define H0 0x67452301
#define H1 0xEFCDAB89
#define H2 0x98BADCFE
#define H3 0x10325476
#define H4 0xC3D2E1F0

// From: http://stackoverflow.com/questions/111928/is-there-a-printf-converter-to-print-in-binary-format
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


// Modified from: https://en.wikipedia.org/wiki/Circular_shift
#define CHAR_BIT 8

static uint leftrotate(uint value, uint count) {
    const uint mask = (CHAR_BIT*sizeof(value)-1);
    count &= mask;
    return (value<<count) | (value>>( (-count) & mask ));
}




// see https://en.wikipedia.org/wiki/SHA-1
__kernel void sha1_crypt_kernel(__global uint *data_info, __global uchar *plain_key, __global uint *digest) {
  int gid = get_global_id(0);

  uint tmp;
  //uint num_keys = data_info[0];
  uint ml = data_info[1] * sizeof(uchar) * 8; // message length in bits

  
  int i;
  
  unsigned int message[16];
  //unsigned char *char_message = message; // assumes message is smaller than 448 bits

  //uchar4 char_message[16];
  //uint4 *message = char_message;


  
  int plain_key_start = gid * ((int) data_info[1]); // start of a given key
  //unsigned char char_message[64]; // assumes message is smaller than 448 bits
  memset(&message, 0, sizeof(message)); // zeroes the entries


  //char debug_plaintext[data_info[1] + 1];


  // copy the plain_key to the message:

  for (i = 0; i < (int) data_info[1]; i++) {
    /* if (gid == 0) { */
    /*   printf("%c", plain_key[i + plain_key_start]); */
    /* } */
    message[i/4] += plain_key[i + plain_key_start] << (8 * (3 - (i % 4)) );
  }

  //printf("\t %d\n", plain_key_start);
  //printf("\t %d\n", gid);
  
  // append the bit 1 to the message
  i = (int) data_info[1];
  message[i/4] += 0x80 << (8 * (3 - (i % 4)) );
  
  // set the last bits as the message length as a 64 bit big endian integer (unsigned????)
  message[15] = ml;

  //debug printing
  /*
  if (gid == 0) {
    printf("Message: ");
    for (i = 0; i < 16; i++) {

      printf("%08x  ", message[i]);
    }
    printf("\n\n");
    printf("bin_message: \n");
    
    for (i = 0; i < 16; i++) {
      unsigned int swapped = SWAP32(message[i]);
      uchar4 c = as_uchar4(swapped);
      printf(BYTE_TO_BINARY_PATTERN"  ", BYTE_TO_BINARY(c.x));
      printf(BYTE_TO_BINARY_PATTERN"  ", BYTE_TO_BINARY(c.y));
      printf(BYTE_TO_BINARY_PATTERN"  ", BYTE_TO_BINARY(c.z));
      printf(BYTE_TO_BINARY_PATTERN"  ", BYTE_TO_BINARY(c.w));
      printf("\n");
      // printf("%x ", c.x);
      // printf("%x ", c.y);
      // printf("%x ", c.z);
      // printf("%x ", c.w);
      
    }
    printf("\n\n");
  }
  */

  // set up 80 32-bit words from the 16 in message
  uint w[80];
  // Copy the message
  memcpy(w, message, sizeof(uint) * 16);

  // Generate the other 64 words
  for (i = 16; i < 80; i++) {
    w[i] = leftrotate( (w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16]), 1);
  }

  // debugging
  /*
  if (gid == 0) {
    for (i = 0; i < 80; i++) {
      
      printf("%08x", w[i]);
      
      if ((i + 1) % 4) {
	printf(" ");
      }
      else {
	printf("\n");
      }
    }
    printf("\n");
  }
  */

  uint h0, h1, h2, h3, h4;
  h0 = H0;
  h1 = H1;
  h2 = H2;
  h3 = H3;
  h4 = H4;
	
  // Main loop of hash
  uint a, b, c, d, e, f, k;
  a = h0;
  b = h1;
  c = h2;
  d = h3;
  e = h4;

  for (i = 0; i < 80; i++) {
    if ((0 <= i) & (i <= 19)) {
      f = (b & c) | ((~b) & d);
      k = 0x5A827999;
    }
    else if ((20 <= i) & (i <= 39)) {
      f = b ^ c ^ d;
      k = 0x6ED9EBA1;
    }
    else if ((40 <= i) & (i <= 59)) {
      f = (b & c) | (b & d) | (c & d);
      k = 0x8F1BBCDC;
    }
    else if ((60 <= i) & (i <= 79)) {
      f = b ^ c ^ d;
      k = 0xCA62C1D6;
    }

    tmp = leftrotate(a, 5) + f + e + k + w[i];
    e = d;
    d = c;
    c = leftrotate(b, 30);
    b = a;
    a = tmp;

    /*
    if (gid == 0) {
      printf("%08x %08x %08x %08x %08x\n", a, b, c, d, e);
    }
    */
  }

  h0 = h0 + a;
  h1 = h1 + b;
  h2 = h2 + c;
  h3 = h3 + d;
  h4 = h4 + e;


  /*
  if (gid == 0) {
    printf("\n");
    //    printf("xor test: 0xFFFFFF0F ^ 0xFFFFFFFF = %08x", 0xFFFFFF0F ^ 0xFFFFFFFF);
    //    printf("\n");
    printf("%x  %x  %x  %x  %x\n", h0, h1, h2, h3, h4);
  }
  */

  digest[0 + 5*gid] = h0;
  digest[1 + 5*gid] = h1;
  digest[2 + 5*gid] = h2;
  digest[3 + 5*gid] = h3;
  digest[4 + 5*gid] = h4;

  return;
}


