/*
    Kalman Filter Example for MPU6050. Output for processing.
    Read more: http://www.jarzebski.pl/arduino/rozwiazania-i-algorytmy/odczyty-pitch-roll-oraz-filtr-kalmana.html
    GIT: https://github.com/jarzebski/Arduino-KalmanFilter
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/


#include <Wire.h>
#include <MPU6050.h>
#include <KalmanFilter.h>

//define objects of IMU's
MPU6050 mpu;
MPU6050 mpu2;

//setting time for gyroscope calculation
unsigned long timer = 0;
float timeStep = 0.01;
// Pitch, Roll and Yaw values
float gyroPitch = 0;
float gyroRoll = 0;
float gyroYaw = 0;

float gyroPitch2 = 0;
float gyroRoll2 = 0;
float gyroYaw2 = 0;

float accPitch = 0;
float accRoll = 0;

float accPitch2 = 0;
float accRoll2 = 0;

float kalPitch = 0;
float kalRoll = 0;

float kalPitch2 = 0;
float kalRoll2 = 0;

KalmanFilter kalmanX(0.001, 0.003, 0.03);
KalmanFilter kalmanY(0.001, 0.003, 0.03);

KalmanFilter kalmanX2(0.001, 0.003, 0.03);
KalmanFilter kalmanY2(0.001, 0.003, 0.03);

typedef unsigned long long ULL;
ULL startTime = 0;
ULL passedTime = 0;
ULL t = 0;


bool activated = false;

void setup() 
{
  Serial.begin(115200);

  // Initialize MPU6050
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G,0x68))
  {
    delay(500);
  }

  while(!mpu2.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G,0x69))
  {
    delay(500);
  }
 
  // Calibrate gyroscope. The calibration must be at rest.
  // If you don't want calibrate, comment this line.
  mpu.calibrateGyro();
  mpu2.calibrateGyro();
}

void loop()
{

  if(!activated){
    if(digitalRead(11) == LOW){
      startTime = millis();
      activated = true;
    }
  }else{
  passedTime = millis();
  t = passedTime - startTime;

  Serial.print("\n");
  Serial.print((t/1000.0));
  Serial.print(",");
  
  

  timer = millis();

  Vector acc = mpu.readNormalizeAccel();
  Vector gyr = mpu.readNormalizeGyro();

  Vector acc2 = mpu2.readNormalizeAccel();
  Vector gyr2 = mpu2.readNormalizeGyro();

  gyroPitch = gyroPitch + gyr.YAxis * timeStep;
  gyroRoll = gyroRoll + gyr.XAxis * timeStep;
  gyroYaw = gyroYaw + gyr.ZAxis * timeStep;

  gyroPitch2 = gyroPitch2 + gyr2.YAxis * timeStep;
  gyroRoll2 = gyroRoll2 + gyr2.XAxis * timeStep;
  gyroYaw2 = gyroYaw2 + gyr2.ZAxis * timeStep;


  // Calculate Pitch & Roll from accelerometer (deg)
  accPitch = -(atan2(acc.XAxis, sqrt(acc.YAxis*acc.YAxis + acc.ZAxis*acc.ZAxis))*180.0)/M_PI;
  accRoll  = (atan2(acc.YAxis, acc.ZAxis)*180.0)/M_PI;

  accPitch2 = -(atan2(acc2.XAxis, sqrt(acc2.YAxis*acc2.YAxis + acc2.ZAxis*acc2.ZAxis))*180.0)/M_PI;
  accRoll2  = (atan2(acc2.YAxis, acc2.ZAxis)*180.0)/M_PI;

  // Kalman filter
  kalPitch = kalmanY.update(accPitch, gyr.YAxis);
  kalRoll = kalmanX.update(accRoll, gyr.XAxis);

  kalPitch2 = kalmanY2.update(accPitch2, gyr2.YAxis);
  kalRoll2 = kalmanX2.update(accRoll2, gyr2.XAxis);

  
  Serial.print(kalRoll);
  Serial.print(",");
  Serial.print(kalRoll2);
  Serial.print(",");
  //=============== imu1 ================
  Serial.print(acc.XAxis);
  Serial.print(",");
  Serial.print(acc.YAxis);
  Serial.print(",");
  Serial.print(acc.ZAxis);
  Serial.print(",");
  Serial.print(gyr.XAxis);
  Serial.print(",");
  Serial.print(gyr.YAxis);
  Serial.print(",");
  Serial.print(gyr.ZAxis);
  Serial.print(",");
  Serial.print(accPitch);
  Serial.print(",");
  Serial.print(accRoll);
  Serial.print(",");
  Serial.print(gyroPitch);
  Serial.print(",");
  Serial.print(gyroRoll);  
  Serial.print(",");
  Serial.println(gyroYaw);
  Serial.print(",");
  Serial.print(kalPitch);
  Serial.print(",");
  //=========== imu 2 ==============
  Serial.print(acc2.XAxis);
  Serial.print(",");
  Serial.print(acc2.YAxis);
  Serial.print(",");
  Serial.print(acc2.ZAxis);
  Serial.print(",");
  Serial.print(gyr2.XAxis);
  Serial.print(",");
  Serial.print(gyr2.YAxis);
  Serial.print(",");
  Serial.print(gyr2.ZAxis);
  Serial.print(",");
  Serial.print(accPitch2);
  Serial.print(",");
  Serial.print(accRoll2);
  Serial.print(",");
  Serial.print(gyroPitch2);
  Serial.print(",");
  Serial.print(gyroRoll2);  
  Serial.print(",");
  Serial.println(gyroYaw2);
  Serial.print(",");
  Serial.print(kalPitch2);

  }

  //Serial.println();
  //delay(500);
  //delay((timeStep*1000) - (millis() - timer));
}


