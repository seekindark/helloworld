#ifndef CANTPAPPBASE_H
#define CANTPAPPBASE_H
#include <stdint.h>

class canTpAppBase
{
public:
    canTpAppBase();
    ~canTpAppBase();
    virtual int receive_cantp_payload(uint8_t payload[], uint16_t size) = 0;
};

#endif // CANTPAPPBASE_H
