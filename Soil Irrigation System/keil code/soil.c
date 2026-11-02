#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>

/* ---------------- CONFIGURATION ---------------- */

#define SOIL_THRESHOLD 4024

volatile uint16_t soil = 0;
volatile uint8_t soil_ready = 0;

int temperature = -1;
int humidity = -1;


/* ---------------- SYSTICK INITIALIZATION ---------------- */

void SysTick_Init(void)
{
    SysTick->CTRL = 0;
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL = 0;

    /* Select processor clock */
    SysTick->CTRL |= (1 << 2);

    /* Enable SysTick counter */
    SysTick->CTRL |= (1 << 0);
}


/* ---------------- DELAY FUNCTIONS ---------------- */

void delay_us(unsigned int us)
{
    unsigned int ticks = us * (SystemCoreClock / 1000000U);

    unsigned int start = SysTick->VAL;
    unsigned int elapsed;

    while (1)
    {
        unsigned int current = SysTick->VAL;

        if (current <= start)
        {
            elapsed = start - current;
        }
        else
        {
            elapsed = start + (SysTick->LOAD + 1U - current);
        }

        if (elapsed >= ticks)
        {
            break;
        }
    }
}

void delay_ms(unsigned int ms)
{
    for (unsigned int i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}


/* ---------------- USART2 INITIALIZATION ---------------- */
/* PA2 = USART2_TX, 9600 baud at 16 MHz */

void USART2_Init(void)
{
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 17);

    /* PA2 alternate function mode */
    GPIOA->MODER &= ~(3 << 4);
    GPIOA->MODER |= (2 << 4);

    /* AF7 for PA2 */
    GPIOA->AFR[0] &= ~(15 << 8);
    GPIOA->AFR[0] |= (7 << 8);

    /* USART2 configuration */
    USART2->CR1 = 0;
    USART2->BRR = 0x0683;
    USART2->CR1 |= (1 << 3);
    USART2->CR1 |= (1 << 13);
}


/* ---------------- USART2 TRANSMISSION ---------------- */

void send_char(char c)
{
    while (!(USART2->SR & (1 << 7)));

    USART2->DR = c;
}

void send_string(char *str)
{
    while (*str)
    {
        send_char(*str++);
    }
}

void send_number(int num)
{
    char buffer[12];

    sprintf(buffer, "%d", num);

    send_string(buffer);
}


/* ---------------- ADC1 INITIALIZATION ---------------- */
/* PA0 = ADC1_IN0 */

void ADC1_Init(void)
{
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB2ENR |= (1 << 8);

    /* PA0 analog mode */
    GPIOA->MODER &= ~(3 << 0);
    GPIOA->MODER |= (3 << 0);

    /* ADC prescaler: PCLK2 / 4 */
    ADC->CCR &= ~(3 << 16);
    ADC->CCR |= (1 << 16);

    /* Configure ADC1 */
    ADC1->CR1 = 0;
    ADC1->CR2 = 0;

    /* One conversion, channel 0 */
    ADC1->SQR1 = 0;
    ADC1->SQR3 = 0;

    /* Long sample time for channel 0 */
    ADC1->SMPR2 &= ~(7 << 0);
    ADC1->SMPR2 |= (7 << 0);

    /* Enable end-of-conversion interrupt */
    ADC1->CR1 |= (1 << 5);

    /* Enable ADC1 */
    ADC1->CR2 |= (1 << 0);

    /* Clear pending interrupt and enable NVIC */
    NVIC_ClearPendingIRQ(ADC_IRQn);
    NVIC_EnableIRQ(ADC_IRQn);
}


/* ---------------- ADC INTERRUPT HANDLER ---------------- */

void ADC_IRQHandler(void)
{
    if (ADC1->SR & (1 << 1))
    {
        /* Read conversion result */
        soil = (uint16_t)ADC1->DR;

        /* Notify main loop */
        soil_ready = 1;
    }
}


/* ---------------- BUZZER INITIALIZATION ---------------- */
/* PB9 = buzzer signal */

void Buzzer_Init(void)
{
    RCC->AHB1ENR |= (1 << 1);

    /* Set PB9 LOW before configuring output */
    GPIOB->BSRR = (1 << 25);

    /* PB9 output mode */
    GPIOB->MODER &= ~(3 << 18);
    GPIOB->MODER |= (1 << 18);

    /* No pull-up or pull-down */
    GPIOB->PUPDR &= ~(3 << 18);
}


/* ---------------- BUZZER CONTROL ---------------- */
/* Active-HIGH buzzer module */

void buzzer_control(uint16_t value)
{
    if (value < SOIL_THRESHOLD)
    {
        GPIOB->BSRR = (1 << 9);
    }
    else
    {
        GPIOB->BSRR = (1 << 25);
    }
}


/* ---------------- DHT11 WAIT FUNCTION ---------------- */
/* Wait until PB8 reaches the requested state.
   Returns 1 on success and 0 on timeout. */

int wait_pin(uint8_t state, unsigned int timeout_us)
{
    while (((GPIOB->IDR & (1 << 8)) ? 1U : 0U) != state)
    {
        if (timeout_us == 0)
        {
            return 0;
        }

        delay_us(1);
        timeout_us--;
    }

    return 1;
}


/* ---------------- DHT11 READING FUNCTION ---------------- */

int read_dht11(int *temp, int *hum)
{
    uint8_t data[5] = {0};

    /* Configure PB8 as output */
    GPIOB->MODER &= ~(3 << 16);
    GPIOB->MODER |= (1 << 16);

    /* Send start signal: LOW for at least 18 ms */
    GPIOB->BSRR = (1 << 24);
    delay_ms(20);

    /* Release data line HIGH */
    GPIOB->BSRR = (1 << 8);
    delay_us(30);

    /* Switch PB8 to input */
    GPIOB->MODER &= ~(3 << 16);

    /* Wait for DHT11 response: LOW, HIGH, LOW */
    if (!wait_pin(0, 100))
        return 0;

    if (!wait_pin(1, 100))
        return 0;

    if (!wait_pin(0, 100))
        return 0;

    /* Read 40 bits */
    for (int i = 0; i < 40; i++)
    {
        /* Wait for the start of the HIGH pulse */
        if (!wait_pin(1, 100))
            return 0;

        /* Sample during the HIGH pulse */
        delay_us(40);

        data[i / 8] <<= 1;

        if (GPIOB->IDR & (1 << 8))
        {
            data[i / 8] |= 1;
        }

        /* Wait for the HIGH pulse to finish */
        if (!wait_pin(0, 100))
            return 0;
    }

    /* Verify checksum */
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) != data[4])
    {
        return 0;
    }

    /* DHT11 integer humidity and temperature */
    *hum = data[0];
    *temp = data[2];

    return 1;
}


/* ---------------- MAIN PROGRAM ---------------- */

int main(void)
{
    /* Update system clock information */
    SystemCoreClockUpdate();

    /* Initialize SysTick instead of DWT */
    SysTick_Init();

    /* Enable GPIOB clock */
    RCC->AHB1ENR |= (1 << 1);

    /* Configure PB8 input with pull-up */
    GPIOB->MODER &= ~(3 << 16);
    GPIOB->PUPDR &= ~(3 << 16);
    GPIOB->PUPDR |= (1 << 16);

    /* Initialize peripherals */
    Buzzer_Init();
    USART2_Init();
    ADC1_Init();

    while (1)
    {
        /* Start one ADC conversion */
        soil_ready = 0;
        ADC1->SR = 0;
        ADC1->CR2 |= (1 << 30);

        /* Wait for the ADC interrupt */
        while (soil_ready == 0)
        {
            /* ADC_IRQHandler handles the conversion */
        }

        /* Control buzzer using the latest soil value */
        buzzer_control(soil);

        /* Read DHT11 */
        if (read_dht11(&temperature, &humidity) == 0)
        {
            temperature = -1;
            humidity = -1;
        }

        /* Send data to ESP8266 */
        send_string("SOIL:");
        send_number(soil);

        send_string(",TEMP:");
        send_number(temperature);

        send_string(",HUM:");
        send_number(humidity);

        send_string("\r\n");

        /* Wait before the next reading */
        delay_ms(2000);
    }
}