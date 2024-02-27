/************************************************************************************************************************
* Gonzalo Alberto Guajardo Galindo *
* 
* Device: Atmega328P *
* Rev: 1 *
* Activity: Mid-term evaluation *
* Description: This project entails the design of an electronic system using a microcontroller to control the operation 
* of four LEDs and a 7-segment display. The system is programmed to exhibit specific sequences triggered by switches:
* Sequence a) "00" - All LEDs are fully illuminated, and the 7-segment display shows an "E".
* Sequence b) "01" - LEDs display a descending 4-bit counter, while the display shows a "1".
* Sequence c) "10" - LEDs display an ascending 4-bit counter, while the display shows a "2".
* Sequence d) "11" - All LEDs turn off, and the display shows a descending counter from 9 to 0.

* Date: 09/26/22 *
************************************************************************************************************************/
// LIBRARIES
#include <avr/io.h>
#include <util/delay.h>

// CONSTANTS
#define F_CPU 1600000UL
#define DELAY _delay_ms(500)

// MACROS
#define _BV(bit) (1 << (bit))

// INPUTS
#define Btns PINC
#define Btn1 PINC2
#define Btn1_press bit_is_set(PINC,Btn1)
#define Btn2 PINC3
#define Btn2_press bit_is_set(PINC,Btn2)
#define C_Zero ((!Btn1_press) && (!Btn2_press))
#define C_One ((!Btn1_press) && (Btn2_press))
#define C_Two ((Btn1_press) && (!Btn2_press))
#define C_Three ((Btn1_press) && (Btn2_press))

// OUTPUTS DISPLAY
#define Seg_A PORTB3
#define Seg_A_On PORTB &= ~_BV(Seg_A)
#define Seg_A_Off PORTB |= _BV(Seg_A)
#define Seg_B PORTB4
#define Seg_B_On PORTB &= ~_BV(Seg_B)
#define Seg_B_Off PORTB |= _BV(Seg_B)
#define Seg_C PORTD6
#define Seg_C_On PORTD &= ~_BV(Seg_C)
#define Seg_C_Off PORTD |= _BV(Seg_C)
#define Seg_D PORTD7
#define Seg_D_On PORTD &= ~_BV(Seg_D)
#define Seg_D_Off PORTD |= _BV(Seg_D)
#define Seg_E PORTB0
#define Seg_E_On PORTB &= ~_BV(Seg_E)
#define Seg_E_Off PORTB |= _BV(Seg_E)
#define Seg_F PORTB2
#define Seg_F_On PORTB &= ~_BV(Seg_F)
#define Seg_F_Off PORTB |= _BV(Seg_F)
#define Seg_G PORTB1
#define Seg_G_On PORTB &= ~_BV(Seg_G)
#define Seg_G_Off PORTB |= _BV(Seg_G)
#define Seg_DP PORTD5
#define Seg_DP_On PORTD &= ~_BV(Seg_DP)
#define Seg_DP_Off PORTD |= _BV(Seg_DP)

// OUTPUTS LEDS
#define Led_1 PORTB6
#define Led_1_On PORTB |= _BV(Led_1)
#define Led_1_Off PORTB &= ~_BV(Led_1)
#define Led_2 PORTD4
#define Led_2_On PORTD |= _BV(Led_2)
#define Led_2_Off PORTD &= ~_BV(Led_2)
#define Led_3 PORTD3
#define Led_3_On PORTD |= _BV(Led_3)
#define Led_3_Off PORTD &= ~_BV(Led_3)
#define Led_4 PORTD2
#define Led_4_On PORTD |= _BV(Led_4)
#define Led_4_Off PORTD &= ~_BV(Led_4)

// FUNCTIONS
void init_port (void);
void show_num (uint8_t x);
void off (void);
void on (void);
void ascend (uint8_t i);
void descend (uint8_t z);

// MAIN
int main(void)
{
    init_port ();
    uint8_t x = 9;
    uint8_t j = 0;
    uint8_t z = 0;
    while (1)
    {
        if (C_Zero)
        {
            show_num(14);   //Display shows E
            on();           //Leds turned on
        }
        else if(C_One)
        {
            show_num(1);    //Display shows 1
            while(z < 16)
            {
                ascend(z);
                DELAY;
                if (C_Zero || C_Two || C_Three)
                {
                break;
                }
                else
                {
                z++;
                }
            }
            z = 0;          //Reset count to 0
        }
        else if (C_Two)
        {
            show_num(2);    //Display shows 2
            while(j < 16)
            {
                descend(j);
                DELAY;
                if (C_Zero || C_One || C_Three)
                {
                    break;
                }
                else
                {
                    j++;
                }
            }
            j = 0;           //Reset count to 0
        }
        else if(C_Three)
        {
            off();           //Leds turned off
            while(x <= 9)    // 0 to 9
            {
                show_num(x);
                DELAY;
                if (C_Zero || C_One || C_Two)
                {
                    break;
                }
                else 
                {
                    x--;
                }
            }
            x=9;            //Reset count to 0
        }
    }
}

void init_port (void)
{
    // INPUTS
    DDRC &= ~_BV(Btn1);
    DDRC &= ~_BV(Btn2);
    // OUTPUTS
    DDRB |= (_BV(Seg_A) | _BV(Seg_B) | _BV(Seg_E) | _BV(Seg_F) | _BV(Seg_G) |
    _BV(Led_1));
    DDRD |= (_BV(Seg_C) | _BV(Seg_D) | _BV(Seg_DP)| _BV(Led_2) | _BV(Led_3) |
    _BV(Led_4));
}

void show_num (uint8_t output)
{
    switch(output)
    {
        case 0:     // 0
        Seg_A_On;
        Seg_B_On;
        Seg_C_On;
        Seg_D_On;
        Seg_E_On;
        Seg_F_On;
        Seg_G_Off;
        Seg_DP_Off;
        break;
        case 1:     // 1
        Seg_A_Off;
        Seg_B_On;
        Seg_C_On;
        Seg_D_Off;
        Seg_E_Off;
        Seg_F_Off;
        Seg_G_Off;
        Seg_DP_Off;
        break;
        case 2:     // 2
        Seg_A_On;
        Seg_B_On;
        Seg_C_Off;
        Seg_D_On;
        Seg_E_On;
        Seg_F_Off;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 3:     // 3
        Seg_A_On;
        Seg_B_On;
        Seg_C_On;
        Seg_D_On;
        Seg_E_Off;
        Seg_F_Off;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 4:     // 4
        Seg_A_Off;
        Seg_B_On;
        Seg_C_On;
        Seg_D_Off;
        Seg_E_Off;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 5:     // 5
        Seg_A_On;
        Seg_B_Off;
        Seg_C_On;
        Seg_D_On;
        Seg_E_Off;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 6:     // 6
        Seg_A_On;
        Seg_B_Off;
        Seg_C_On;
        Seg_D_On;
        Seg_E_On;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 7:     // 7
        Seg_A_On;
        Seg_B_On;
        Seg_C_On;
        Seg_D_Off;
        Seg_E_Off;
        Seg_F_Off;
        Seg_G_Off;
        Seg_DP_Off;
        break;
        case 8:     // 8
        Seg_A_On;
        Seg_B_On;
        Seg_C_On;
        Seg_D_On;
        Seg_E_On;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 9:     // 9
        Seg_A_On;
        Seg_B_On;
        Seg_C_On;
        Seg_D_Off;
        Seg_E_Off;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 10:    // A
        Seg_A_On;
        Seg_B_On;
        Seg_C_On;
        Seg_D_Off;
        Seg_E_On;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 11:    // B
        Seg_A_Off;
        Seg_B_Off;
        Seg_C_On;
        Seg_D_On;
        Seg_E_On;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 12:    // C
        Seg_A_On;
        Seg_B_Off;
        Seg_C_Off;
        Seg_D_On;
        Seg_E_On;
        Seg_F_On;
        Seg_G_Off;
        Seg_DP_Off;
        break;
        case 13:    // D
        Seg_A_Off;
        Seg_B_On;
        Seg_C_On;
        Seg_D_On;
        Seg_E_On;
        Seg_F_Off;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 14:    // E
        Seg_A_On;
        Seg_B_Off;
        Seg_C_Off;
        Seg_D_On;
        Seg_E_On;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
        case 15:    // F
        Seg_A_On;
        Seg_B_Off;
        Seg_C_Off;
        Seg_D_Off;
        Seg_E_On;
        Seg_F_On;
        Seg_G_On;
        Seg_DP_Off;
        break;
    }
}

void off (void)
{
    Led_1_Off;
    Led_2_Off;
    Led_3_Off;
    Led_4_Off;
}

void on (void)
{
    Led_1_On;
    Led_2_On;
    Led_3_On;
    Led_4_On;
}

void ascend (uint8_t asc)
{
    switch(asc)
    {
        case 0:
        on(); //15
        break;
        case 1:
        Led_1_On; Led_2_On; Led_3_On; Led_4_Off;    //14
        break;
        case 2:
        Led_1_On; Led_2_On; Led_3_Off; Led_4_On;    //13
        break;
        case 3:
        Led_1_On; Led_2_On; Led_3_Off; Led_4_Off;   //12
        break;
        case 4:
        Led_1_On; Led_2_Off; Led_3_On; Led_4_On;    //11
        break;
        case 5:
        Led_1_On; Led_2_Off; Led_3_On; Led_4_Off;   //10
        break;
        case 6:
        Led_1_On; Led_2_Off; Led_3_Off; Led_4_On;   //09
        break;
        case 7:
        Led_1_On; Led_2_Off; Led_3_Off; Led_4_Off;  //08
        break;
        case 8:
        Led_1_Off; Led_2_On; Led_3_On; Led_4_On;    //07
        break;
        case 9:
        Led_1_Off; Led_2_On; Led_3_On; Led_4_Off;   //06
        break;
        case 10:
        Led_1_Off; Led_2_On; Led_3_Off; Led_4_On;   //05
        break;
        case 11:
        Led_1_Off; Led_2_On; Led_3_Off; Led_4_Off;  //04
        break;
        case 12:
        Led_1_Off; Led_2_Off; Led_3_On; Led_4_On;   //03
        break;
        case 13:
        Led_1_Off; Led_2_Off; Led_3_On; Led_4_Off;  //02
        break;
        case 14:
        Led_1_Off; Led_2_Off; Led_3_Off; Led_4_On;  //01
        break;
        case 15:
        off(); //00
        break;
    }
}

void descend(uint8_t des)
{
    switch(des)
    {
        case 0:
        off(); //00
        break;
        case 1:
        Led_1_Off; Led_2_Off; Led_3_Off; Led_4_On;  //01
        break;
        case 2:
        Led_1_Off; Led_2_Off; Led_3_On; Led_4_Off;  //02
        break;
        case 3:
        Led_1_Off; Led_2_Off; Led_3_On; Led_4_On;   //03
        break;
        case 4:
        Led_1_Off; Led_2_On; Led_3_Off; Led_4_Off;  //04
        break;
        case 5:
        Led_1_Off; Led_2_On; Led_3_Off; Led_4_On;   //05
        break;
        case 6:
        Led_1_Off; Led_2_On; Led_3_On; Led_4_Off;   //06
        break;
        case 7:
        Led_1_Off; Led_2_On; Led_3_On; Led_4_On;    //07
        break;
        case 8:
        Led_1_On; Led_2_Off; Led_3_Off; Led_4_Off;  //08
        break;
        case 9:
        Led_1_On; Led_2_Off; Led_3_Off; Led_4_On;   //09
        break;
        case 10:
        Led_1_On; Led_2_Off; Led_3_On; Led_4_Off;   //10
        break;
        case 11:
        Led_1_On; Led_2_Off; Led_3_On; Led_4_On;    //11
        break;
        case 12:
        Led_1_On; Led_2_On; Led_3_Off; Led_4_Off;   //12
        break;
        case 13:
        Led_1_On; Led_2_On; Led_3_Off; Led_4_On;    //13
        break;
        case 14:
        Led_1_On; Led_2_On; Led_3_On; Led_4_Off;    //14
        break;
        case 15:
        on(); //15
        break;
    }
}
