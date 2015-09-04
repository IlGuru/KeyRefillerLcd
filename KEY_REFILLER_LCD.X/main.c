/* 
 * File:   main.c
 * Author: Stefano
 *
 * Created on 30 giugno 2015, 20.39
 */

//------------------------------------------------------------------------------
//  Wrap librerie

//#include <stdio.h>
#ifndef	_STDIO_H_
#endif	/* _STDIO_H_ */

//#include <stdlib.h>
#ifndef	_STDDEF
#define	EXIT_SUCCESS	0
#define	EXIT_FAILURE	1
#endif	/* _STDDEF */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Include
#include "project.h"

//------------------------------------------------------------------------------

// CONFIG
#pragma config FOSC     = INTOSCIO  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE     = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE    = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    = ON        // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN    = OFF       // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP      = OFF       // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD      = OFF       // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP       = OFF       // Flash Program Memory Code Protection bit (Code protection off)

//------------------------------------------------------------------------------

void print_err() {
#ifdef M24C02_M24C02_ERR_SHOW    
    lcd_print_s( (unsigned char *) m24c02_error_msg( &p_24c02_error ) );
#else
    lcd_print_s( (unsigned char *) "Check\0" );
#endif    
    __delay_ms(2500);
}

/*
void write_credito( uchar v_device, uchar *p_money, const uchar *p_message ) {
    uchar v_addr            = 0x44;
    uchar p_24c02_error     = M24C02_ERR_OK;

    lcd_clear_display();
    lcd_goto_home();
    lcd_print_s( p_message );
    
    lcd_goto_xy(1,2);
    
    m24c02_multibyte_write( v_device, v_addr       , p_money  , 4, &p_24c02_error );
    if ( p_24c02_error != M24C02_ERR_OK ) {
        print_err( p_24c02_error );
    }

    i2c_Pause();
    m24c02_multibyte_write( v_device, v_addr + 0x10, p_money+4, 4, &p_24c02_error );
    if ( p_24c02_error != M24C02_ERR_OK ) {
        print_err( p_24c02_error );
    }
    
    lcd_print_s( "Fatto.\0" );
}
*/

void write_credito( unsigned short long v_credito ) {
    uchar   v_addr          = 0x00;

    lcd_clear_display();
    lcd_goto_home();
    
    lcd_print_s( (unsigned char *) "Scrivo \0" );
    coges_val_to_string( v_credito, v_buffer );
    lcd_print_s( v_buffer );
    
    lcd_goto_xy(1,2);

    for ( v_addr=0x44; v_addr<=0x54 & p_24c02_error==M24C02_ERR_OK; v_addr+=0x10 ) {
        coges_money_code( v_credito, v_addr, v_buffer );
        m24c02_multibyte_write( 0x00, v_addr, v_buffer  , 4, &p_24c02_error );

        i2c_Pause();
    }
    if ( p_24c02_error==M24C02_ERR_OK )
        lcd_print_s( (unsigned char *) "Fatto.\0" );
}

void read_credito( unsigned short long * v_credito, unsigned char * v_buffer ) {
    /*      v_credito conterrà il valore numerico del credito presente
     *      v_buffer conterrà la stringa di caratteri con il credito da visualizzare
     */
    m24c02_sequential_random_read( 0x00, 0x44       , &v_buffer[0], 4, &p_24c02_error );

    if ( p_24c02_error == M24C02_ERR_OK ) {
        coges_money_value( v_buffer, v_credito );
        coges_val_to_string( *v_credito, v_buffer );
    }
}

void print_credito( unsigned char * v_buffer ) {
    /*      v_buffer contiene la stringa di caratteri con il credito da visualizzare
     * 
     *      Ho spostato la lettura del credito fuori da print_credito per salvare una posiione nella stack
     *      Perciò questa funzione stampa solo il credito da v_buffer, il credito va letto prima con read_credito
     */
    
//    uchar v_addr            = 0x44;
//    uchar v_ind             = 0x00;
//    uchar v_row             = 0x00;
//    
//    uchar p_24c02_error     = M24C02_ERR_OK;
//    uchar v_buffer[8];
//    
//    m24c02_sequential_random_read( v_device, v_addr       , &v_buffer[0], 4, &p_24c02_error );
//    if ( p_24c02_error == M24C02_ERR_OK ) {
//        m24c02_sequential_random_read( v_device, v_addr + 0x10, &v_buffer[4], 4, &p_24c02_error );
//        if ( p_24c02_error == M24C02_ERR_OK ) {
//            lcd_clear_display();
//            lcd_goto_home();
//
//            for ( v_row=0; v_row<2; v_row++ ) {    
//
//                lcd_goto_xy(1,1 + v_row);
//                lcd_print_s((unsigned char *) "0x\0");
//                lcd_print_hex( v_addr + ( v_row * 0x10 ) );
//
//                for ( v_ind=0; v_ind<4; v_ind++ ) {    
//                    lcd_goto_xy(6+(3*v_ind),1+v_row);
//                    lcd_print_hex( v_buffer[v_ind + (4*v_row)] );
//                }
//
//            }
//        } 
//    } 
//    if ( p_24c02_error != M24C02_ERR_OK ) {
//        print_err( p_24c02_error );
//    }
    
    lcd_clear_display();
    lcd_goto_home();
    
    lcd_print_s( (unsigned char * ) "Ci sono \0" );
    lcd_print_s( (unsigned char * ) v_buffer );

}

void sel_credito( unsigned short long * v_credito, unsigned char * v_buffer ) {
    /*      v_buffer contiene i 4 byte del coce del credito
     * 
     *      Ho spostato la lettura del credito fuori per salvare una posiione nella stack
     *      Questa funzione mostra il valore precedente del credito, e permette di selezionarne uno nuovo.
     *      Mette il valore scelto in v_buffer dopodichè potrà essere scritto con write_credito()
     */
    uchar       v_pulsante    = 0x00;
    uchar       v_change      = 0x00;

    /*      Si aspetta che sia stato chiamato precedentemente: 
     *          read_credito
     *          print_credito   che scrive nella prima riga il credito presente
     * 
     *      La funzione quindi potrà scrivere nella riga 2 del display.
     */
    
    *v_credito  = 1000;
    v_change    = 2;
    while( v_change ) {

        if ( v_change > 1 ) {
            coges_val_to_string( *v_credito, v_buffer );
            lcd_goto_xy(1,2);
            lcd_print_s( (unsigned char * ) "            \0" );
            lcd_goto_xy(1,2);
            lcd_print_s( (unsigned char * ) v_buffer );
            v_change    = 1;
        }

#ifdef E_CUSTOM_CHARSET        
        print_btns( E_CUSTOM_CHARSET_UP, E_CUSTOM_CHARSET_DOWN, E_CUSTOM_CHARSET_ENTER, BTN_BTNS );
        v_pulsante = wait_button();
        print_btns( E_CUSTOM_CHARSET_UP, E_CUSTOM_CHARSET_DOWN, E_CUSTOM_CHARSET_ENTER, BTN_BTNS );
#else
        print_btns( 0b01111111, 0b01111110, '#', BTN_BTNS );
        v_pulsante = wait_button();
        print_btns( 0b01111111, 0b01111110, '#', BTN_BTNS );
#endif

        switch ( v_pulsante ) {
            case    BTN__UP_PRESS:
                if ( *v_credito < 60000 ) {
                    *v_credito      += 500;
                    v_change         = 2;
                }
                break;
            case    BTN_DWN_PRESS:
                if ( *v_credito > 500 ) {
                    *v_credito      -= 500;
                    v_change         = 2;
                }
                break;
            case    BTN_ENT_PRESS:
                v_change = 0;
                break;
            case    BTN_ALL_FREE:
                break;
        }

    }

}

void exec_function( signed char *v_item ) {
    
    switch ( *v_item ) {
        case MENU_BACKLIGHT:
            if ( SR_PORTBbits.BACKLIGHT ) {
                iopins_set_backlight( 0x00 );
            } else {
                iopins_set_backlight( 0x01 );
            }
            break;
        case MENU_READ:
            read_credito( &v_credito, v_buffer );
            if ( p_24c02_error == M24C02_ERR_OK ) {
                print_credito( v_buffer );
                menu_pause();
            }
            lcd_clear_display();
            break;
        case MENU_REFILL:
            read_credito( &v_credito, v_buffer );
            if ( p_24c02_error == M24C02_ERR_OK ) {
                print_credito( v_buffer );
                sel_credito( &v_credito, v_buffer );
                write_credito( v_credito );
                menu_pause();
            }
            lcd_clear_display();
            break;
        case MENU_EXIT:
            
            pic_eeprom_write( STORE_BACKLIGHT_SETTING, C_TRISBbits.BACKLIGHT );
            
            iopins_set_backlight( 0x00 );
            lcd_display_onoff_control( LCD_D_DISP_OFF, LCD_C_CURS_OFF, LCD_B_BLINK_OFF );
            lcd_delay();
            C_TRISAbits.DATA        = 0b1111;
            C_TRISAbits.E           = 0b1;
            C_TRISAbits.RS          = 0b1;
            C_TRISAbits.RW          = 0b1; 
            C_TRISBbits.BTNS        = 0b111;
//            C_TRISBbits.BACKLIGHT   = 0b0;
            C_TRISBbits.BUS         = 0b11;
            
            SLEEP();
            break;
    }

    if ( p_24c02_error != M24C02_ERR_OK ) {
        print_err();
        p_24c02_error = M24C02_ERR_OK;
    }

}
    
//------------------------------------------------------------------------------

int main(int argc, char** argv) {

    //  Imposto tutti i pin di I/O come Input
    TRISA           = 0xFF;             //  Porta A INPUT
    TRISB           = 0xFF;             //  Porta B INPUT

    //  Global Interrupt Enable
    INTCONbits.GIE          =   0;      //  Global Interrupt Enable ( 1=Enable, 0=Disable )

    //  Pull up resistors PORT B
//    OPTION_REGbits.nRBPU    =   0;      //  Pull-Up Resistors Porta B

    //  Comparatori e Voltage reference
    CMCONbits.CM    = 0b111;            // Disabilito i comparatori
    VRCONbits.VREN  = 0;                // Disabilito Voltage Reference
    
#ifdef E_I2C
    i2c();
#endif

#ifdef E_LCD
    lcd_init();

    #ifdef E_CUSTOM_CHARSET
        lcd_set_char();
    #endif
#endif
    
#ifdef E_BUTTON
//    btn_init();
#endif

#ifdef E_IO
    iopins_init();
    iopins_set_backlight( pic_eeprom_read( STORE_BACKLIGHT_SETTING ) );
#endif
    
#ifdef E_INTERRUPT
    //--------------------------------------------------------------------------
    //  Abilito Interrupt
        INTCON                  = 0b00000000;	//  Azzero flag interrupt
    //            bit 7 GIE: Global Interrupt Enable bit
    //                1 = Enables all un-masked interrupts
    //                0 = Disables all interrupts
    //            bit 6 PEIE: Peripheral Interrupt Enable bit
    //                1 = Enables all un-masked peripheral interrupts
    //                0 = Disables all peripheral interrupts
    //            bit 5 T0IE: TMR0 Overflow Interrupt Enable bit
    //                1 = Enables the TMR0 interrupt
    //                0 = Disables the TMR0 interrupt
    //            bit 4 INTE: RB0/INT External Interrupt Enable bit
    //                1 = Enables the RB0/INT external interrupt
    //                0 = Disables the RB0/INT external interrupt
    //            bit 3 RBIE: RB Port Change Interrupt Enable bit
    //                1 = Enables the RB port change interrupt
    //                0 = Disables the RB port change interrupt
    //            bit 2 T0IF: TMR0 Overflow Interrupt Flag bit
    //                1 = TMR0 register has overflowed (must be cleared in software)
    //                0 = TMR0 register did not overflow
    //            bit 1 INTF: RB0/INT External Interrupt Flag bit
    //                1 = The RB0/INT external interrupt occurred (must be cleared in software)
    //                0 = The RB0/INT external interrupt did not occur
    //            bit 0 RBIF: RB Port Change Interrupt Flag bit
    //                1 = When at least one of the RB<7:4> pins changes state (must be cleared in software)
    //                0 = None of the RB<7:4> pins have changed state
        OPTION_REGbits.INTEDG	= 0;            // 	Interrupt on falling edge(1 is rising). Resistenze di pull-up sugli ingressi

        INTCONbits.INTE         = 0;            //  Disables the RB0/INT external interrupt
//        INTCONbits.INTE         = 1;            //  Enables the RB0/INT external interrupt

//        INTCONbits.RBIE         = 0;            //	Disables the RB port change interrupt
        INTCONbits.RBIE         = 1;            //	Enables the RB port change interrupt

//        INTCONbits.GIE          = 0;            //  Disables all interrupts    
        INTCONbits.GIE          = 1;            //  Enables all un-masked interrupts    
    //--------------------------------------------------------------------------

#endif

#ifndef NORMAL_PROG

    uchar v_addr = 0x00;

    lcd_clear_display();
    
    INTCONbits.GIE  = 0;    
    for (v_addr = 0; v_addr<8*CUSTOM_CHARS; v_addr++) {

        lcd_goto_xy(1,1);
        lcd_print_s( (unsigned char *) "EEPROM: 0x\0" );
        lcd_print_hex( v_addr );
        
        pic_eeprom_write( v_addr, a_custom_chars[v_addr] );
    }

    lcd_goto_xy(1,2);
    lcd_print_s( (unsigned char *) "Fatto.\0" );
    
#else
    
    signed char v_menu_item   = 0x00;
    uchar       v_pulsante    = 0x00;

    while(1) {
        
        print_display( &v_menu_item );

#ifdef E_CUSTOM_CHARSET        
        print_btns( E_CUSTOM_CHARSET_UP, E_CUSTOM_CHARSET_DOWN, E_CUSTOM_CHARSET_ENTER, BTN_BTNS );
        v_pulsante = wait_button();
        print_btns( E_CUSTOM_CHARSET_UP, E_CUSTOM_CHARSET_DOWN, E_CUSTOM_CHARSET_ENTER, BTN_BTNS );
#else
        print_btns( 0b01111111, 0b01111110, '#', BTN_BTNS );
        v_pulsante = wait_button();
        print_btns( 0b01111111, 0b01111110, '#', BTN_BTNS );
#endif

        switch ( v_pulsante ) {
            case    BTN__UP_PRESS:
                v_menu_item -= 1;
                break;
            case    BTN_DWN_PRESS:
                v_menu_item += 1;
                break;
            case    BTN_ENT_PRESS:
                exec_function( &v_menu_item );
                break;
            case    BTN_ALL_FREE:
                break;
        }
        if ( v_menu_item > MENU_MAX ) 
            v_menu_item = 0x00;
        if ( v_menu_item < 0x00 ) 
            v_menu_item = MENU_MAX;

    }

#endif  /* NORMAL_PROG */
    
    SLEEP();

    return (EXIT_SUCCESS);
}

