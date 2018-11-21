#include <Wire.h>
#include <MPU6050.h>

#define ADXL_SCL 21
#define ADXL_SDA 20

#include <SoftwareSerial.h>
SoftwareSerial hackPitch(0, 1);
SoftwareSerial hackRoll(5, 6);
SoftwareSerial hackFall(8, 9);
MPU6050 mpu;

int fall;

void setup() 
{
  Serial.begin(115200);
  hackPitch.begin(115200);
  hackRoll.begin(115200);
  hackFall.begin(115200);
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
}

void loop()
{
  // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  int roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;

  // Output
  Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.print(roll);
  Serial.println();
  delay(10);
  hackPitch.write(pitch);
  hackRoll.write(roll);
  hackFall.write(fall);
}


