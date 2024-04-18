# tamagochi.c
# Descripción del Código

Este código está diseñado para controlar un LCD de 16x2 y botones conectados a un microcontrolador Raspberry Pi Pico. Utiliza la librería Pico SDK para interactuar con el hardware y mostrar información en el LCD. Además, permite la interacción con los botones para seleccionar opciones en el LCD.

## Configuración del Hardware

El código requiere la siguiente configuración de hardware:

- Conexión de un LCD de 16x2 al bus I2C utilizando los pines GPIO 4 (SDA) y GPIO 5 (SCL) del Raspberry Pi Pico.
- Conexión de botones a los pines GPIO definidos como `BUTTON_1`, `BUTTON_2`, `BUTTON_3` y `BUTTON_4`.

## Funcionamiento

El programa inicializa el LCD y los botones, luego muestra un menú en el LCD con opciones definidas en el arreglo `message`. El usuario puede navegar por las opciones utilizando los botones, y al presionar el botón `BUTTON_3`, se activa la selección de la opción.

El código utiliza la detección de botones para controlar el desplazamiento por las opciones del menú y la selección de una opción específica. Además, incluye la capacidad de mostrar un personaje especial "ajolote" en el LCD.

## Componentes Especiales

- El arreglo `ajolote_N` define un personaje especial "ajolote" en formato de matriz de bytes para ser mostrado en el LCD.
- El archivo `Config.h` contiene definiciones de pines y constantes utilizadas en el código.

## Uso

1. Configura tu entorno de desarrollo y asegúrate de tener los drivers y herramientas necesarias para programar la Raspberry Pi Pico.
2. Conecta el LCD de 16x2 y los botones según las especificaciones de pines definidas en el código y en el archivo `Config.h`.
3. Compila y carga el código en la Raspberry Pi Pico.
4. Ejecuta el programa y utiliza los botones para navegar por las opciones del menú en el LCD.
5. Selecciona una opción presionando el botón `BUTTON_3` para realizar una acción asociada a esa opción.

## Notas

- Puedes modificar las opciones del menú y agregar más funcionalidades según tus necesidades utilizando los botones y el LCD.
- Asegúrate de seguir las medidas de seguridad al manipular componentes electrónicos y al utilizar la Raspberry Pi Pico.

¡Disfruta de la interacción con el LCD y los botones utilizando tu Raspberry Pi Pico!

# Config.h
# Descripción del Código

Este código está diseñado para controlar un LCD de 16x2 y mostrar información en él utilizando un microcontrolador Raspberry Pi Pico. Utiliza la librería Pico SDK y la interfaz I2C para comunicarse con el LCD y enviar comandos de visualización y control.

## Configuración del Hardware

El código requiere la siguiente configuración de hardware:

- Conexión de un LCD de 16x2 al bus I2C utilizando los pines GPIO 4 (SDA) y GPIO 5 (SCL) del Raspberry Pi Pico.
- Configuración de la alimentación del LCD con 3.3V y conexión del GND.

## Funcionamiento

El programa inicializa el LCD y define varios comandos y funciones para enviar datos y controlar el LCD, incluyendo la creación de caracteres personalizados (como el "ajolote"). También define un arreglo de mensajes (`message`) que se pueden mostrar en el LCD como opciones de un menú.

El código incluye funciones para controlar el cursor, limpiar la pantalla, enviar caracteres y cadenas de texto al LCD, crear caracteres personalizados, y más. Además, tiene una función `actividades` que muestra un menú en el LCD y ejecuta acciones dependiendo de la opción seleccionada.

## Componentes Especiales

- Se definen caracteres especiales "ajolote" en formato de matriz de bytes para ser mostrados en el LCD como parte de la función `mostrar_ajolote`.
- Se utiliza un arreglo de mensajes (`message`) para definir opciones de un menú en el LCD.

## Uso

1. Configura tu entorno de desarrollo y asegúrate de tener los drivers y herramientas necesarias para programar la Raspberry Pi Pico.
2. Conecta el LCD de 16x2 al bus I2C y a la alimentación según las especificaciones de pines definidas en el código.
3. Compila y carga el código en la Raspberry Pi Pico.
4. Ejecuta el programa y observa cómo se muestra la información en el LCD, incluyendo los caracteres personalizados y las opciones del menú.
5. Utiliza los botones o la lógica del programa para interactuar con las opciones del menú y ejecutar las acciones correspondientes.

## Notas

- Puedes personalizar los mensajes del menú (`message`) y agregar más funcionalidades al código según tus necesidades.
- Asegúrate de seguir las medidas de seguridad al manipular componentes electrónicos y al utilizar la Raspberry Pi Pico.

¡Disfruta de la interacción con el LCD y la creación de caracteres personalizados con tu Raspberry Pi Pico!
