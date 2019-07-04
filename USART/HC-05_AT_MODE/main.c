/*
 * ADXL345
 *
 * Created          : 7/4/2019 4:00 AM
 * Author           : Mohamed_Hassanin
 *
 * micro controller : TM4C1294NCPDT
 * LaunchPad        : TIVA C TMC41294
 */

//this project is about how to configure HC-05 using AT-commands

            //------------------connection-----------------------//
            //1          |  |                                 |  |
            //2          |  |          / \                    |  |
            //3HC-05_TX<-|  |         / | \                   |  |
            //4HC-05_RX<-|  |           |                     |  |
            //5          |  |           |                     |  |
            //6          |  |           |BoosterPack 1        |  |
            //7          |  |           |                     |  |
            //8          |  |           |                     |  |
            //9          |  |           |                     |  |
            //10         |  |           |                     |  |

            //1          |  |                                 |  |
            //2          |  |          / \                    |  |
            //3          |  |         / | \                   |  |
            //4          |  |           |                     |  |
            //5          |  |           |                     |  |
            //6          |  |           |BoosterPack 2        |  |
            //7          |  |           |                     |  |
            //8          |  |           |                     |  |
            //9          |  |           |                     |  |
            //10         |  |           |                     |  |

#include "UART0/UART0.h"
#include "UART7/UART7.h"
#include "CLK.h"

void main(void)
{
    INIT_CLK();

    INIT_UART0();
    INIT_UART7();

    while(1){
    }
}
