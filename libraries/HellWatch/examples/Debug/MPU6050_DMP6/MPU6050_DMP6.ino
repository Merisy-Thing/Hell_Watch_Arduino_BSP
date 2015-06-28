#include <HellWatch.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#define OUTPUT_READABLE_QUATERNION			0
#define OUTPUT_READABLE_EULER				1
#define OUTPUT_READABLE_YAWPITCHROLL		2
#define OUTPUT_READABLE_REALACCEL			3
#define OUTPUT_READABLE_WORLDACCEL			4

#define OUTPUT_SELECT						OUTPUT_READABLE_YAWPITCHROLL

HellWatch hell;
MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void setup() {
	Serial.begin(115200);
	Wire.begin();
    mpu.initialize();

	hell.clearDisplay();
	hell.setCursor(0, 0);
    // load and configure the DMP
    hell.println(F("Initializing DMP..."));
	hell.display();
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        hell.println(F("Enabling DMP..."));
		hell.display();
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        hell.println(F("Enabling interrupt"));
		hell.display();
        attachInterrupt(RTC_MPU_INT, dmpDataReady, FALLING);

        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        hell.println(F("DMP ready! Wait INT"));
		hell.display();
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
		hell.clearDisplay();
		hell.setCursor(0, 0);
        hell.print(F("DMP Init failed: "));
        hell.print(devStatus);
		hell.display();
    }
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
		hell.clearDisplay();
		hell.setCursor(0, 0);
        hell.println(F("FIFO overflow!"));
		hell.display();

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

		hell.clearDisplay();
		hell.setCursor(0, 0);

        #if OUTPUT_SELECT == OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            hell.println("quat:");
            hell.println(q.w);
            hell.println(q.x);
            hell.println(q.y);
            hell.print(q.z);
        #endif

        #if OUTPUT_SELECT == OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
            hell.println("euler:");
            hell.println(euler[0] * 180/M_PI);
            hell.println(euler[1] * 180/M_PI);
            hell.println(euler[2] * 180/M_PI);
        #endif

        #if OUTPUT_SELECT == OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            hell.println("ypr:");
            hell.println(ypr[0] * 180/M_PI);
            hell.println(ypr[1] * 180/M_PI);
            hell.println(ypr[2] * 180/M_PI);
        #endif

        #if OUTPUT_SELECT == OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            hell.println("areal:");
            hell.println(aaReal.x);
            hell.println(aaReal.y);
            hell.println(aaReal.z);
        #endif

        #if OUTPUT_SELECT == OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
            hell.println("aworld:");
            hell.println(aaWorld.x);
            hell.println(aaWorld.y);
            hell.println(aaWorld.z);
        #endif

		hell.display();
    }
}
