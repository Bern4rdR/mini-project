/*
 * This file will read and handle the gyro data and translate it into instructions
 * that can be used to control the game. Tilting the gyro will make the player
 * turn in that direction.
 */
#include <plib.h> // PIC32 peripheral library
#include "pic32mx.h"


#define POTENTIONMETER_PIN 0


voind setup() {
    // ensure that ADC is off before setting the configuration
    CloseADC10();

    #define PARAM1 ADC_MODULE_ON
                 | ADC_FORMAT_INTG
                 | ADC_CLK_AUTO
                 | ADC_AUTO_SAMPLING_ON
    #define PARAM2 ADC_VREF_AVDD_AVSS
                 | ADC_OFFSET_CAL_DISABLE
                 | ADC_SCAN_ON
                 | ADC_SAMPLES_PER_INT_16
                 | ADC_ALT_BUF_OFF
                 | ADC_ALT_INPUT_OFF
    #define PARAM3 ADC_CONV_CLK_INTERNAL_RC
                 | ADC_SAMPLE_TIME_15
    #define PARAM4 ENABLE_AN0_ANA
    #define PARAM5 SKIP_SCAN_ALL

    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF
               | ADC_CH0_POS_SAMPLEA_AN0);
    OpenADC10(PARAM1, PARAM2, PARAM3, PARAM4, PARAM5);

    // enable the ADC again
    EnableADC10();

    // configure the UART for 9600 baud rate communication
    UARTConfigure(UART1, UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS
                     | UART_PARITY_NONE
                     | UART_STOP_BITS_1);
    UARTSetDataRate(UART1, GetPeripheralClock(), 9600);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL
                                      | UART_RX
                                      | UART_TX));
}

void loop() {
    while(!mAD1GetIntFlag()) {
        // wait for the first conversion to complete
        // so there will be valid data in ADC result registers
    }
    int data = ReadADC10(0);

    int percentage = data / 1023 * 100;

    char buffer[10];
    sprintf(buffer, "Potentiometer at %d%%\n", percentage);
    putsUART1(buffer);

    delay(100);
}

int mian() {
    setup();
    while(1) {
        loop();
    }
}