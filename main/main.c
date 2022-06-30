/* Program that fade on and then fade out ina led using pwm control by hardware.
 */

#include <stdio.h>
#include "driver/ledc.h"       //Driver para controlar el ancho de pulso
#include "driver/gpio.h"       //Driver para configurar los pines
#include "freertos/FreeRTOS.h" //Driver para manejo de funciones de FreeRTOS
#include "freertos/task.h"     //Driver para manejo de funciones FreeRTOS
#include "esp_err.h"           //Driver para manejo de errores

void configura_pwm(void)
{
    // Paso 1 Configuramos el timer
    // Definimos la estructura para configurar el timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_13_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 5000,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    // Configuramos al timer usando la estructura preparada
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Paso 2 Preparamos ahora el canal en el que usaremos PWM
    // Definimos la estructura paa la función ledc_channel_config
    ledc_channel_config_t ledc_canal = {
        .gpio_num = GPIO_NUM_2, // Usaremos el control de ancho de pulso el pin 2
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,      // iniciamos con un ciclo de trabajo de cero
        .hpoint = 8191, //
    };

    // Ahora configuramos el canal con ledc_channel_config usando la estructura
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_canal));
}

void app_main(void)
{

    configura_pwm();                                       // Configuramos las opciones para aplicar el pwm
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 8191); // Asegurar led encendido
    ledc_fade_func_install(0);                             // ACtivamos las funciones de pwm por hardware

    while (1)
    {
        // Preparamos la ejecución del fade off
        ledc_set_fade_with_step(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 0, 20, 100);
        // Ejecutamos el fade off
        ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_WAIT_DONE);
        // Preparamos la ejecución del fade on
        ledc_set_fade_with_step(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 8191, 40, 100);
        // Ejecutamos el fade off
        ledc_fade_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, LEDC_FADE_WAIT_DONE);
        
        vTaskDelay(200/portTICK_PERIOD_MS);
    }
}