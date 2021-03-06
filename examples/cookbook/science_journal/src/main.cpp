/*
===============================================================================
 Name        : Science Journal
 Author      : uCXpresso
 Version     : v1.0.0
 Copyright   : www.ucxpresso.net
 License	 : MIT
 Description : A BLE UART to Google Science Journal
===============================================================================
 	 	 	 	 	 	 	 	 History
 ---------+---------+--------------------------------------------+-------------
 DATE     |	VERSION |	DESCRIPTIONS							 |	By
 ---------+---------+--------------------------------------------+-------------
 2016/7/20 v1.0.0	First Edition.									Jason
 ===============================================================================
 */

#include <uCXpresso.h>

#ifdef DEBUG
#include <debug.h>
#include <class/serial.h>
#define DBG dbg_printf
#else
#define DBG(...)
#endif

// TODO: insert other include files here
#include <class/ble/ble_device.h>
#include <class/ble/ble_service.h>
#include <class/pin.h>

#include <class/ble/ble_service_uart.h>
#include <class/ble/ble_conn_params.h>
#include <class/timeout.h>

// TODO: insert other definitions and declarations here
#define DEVICE_NAME                          "Science"            /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                    "uCXpresso.NRF"     /**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_INTERVAL                     50                  /**< The advertising interval (in ms). */
#define APP_COMPANY_IDENTIFIER           	 0x0059				 /**< Company identifier for Nordic Semi. as per www.bluetooth.org. */

//#define BOARD_LILYPAD
#define BOARD_NANO51822_UDK
#include <config/board.h>

const ble_uuid128_t science_base_uuid = {
    0xE8, 0x74, 0x2C, 0x65, 0xF0, 0x01, 0x38, 0x95,
	0x7A, 0x46, 0xAA, 0x0A, 0x00, 0x00, 0x5A, 0x55
};

#define UUID_SCIENCE_SERVICE	0x0001
#define UUID_SCIENCE_VALUE		0x0003	// Output (TXD)
#define UUID_SCIENCE_CONFIG		0x0010	// Input (RXD)

#include "config_change.h"
#include "goosci_utility.h"
#include <class/adc.h>
#include <utils/utils.h>

//
// Main Routine
//
int main(void) {
	CSerial ser;		// declare a Serial object
	ser.enable();

#ifdef DEBUG
	CDebug dbg(ser);	// Debug stream use the UART object
	dbg.start();
//	dbg.waitToDebugMode();
#endif

	//
	// SoftDevice
	//
	bleDevice ble;	// use internal 32.768KHz for SoftDevice
	ble.enable();	// enable BLE SoftDevice stack

	// GAP
	ble.m_gap.settings(DEVICE_NAME, 20, 50);	// set Device Name on GAP
	ble.m_gap.tx_power(BLE_TX_0dBm);	// set Output power

	//
	// Add BLE UART Service
	//
	bleServiceUART	bleScience( ble,
								false,
								&science_base_uuid,
								UUID_SCIENCE_SERVICE,
								UUID_SCIENCE_VALUE,
								UUID_SCIENCE_CONFIG
								);

	//
	// Add "connection parameters update" negotiation. (optional)
	//
	bleConnParams conn(ble);

	//
	// BLE Advertising
	//
//	int8_t const txPowerLevel = -55;
	ble.m_advertising.commpany_identifier(APP_COMPANY_IDENTIFIER);	// add company identifier
//	ble.m_advertising.tx_power_level(&txPowerLevel);
	ble.m_advertising.add_uuid_to_complete_list(BLE_UUID_DEVICE_INFORMATION_SERVICE);
	ble.m_advertising.add_uuid_to_complete_list(bleScience);

	ble.m_advertising.update();										// update advertising data

	// Start advertising
	ble.m_advertising.interval(APP_ADV_INTERVAL);					// set advertising interval
	ble.m_advertising.start();

	//
	// Your Application setup code here
	//

	CPin led1(LED1);	// led0 on P0.18
	CPin led2(LED2);	// led1 on P0.19

	led1.output();	// set led0 as an output pin
	led2.output();	// set led1 as an output pin

	//
	// Initialize & enable the ADC core
	//
	CAdc::init();
	CAdc::enable();

	//
	CTimeout	tm, period;
	uint8_t		buffer[64];
	uint16_t 	size, value;
	uint16_t 	sensorValue = 0;
	//
    // Enter main loop.
	//
    while(1) {
    	if ( bleScience.isAvailable() ) {	// check BLE NUS service
    		// receive configrations
    		size = bleScience.readable();
    		if ( size > 0 ) {
    			bleScience.read(buffer, size);
    			handle(buffer, size);
    		}

    		// send data at every second
    		if ( period.isExpired(1000) ) {
    			period.reset();
				if ( pin_type == ANALOG ) {
					if ( pin <= 5) { // AD0 - AD5
						CAdc::pin(analog_pin[pin+1]);
						CAdc::read(value);
						sensorValue = (value * 3.52f); 	// mV = (value x 3.6V / 1024) x 1000
						DBG("analog[%d] = %d\n", pin, sensorValue);
					}
				} else {
					CPin di(pin);
					di.input();
					sensorValue = di.read();
				}
				// send to BLE
				bleScience.waitTxEmpty();	// Separates BLE data package
				send_data(bleScience, GetSystemTickCount(), sensorValue);
				led2.invert();
    		}
    	} else {
    		led2 = LED_OFF;
    	}

    	//
    	// blink led
    	//
    	if ( tm.isExpired(500) ) {
    		tm.reset();
    		led1 = !led1;	// blink led
    	}

    	// Negotiate the "Connect Parameters Update"
    	conn.negotiate();
    }
}
