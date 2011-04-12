/*
 * An interface to the TI TMP102 temperature sensor
 * 12 bit temperature reading, 0.5 deg. Celsius accuracy
 * -----------------------------------------------------------------
 *
 * Author  : Hedde Bosman (heddebosman@incas3.eu)
 * Author  : Ilya Dmitrichenko (errordeveloper@gmail.com)
 */

#include "contiki.h"
#include "lib/sensors.h"
#include "dev/adxl345-sensor.h"

#ifndef bool
#define bool uint8_t
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif


static void set_configuration(uint8_t rate, bool precision) {
	uint8_t tx_buf[2]; /* = {TMP102_REGISTER_CONFIGURATION, 
						0,
						(precision ? TMP102_CONF_EXTENDED_MODE : 0) | ((rate << 6) & TMP102_CONF_CONVERSION_RATE)
						};
	*/

	i2c_transmitinit(ADXL345_ADDR, 3, tx_buf);
}

/*---------------------------------------------------------------------------*/
static int value(int type) {
	uint8_t reg = X_AXIS;
	uint8_t temp[2];
	int16_t v = 0;
	
	/* transmit the register to start reading from */
	i2c_transmitinit(ADXL345_ADDR, 1, &reg);
	while (!i2c_transferred()); // wait for data to arrive

	/* receive the data */
	i2c_receiveinit(ADXL345_ADDR, 2, temp);
	while (!i2c_transferred()); // wait for data to arrive
	
	// 12 bit normal mode
	v = ((temp[0] <<8) | (temp[1])) >> 4; // lsb

	// 13 bit extended mode
	//temperature = ((temp[0] <<8) | (temp[1])) >> 3; // lsb

	return v;
}
/*---------------------------------------------------------------------------*/
static int status(int type) {
	if (type == SENSORS_ACTIVE || type == SENSORS_READY) {
		return 1; // fix?
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
static int configure(int type, int c) {
	if (type == SENSORS_ACTIVE) {
		if (c) {
			// set active
			set_configuration(1, false); // every 1 second, 12bit precision
		} else {
			// set inactive
		}
		return 1;
	}
	return 0;
}

/*---------------------------------------------------------------------------*/
SENSORS_SENSOR(adxl345_sensor, "Accelerometer", value, configure, status); // register the functions
