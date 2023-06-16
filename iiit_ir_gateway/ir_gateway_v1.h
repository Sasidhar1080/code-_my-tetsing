#ifndef __ESP_IR_GATEWAY_V1_H__
#define __ESP_IR_GATEWAY_V1_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NRF_RST_PIN					32
/* Swapped for uart communication. */
#define NRF_TX_PIN					25
#define NRF_RX_PIN					33
#define NRF_INT_PIN					26
#define SW_USR_PIN             		27  // User Switch Pin
#define SDA_PIN                    	16  // TWI Data
#define SCL_PIN                    	17  // TWI Clock
#define RTC_MFP_PIN                 4 	// RTC Multifunction pin.
/* Swapped for uart communication. */
#define CLI_TX_PIN					5	// CLI Uart Tx Pin.
#define CLI_RX_PIN					18	// CLI Uart Rx Pin.

#define IR_INT2_PIN					19	//MAXQ6 IR Sw int pins.
#define IR_INT1_PIN					21	//MAXQ6 IR Sw int pins.
#define IR_INT0_PIN					22	//MAXQ6 IR Sw int pins.
#define IR_RST_PIN					23	//MAXQ6 RST Pin.

#ifdef __cplusplus
}
#endif

#endif
