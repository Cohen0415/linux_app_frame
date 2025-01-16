/*
*
*   file: gpio.c
*   updata: 2025-01-16
*
*/

#include "bsp/gpio/gpio.h"

gpio_mes_t *gpio_init(const char *gpiochip, unsigned int gpionum, unsigned int mode, unsigned int level)
{
    ssize_t ret;
    
    if (gpiochip == NULL)
        return NULL;

    gpio_mes_t *gpio_mes = (gpio_mes_t *)malloc(sizeof(gpio_mes_t));
    if (gpio_mes == NULL)
        return NULL;

    gpio_mes->chip = gpiod_chip_open(gpiochip);  
    if (gpio_mes->chip == NULL)
    {
        perror("gpiod_chip_open");
        return NULL;
    }

    gpio_mes->line = gpiod_chip_get_line(gpio_mes->chip, gpionum);
    if (gpio_mes->line == NULL)
    {
        perror("gpiod_chip_get_line");
        return NULL;
    }

    if (mode)
    {
        ret = gpiod_line_request_output(gpio_mes->line, "line", (level?1:0));   
        if(ret < 0)
        {
            perror("gpiod_chip_get_line");
            return NULL;
        }
        fprintf(stdout, " [GPIO INIT] : %s pinnum%d, mode = output, default level = %d\n", gpiochip, gpionum, (level?1:0));
    }
    else
    {
        ret = gpiod_line_request_input(gpio_mes->line, "line");   
        if(ret < 0)
        {
            perror("gpiod_line_request_input");
            return NULL;
        }
        fprintf(stdout, " [GPIO INIT] : %s pinnum%d, mode = input\n", gpiochip, gpionum);
    }

    return gpio_mes;
}

void gpio_set_value(gpio_mes_t *gpio_mes, unsigned int level)
{
    if (gpio_mes == NULL)
        return;

    if (gpio_mes->chip == NULL || gpio_mes->line == NULL)
        return;

    gpiod_line_set_value(gpio_mes->line, (level?1:0));
}

int gpio_get_value(gpio_mes_t *gpio_mes)
{
    if (gpio_mes == NULL)
        return -EINVAL;

    if (gpio_mes->chip == NULL || gpio_mes->line == NULL)
        return -EINVAL;

    return gpiod_line_get_value(gpio_mes->line);
}

void gpio_exit(gpio_mes_t *gpio_mes)
{
    if (gpio_mes == NULL)
        return;

    if (gpio_mes->chip == NULL || gpio_mes->line == NULL)
        return;

    gpiod_line_release(gpio_mes->line);
    gpiod_chip_close(gpio_mes->chip);

    free(gpio_mes);
    gpio_mes = NULL;
}