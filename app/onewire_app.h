#ifndef ONEWIRE_H
#define ONEWIRE_H

#include <bsp_system.h>

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_Pin_12

#define DHT11_SET(n) GPIO_WriteBit(DHT11_PORT, DHT11_PIN, n)
#define DHT11_READ() GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)

typedef struct
{
    float temperature;
    float humidity;
} DHT11_data;

extern DHT11_data DHT11;

void DHT11_Init();
unsigned char DHT11_Read();
void show_temperature();
#endif
