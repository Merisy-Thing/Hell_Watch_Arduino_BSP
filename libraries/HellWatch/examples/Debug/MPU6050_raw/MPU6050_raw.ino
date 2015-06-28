#include <HellWatch.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

HellWatch hell;
MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
	Serial.begin(115200);
	Wire.begin();
	mpu.initialize();

	hell.println("Hell");
	hell.display();
}

void loop() {
    // read raw accel/gyro measurements from device
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

	// display  accel/gyro x/y/z values
	hell.clearDisplay();
	hell.setCursor(0, 0);
	hell.println("a/g:\t");
	hell.println(ax);
	hell.println(ay);;
	hell.println(az);
	hell.println(gx);
	hell.println(gy);
	hell.print(gz);
	hell.display();

	delay(500);
}
