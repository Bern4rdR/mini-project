/*
 * This file will read and handle the gyro data and translate it into instructions
 * that can be used to control the game. Tilting the gyro will make the player
 * turn in that direction.
 */
#include <plib.h> // PIC32 peripheral library
#include "pic32mx.h"


// pins
#define POTENTIONMETER_PIN 0
#define BUTTON_PIN 1
// math
#define PI 3.1415926535
#define P2 PI/2


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

    // configure the UART interrupts
    INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);

    // enable interrupts
    INTEnableSystemMultiVectoredInt();
    
    // configure the ADC interrupt
    INTEnable(INT_AD1, INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_AD1, INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_AD1, INT_SUB_PRIORITY_LEVEL_0);
}

/* Update the player direction and walking status based on the inputs
 * @param playerDirection: pointer to the player's direction
 * @param walking:         pointer to the player's walking status
 */
void get_inputs(float* playerDirection, bool* walking) {
    if(!mAD1GetIntFlag()) {
        // wait for the first conversion to complete
        // so there will be valid data in ADC result registers
        return;
    }
    // read potentiometer data
    int data = ReadADC10(0);
    // read button data
    *button = PORTD & (1 << BUTTON_PIN);

    // change potentiometer to rotation
    float rotation = data / 1023 * 2 * PI;

    // transform rotation to intuitively align it with the potentiometer
    // currently:  turning the potentiometer clockwise will turn the player counter-clockwise
    //             having the potentiometer in the middle will point the player left on the map (pi radians)
    // desired:    turning the potentiometer clockwise will turn the player clockwise
    //             having the potentiometer in the middle will point the player up on the map (pi/2 radians)
    rotation = P2 - rotation;

    // clamp rotation to 0-2*PI
    if (rotation < 0) {
        rotation += 2*PI;
    }
    if (rotation > 2*PI) {
        rotation -= 2*PI;
    }

    // set the player direction
    *playerDirection = rotation;

    char buffer[10];
    sprintf(buffer, "Potentiometer at %d%%\n", rotation);
    putsUART1(buffer);
}

int mian() {
    setup();
    while(1) {
        loop();
    }
}