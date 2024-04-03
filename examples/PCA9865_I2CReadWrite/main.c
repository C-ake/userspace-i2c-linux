#include <stdio.h>
#include <us-i2c.h>


int main()
{
    printf("Era affezzionata\n");
    i2cConfiguration_t i2cConf;
    uint8_t dataBuffer = 0U;
    printf("Initializing\n");
    i2cInit(1, &i2cConf);
    printf("Reading\n");
    i2cRead(&i2cConf, 0x40, 1, 0x00, 1, &dataBuffer);

    printf("Data read: %d\n", dataBuffer);

    printf("Trying to write: %d\n", i2cWrite(&i2cConf, 0x40, 1, 0xFE, 1, &dataBuffer));

    return 0;
}
