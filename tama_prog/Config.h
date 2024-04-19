#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <string.h>
#include "hardware/i2c.h"
#include "pico/binary_info.h"

/*
   GPIO 4 (pin 6)-> SDA on LCD bridge board
   GPIO 5 (pin 7)-> SCL on LCD bridge board
   3.3v (pin 36) -> VCC on LCD bridge board
   GND (pin 38)  -> GND on LCD bridge board
*/
// commands

const int CGRAM_BASE = 0x40;

const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;


// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_CURSOROFF = 0x00;
const int LCD_DISPLAYON = 0x04;
const int LCD_BLINKOFF = 0x00;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

const int CUSTOM_CHAR_ADDRS[] = { CGRAM_BASE, CGRAM_BASE + 8, CGRAM_BASE + 16, CGRAM_BASE + 24 };

// By default these LCD display drivers are on bus address 0x27
static int addr = 0x27;

// Modes for lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

#define Zumbador       26

/* creamos los botones a usar */
#define BUTTON_1 (0)
#define BUTTON_2 (1)
#define BUTTON_3 (2)
#define BUTTON_4 (3)

int botones[4] = {
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4
};

uint8_t ajolote_N[4][8] = {
{
    0b00100, 0b00100, 0b10110, 0b10111, 0b11100, 0b01000, 0b01010, 0b11010
},
{
    0b00100, 0b00100, 0b01101, 0b11101, 0b00111, 0b00010, 0b01010, 0b01011
},
{
    0b11001, 0b01000, 0b00111, 0b01001, 0b01010, 0b01010, 0b00100, 0b00000
},
{
    0b10011, 0b00010, 0b11100, 0b10010, 0b01010, 0b01010, 0b00100, 0b00000
}
};

uint8_t ajolote_C[4][8] = {
{
    0b00100, 0b00100, 0b10110, 0b10111, 0b11100, 0b01010, 0b01000, 0b11011
},
{
    0b00100, 0b00100, 0b01101, 0b11101, 0b00111, 0b01010, 0b00010, 0b11011
},
{
    0b11010, 0b01001, 0b00111, 0b01001, 0b01010, 0b01010, 0b00100, 0b00000
},
{
    0b01011, 0b00101, 0b11100, 0b10010, 0b01010, 0b01010, 0b00100, 0b00000
}
};

uint8_t ajolote_D[4][8] = {
{
    0b00100, 0b00100, 0b10110, 0b10111, 0b11100, 0b01000, 0b01000, 0b11010
},
{
    0b00100, 0b00100, 0b01101, 0b11101, 0b00111, 0b00010, 0b00010, 0b01011
},
{
    0b11000, 0b01000, 0b00111, 0b01001, 0b01110, 0b00000, 0b00000, 0b00000
},
{
    0b00011, 0b00010, 0b11100, 0b10010, 0b01110, 0b00000, 0b00000, 0b00000
}
};

uint8_t ajolote_T[4][8] = {
{
    0b00100, 0b00100, 0b10110, 0b10111, 0b11100, 0b01000, 0b01010, 0b11010
},
{
    0b00100, 0b00100, 0b01101, 0b11101, 0b00111, 0b00010, 0b01010, 0b01011
},
{
    0b11001, 0b01000, 0b00111, 0b01001, 0b01010, 0b01010, 0b00100, 0b00000
},
{
    0b10011, 0b00010, 0b11100, 0b10010, 0b01010, 0b01010, 0b00100, 0b00000
}
};
uint8_t ajolote_J[4][8] = {
{
    0b00100, 0b00100, 0b10110, 0b10111, 0b11100, 0b01000, 0b01010, 0b11010
},
{
    0b00100, 0b00100, 0b01101, 0b11101, 0b00111, 0b00010, 0b01010, 0b01011
},
{
    0b11001, 0b01000, 0b00111, 0b01001, 0b01010, 0b01010, 0b00100, 0b00000
},
{
    0b10011, 0b00010, 0b11100, 0b10010, 0b01010, 0b01010, 0b00100, 0b00000
}
};

int sonidos[2][8] = {
    {1, 1, 0, 1, 0, 1, 0, 0},{1, 0, 0, 0, 0, 0, 1, 1}
    };

int tiempos[8] = {
  10, 30, 10, 30, 10, 30, 10, 10
};

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void create_custom_chars(uint8_t ajolote[4][8]) {
    // Crear cada uno de los 4 caracteres especiales
    for (int i = 0; i < 4; i++) {
        // Establecer la dirección de la CGRAM para el caracter especial actual
        lcd_send_byte(LCD_SETCGRAMADDR | (i * 8), LCD_COMMAND);

        // Enviar los datos del caracter especial a la CGRAM
        for (int j = 0; j < 8; j++) {
            lcd_send_byte(ajolote[i][j], LCD_CHARACTER);
        }
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

void mostrar_ajolote(){
    lcd_set_cursor(0, 0);  // Colocar el cursor en la posición deseada
    lcd_char(0); 
    lcd_char(1);
    lcd_set_cursor(1,0);
    lcd_char(2);
    lcd_char(3); 
}

void actividades(char *opcion){
            mostrar_ajolote();
            lcd_set_cursor(0,((MAX_CHARS - 2) / 2 - (strlen(opcion) / 2)));
            lcd_string(opcion);
}

    static char *message[] =
            {
                    "*Comer", "*Dormir",
                    "*Tocar", "*Jugar",
                    "*Cambio", "*Apagar"
            };

void zumbador(int seleccion){
    gpio_init(Zumbador);
    gpio_set_dir(Zumbador, GPIO_OUT);
    for (int i = 0; i < 8; i++)
        {
            int32_t mask = sonidos[seleccion][i] << Zumbador;
            gpio_set_mask(mask);
            sleep_ms(tiempos[i]);
            gpio_clr_mask(mask);
        }        
}
