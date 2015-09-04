/* 
 * File:   main_menu.h
 * Author: Stefano
 *
 * Created on 6 luglio 2015, 21.36
 */

#ifndef MAIN_MENU_H
#define	MAIN_MENU_H

    #include "project.h"

    #define MENU_BACKLIGHT      0x00
    #define MENU_READ           0x01
    #define MENU_REFILL         0x02
    #define MENU_EXIT           0x03

    #define MENU_MIN            0x00
    #define MENU_MAX            0x03

#ifdef E_CUSTOM_CHARSET
    const uchar v_menu[5][12] = {    \
        "BackLight  \0",             \
        "Credito    \0",             \
        "Ricarica   \0",             \
        "Spegni     \0"              \
    };
#else
    const uchar v_menu[5][12] = {    \
        "BackLight  \0",             \
        "Credito    \0",             \
        "Ricarica   \0",             \
        "Spegni     \0"              \
    };
#endif
    
    #define menu_pause()     __delay_ms(2500)

    void print_btns( uchar btn_2, uchar btn_1, uchar btn_0, uchar btn_press );
    void print_display( signed char *v_menu_item );

#endif	/* MAIN_MENU_H */

