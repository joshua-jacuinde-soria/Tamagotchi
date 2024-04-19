#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <string.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"
#include "Config.h"

/*
   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
// commands

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

/* creamos los botones a usar */
#define BUTTON_1 (0)
#define BUTTON_2 (1)
#define BUTTON_3 (2)
#define BUTTON_4 (3)


int main() {
    //stdio_init_all();
                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    /* inicializaacion de botones */
    for (int i = 0; i < 4; i++)
    {
        gpio_init(botones[i]);
        gpio_set_dir(botones[i], GPIO_IN);
        gpio_pull_up(botones[i]);
    }

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
    #warning i2c/lcd_1602_i2c example requires a board with I2C pins
#else
    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();

    create_custom_chars(ajolote_N);

    while (1) {
        lcd_clear();
        int linea = 0;
        int arreglo = 0;
        lcd_set_cursor(0, (MAX_CHARS) - strlen(message[0]));
        lcd_string(message[0]);
        lcd_set_cursor(1, (MAX_CHARS) - strlen(message[1]));
        lcd_string(message[1]);

        mostrar_ajolote();
        while (gpio_get(BUTTON_3))
        {
            lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON, LCD_COMMAND);
            lcd_set_cursor(linea,((MAX_CHARS) - strlen(message[arreglo]))); // Colocar el cursor en la posición deseada
            if (!gpio_get(BUTTON_1)){
              linea ++;
              arreglo ++;
              zumbador(0);
            }

            if (linea == 2 ){
                linea = 0;
                if (arreglo == 6){
                    arreglo = 0;
                    }
                lcd_clear();
                mostrar_ajolote();
                lcd_set_cursor(0, (MAX_CHARS) - strlen(message[arreglo]));
                lcd_string(message[arreglo]);
                lcd_set_cursor(1, (MAX_CHARS) - strlen(message[arreglo + 1]));
                lcd_string(message[arreglo + 1]);
              }

            if (!gpio_get(BUTTON_2)){
              linea = linea -1;
              arreglo = arreglo - 1;
              zumbador(0);
            }

            if (linea < 0 ){
                linea = 1;
                if (arreglo < 0){
                    arreglo = 5;
                    }
                lcd_clear();
                mostrar_ajolote();
                lcd_set_cursor(0, (MAX_CHARS) - strlen(message[arreglo - 1]));
                lcd_string(message[arreglo - 1]);
                lcd_set_cursor(1, (MAX_CHARS) - strlen(message[arreglo]));
                lcd_string(message[arreglo]);
              }
            sleep_ms(120);
        }
        static char *opcion;
        lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF, LCD_COMMAND);
        lcd_clear();
        switch (arreglo)
        {
        case 0:
            zumbador(0);
            opcion = message[0];
            actividades(opcion);
            static char *text = "Enter to feed";
            lcd_set_cursor(1, (MAX_CHARS - strlen(text)));
            lcd_string(text);
            while (gpio_get(BUTTON_4))
            {
                if (!gpio_get(BUTTON_3))
                {
                    zumbador(1);
                    create_custom_chars(ajolote_C);
                    mostrar_ajolote();
                    sleep_ms(500);

                }
                create_custom_chars(ajolote_N);
                mostrar_ajolote();
                
            }
            zumbador(0);
            break;            
        case 1:
            zumbador(0);
            opcion = message[1];
            actividades(opcion);
            create_custom_chars(ajolote_D);
            mostrar_ajolote();
            text = "Enter to wake";
            lcd_set_cursor(1, (MAX_CHARS - strlen(text)));
            lcd_string(text);
            int cont = 0;
            while (gpio_get(BUTTON_4))
            {
                while (cont < 5)
                {
                    if (!gpio_get(BUTTON_3))
                    {
                        zumbador(1);
                        cont ++;
                    }                    
                }
                create_custom_chars(ajolote_N);
                mostrar_ajolote();
                text = "     Is awake";
                lcd_set_cursor(1, (MAX_CHARS - strlen(text)));
                lcd_string(text);
            }
            zumbador(0);
            break; 
        case 2:
            opcion = message[2];
            break;
        case 3:
            opcion = message[3];
            break;
        case 4:
            opcion = message[4];
            break;
        case 5:
            opcion = message[5];

            break;         
        default:
            break;
        }  
    }
#endif
}

