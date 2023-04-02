#include <stdio.h>

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;
#define UDS_SEED_VER_1  1
uint8_t uds_key_cal_ver_1(uint32_t seed, uint8_t *key);

/*
    CALCUALTE the key according to the seed and version
*/
uint8_t uds_key_cal(uint32_t seed, uint8_t *key)
{
    uint8_t seed_ver = ((seed >> 28) & 0x0F);
    if(seed_ver == UDS_SEED_VER_1 && key != 0)
    {
        return uds_key_cal_ver_1(seed, key);
    }
    else
    {
        return 0;
    }
}

/*
    Key algorithm version=1
    input: seed,  uint32
            key,  uint32[4]
    output: key,  uint32[4]
            key_len,    uint8_t

            0x b a8  9 ba
*/
uint8_t uds_key_cal_ver_1(uint32_t seed, uint8_t *key)
{
    const uint8_t appKey[4] = {0x7c, 0xF3,0x73, 0x0D};
    uint8_t seedBytes[4], key2[4];
    uint32_t i;
    for(i = 0; i < 4; i++)
    {
        seedBytes[i] = 0XFF & (seed >> ((sizeof(seed) - (1 + i)) * 8));
        key[i] = seedBytes[i] ^ appKey[i];
    }
    for(i = 0; i < 4; i++)
    {
        key2[i] = seedBytes[4-i-1] ^ appKey[i];
        key[i] = key[i] + key2[i];
    }

    return 4;

}

int main()
{
        uint32_t seed = 0x1f123456;
        uint8_t key[4];
        uint8_t ret = uds_key_cal_ver_1(seed, key);
        printf("ret = %d\n", ret);
        printf("key=%02x,%02x,%02x,%02x\n", key[0],key[1],key[2],key[3]);


		seed =0x1faabcDF;
        ret = uds_key_cal_ver_1(seed, key);
        printf("ret = %d\n", ret);
        printf("key=%02x,%02x,%02x,%02x\n", key[0],key[1],key[2],key[3]);

		
		seed =0x1f0302DF;
        ret = uds_key_cal_ver_1(seed, key);
        printf("ret = %d\n", ret);
        printf("key=%02x,%02x,%02x,%02x\n", key[0],key[1],key[2],key[3]);
		
		seed =0x100302DF;
        ret = uds_key_cal_ver_1(seed, key);
        printf("ret = %d\n", ret);
        printf("key=%02x,%02x,%02x,%02x\n", key[0],key[1],key[2],key[3]);
        return 0;
}
