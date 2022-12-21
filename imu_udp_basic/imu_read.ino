// define must ahead #include <M5Stack.h>
#define M5STACK_MPU6886 
// #define M5STACK_MPU9250 
// #define M5STACK_MPU6050
// #define M5STACK_200Q

#include <M5Stack.h>
//
#include <utility/MahonyAHRS.h>
#include <MadgwickAHRS.h>

Madgwick filter;
unsigned long microsPerReading, microsPrevious;

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0.0F;

float stockedGyroZs[10];
int stockCnt=0;
float adjustGyroZ=0;
int stockedGyroZLength=0;

String imu_data;

void imu_setup(){

  stockedGyroZLength=sizeof(stockedGyroZs)/sizeof(int);

  // Initialize the M5Stack object
  M5.begin();
  M5.Power.begin();

  M5.IMU.Init();
  filter.begin(10);  // 10Hz  initialize the filter

  microsPerReading = 1000000 / 10;
  microsPrevious = micros();

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);
}

String imu_loop() {
  float pitch,roll,yaw,Duty,P,D,now,dt,Time;
  //
  // put your main code here, to run repeatedly:
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ);
  M5.IMU.getAccelData(&accX,&accY,&accZ);
  //
  // M5.IMU.getAhrsData(&pitch,&roll,&yaw); 

  //for calibration purpose store data for "stockedGyroZLength"
  if(stockCnt<stockedGyroZLength){
    stockedGyroZs[stockCnt]=gyroZ;
    stockCnt++;
  }else{
    if(adjustGyroZ==0){
      for(int i=0;i<stockedGyroZLength;i++){
        adjustGyroZ+=stockedGyroZs[i]/stockedGyroZLength;
      }
    }
    //adjust gyroZ
    gyroZ-=adjustGyroZ*0.994;    // 0.993 increase, 0.995 decrease
    //calc pitch/roll/yaw
    MahonyAHRSupdateIMU(gyroX * DEG_TO_RAD, gyroY * DEG_TO_RAD, gyroZ * DEG_TO_RAD, accX, accY, accZ, &pitch, &roll, &yaw);
  }

  //
  M5.IMU.getTempData(&temp);

  M5.Lcd.setCursor(0, 20);
  M5.Lcd.printf("%6.2f  %6.2f  %6.2f      ", gyroX, gyroY, gyroZ);
  M5.Lcd.setCursor(220, 42);
  M5.Lcd.print(" o/s");
  M5.Lcd.setCursor(0, 65);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", accX, accY, accZ);
  M5.Lcd.setCursor(220, 87);
  M5.Lcd.print(" G");
  M5.Lcd.setCursor(0, 110);
  M5.Lcd.printf(" %5.2f   %5.2f   %5.2f   ", pitch, roll, yaw);
  M5.Lcd.setCursor(220, 132);
  M5.Lcd.print(" degree");
  M5.Lcd.setCursor(0, 155);
  M5.Lcd.printf("Temperature : %.2f C", temp);

  imu_data = String(pitch) + "," + String(roll) + "," + String(yaw);
  return imu_data;
}
