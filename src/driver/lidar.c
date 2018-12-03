#include "usart.h"
#include "lidar.h"

#define TFMINI_DATA_Len             9
#define TFMINT_DATA_HEAD            0x59

void lidar_readDist(int lidar)
{   
    USART_TypeDef* usart = USART2;

    if(lidar == 1) usart = USART2;
    else if(lidar == 2) usart = USART3;
    else return;

    uint32_t i = 0;
    uint16_t dist = 0;
    char c;

    _getc(usart, true, &c);
    if(TFMINT_DATA_HEAD == c)
    {
    _getc(usart, true, &c);
        if(TFMINT_DATA_HEAD == c)
        {
            _getc(usart, true, &c);
            dist = c;
            _getc(usart, true, &c);
            dist |= c<<8;

            for(i = 0; i < (TFMINI_DATA_Len - 4); i++)
            {
                _getc(usart, true, &c);
            }
/*        printf("Lidar%d: %d\r\n", lidar, dist);*/

        }
    }
}
