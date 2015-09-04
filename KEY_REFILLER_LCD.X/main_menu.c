#include "main_menu.h"

    void print_btn( uchar btn, uchar btn_press ) {

        if ( btn_press != 0 ) {
            lcd_write_data( btn );
        } else {
            lcd_write_data( '_' );
        }

    }

    void print_btns( uchar btn_2, uchar btn_1, uchar btn_0, uchar btn_press ) {

        lcd_goto_xy(14,2);
        print_btn( btn_2, btn_press & BTN__UP_PRESSED );
        print_btn( btn_1, btn_press & BTN_DWN_PRESSED );
        print_btn( btn_0, btn_press & BTN_ENT_PRESSED );

    }

    void print_menu( signed char *v_item ) {
        signed char v_ind   = 0x00;
        
        for ( v_ind=0x00; v_ind<0x02; v_ind++ ) {
            lcd_goto_xy( 1, v_ind+1 );
            if ( (*v_item | v_ind) <= MENU_MAX ) {
                if ( (*v_item & 0b001) == v_ind ) {
                    lcd_write_data( '>' );
                } else {
                    lcd_write_data( ' ' );
                }
                lcd_print_s( (unsigned char *) v_menu[ ( ( *v_item & 0b11111110 ) | v_ind ) ] );
            } else {
                lcd_print_s( (unsigned char *) "            \0" );
            }
        }

    }

    void print_display( signed char *v_menu_item ) {

        lcd_goto_home();

        print_menu( v_menu_item );


//        lcd_goto_xy( 14, 2 );

//        print_btns( '<', '>', '*', BTN_BTNS );
//        print_btns( 0b01111111, 0b01111110, '#', BTN_BTNS );
//        print_btns( 0x03, 0x02, '#', BTN_BTNS );

    }

