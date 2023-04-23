#include "test_cases.h"

/* test cases from rfc 2202 + rfc 2286 */

rfc_test_t test_hmac_16[] = {
  { "rfc test case 1", "0x0b", 16, "Hi There", 0,
    "0x9294727a3638bb1c13f48ef8158bfc9d",
    "0xfbf61f9492aa4bbf81c172e84e0734db"
  },

  { "rfc test case 2", "Jefe", 0,  "what do ya want for nothing?", 0,
    "0x750c783e6ab0b503eaa86e310a5db738",
    "0x875f828862b6b334b427c55f9f7ff09b"
  },

  { "rfc test case 3", "0xaa", 16, "0xdd", 50,
    "0x56be34521d144c88dbb8c733f0e8b3f6",
    "0x09f0b2846d2f543da363cbec8d62a38d"
  },

  { "rfc test case 4", "0x0102030405060708090a0b0c0d0e0f10111213141516171819", 0, "0xcd", 50,
    "0x697eaf0aca3a3aea3a75164746ffaa79",
    "0xbdbbd7cf03e44b5aa60af815be4d2294"
  },

  { "rfc test case 5", "0x0c", 16, "Test With Truncation", 0,
    "0x56461ef2342edc00f9bab995690efd4c",
    "0xe79808f24b25fd031c155f0d551d9a3a"
  },

  { "rfc test case 6", "0xaa", 80, "Test Using Larger Than Block-Size Key - Hash Key First", 0,
    "0x6b1ab7fe4bd7bf8f0b62e6ce61b9d0cd",
    "0xdc732928de98104a1f59d373c150acbb"
  },

  { "rfc test case 7", "0xaa", 80, "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data", 0,
    "0x6f630fad67cda0ee1fb1f562db3aa53e",
    "0x5c6bec96793e16d40690c237635f30c5"
  },

  { 0,0,0,0,0,0,0 }
};

rfc_test_t test_hmac_20[] = {
  { "rfc test case 1", "0x0b", 20, "Hi There", 0,
    "0xb617318655057264e28bc0b6fb378c8ef146be00",
    "0x24cb4bd67d20fc1a5d2ed7732dcc39377f0a5668"
  },

  { "rfc test case 2", "Jefe", 0,  "what do ya want for nothing?", 0,
    "0xeffcdf6ae5eb2fa2d27416d5f184df9c259a7c79",
    "0xdda6c0213a485a9e24f4742064a7f033b43c4069"
  },

  { "rfc test case 3", "0xaa", 20, "0xdd", 50,
    "0x125d7342b9ac11cd91a39af48aa17b4f63f175d3",
    "0xb0b105360de759960ab4f35298e116e295d8e7c1"
  },

  { "rfc test case 4", "0x0102030405060708090a0b0c0d0e0f10111213141516171819", 0, "0xcd", 50,
    "0x4c9007f4026250c6bc8414f9bf50c86c2d7235da",
    "0xd5ca862f4d21d5e610e18b4cf1beb97a4365ecf4"
  },

  { "rfc test case 5", "0x0c", 20, "Test With Truncation", 0,
    "0x4c1a03424b55e07fe7f27be1d58bb9324a9a5a04",
    "0x7619693978f91d90539ae786500ff3d8e0518e39"
  },

  { "rfc test case 6", "0xaa", 80, "Test Using Larger Than Block-Size Key - Hash Key First", 0,
    "0xaa4ae5e15272d00e95705637ce8a3b55ed402112",
    "0x6466ca07ac5eac29e1bd523e5ada7605b791fd8b"
  },

  { "rfc test case 7", "0xaa", 80, "Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data", 0,
    "0xe8e99d0f45237d786d6bbaa7965c7808bbff1a91",
    "0x69ea60798d71616cce5fd0871e23754cd75d5a0a"
  },

  { 0,0,0,0,0,0,0 }
};
