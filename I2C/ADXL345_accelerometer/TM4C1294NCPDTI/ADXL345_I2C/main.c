/*
 * ADXL345
 *
 * Created          : 7/2/2019 5:22 PM
 * Author           : Mohamed_Hassanin
 *
 * micro controller : TM4C1294NCPDT
 * LaunchPad        : TIVA C TMC41294
 */

//this project is about driving ADX345 using I2C
//print the x,y,z axis rotations using UART

            //------------------connection-----------------------//
            //1          |  |                                 |  |
            //2          |  |          / \                    |  |
            //3          |  |         / | \                   |  |
            //4          |  |           |                     |  |
            //5          |  |           |                     |  |
            //6          |  |           |BoosterPack 1        |  |
            //7          |  |           |                     |  |
            //8          |  |           |                     |  |
            //9     SCL<-|  |           |                     |  |
            //10    SDA<-|  |           |                     |  |

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


#include "ADXL345.h"
#include "UART.h"
#include "CLK.h"

void main(void)
{
    INIT_CLK();

    INIT_UART();

    INIT_ADXL();

    int i = 0;
    while(1){
        UART_PRINTN(X_AXIS());
        UART_TX(' ');
        UART_PRINTN(Y_AXIS());
        UART_TX(' ');
        UART_PRINTN(Z_AXIS());
        UART_TX('\n');
        while(i < 200000){
            i++;
        }
        i = 0;
    }
}

