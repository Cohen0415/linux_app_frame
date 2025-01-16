/*
*
*   file: gpio.h
*   updata: 2025-01-16
*
*/

#ifndef _GPIO_H
#define _GPIO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <gpiod.h>

typedef struct gpio_mes 
{
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    unsigned int mode;
}gpio_mes_t;

gpio_mes_t *gpio_init(const char *gpiochip, unsigned int gpionum, unsigned int mode, unsigned int level);
void gpio_set_value(gpio_mes_t *gpio_mes, unsigned int level);
int gpio_get_value(gpio_mes_t *gpio_mes);
void gpio_exit(gpio_mes_t *gpio_mes);

#endif