#include "Arduino.h"
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 2000
// IP address of the laptop
#define HOST "192.168.110.144"

#define SSID "Galaxy S21 Ultra 5G6dff"
#define PASS "mdwd3094"


int client = -1;
int errors = 0;

void stream(void* pvParameters){

    while(true){

        recv(client, (void*) (&errors), 4, MSG_WAITALL);
    }

}





//The speed control pins viz. ENA and ENB are used to turn the motors ON, OFF and control its speed.
//Pulling these pins HIGH will make the motors spin, pulling it LOW will make them stop. But, with Pulse Width Modulation (PWM), we can actually control the speed of the motors.
//That is why the enA and enB are connected to PWM pins to regulated the speed at which the motors rotate

// motor one
int enA = 14;        //l298n's en_A port connected to the pwm pin (5) of the arduino uno
int in1 = 27;        //direction control pin in1 connected to digital pin (6) of the arduino uno
int in2 = 26;        //direction control pin in2 connected to digital pin (7) of the arduino uno

// motor two
int in3 = 2;        //similarly for motor two
int in4 = 4;
int enB = 5;       //port 10 is also a pwm port so that we can control the speed of the motors via enA and enB pins

const uint8_t max_a = 200 ;          //maximum speed of the motor a
const uint8_t max_b = 200 ;          //maximum speed of the motor b
const uint8_t base_a = 200 ;          //base speed of a
const uint8_t base_b = 200 ;          //base speed of b

float elapsedTime, curr_time, Prev_time;      //time variables

//PID tuning

float pid, err, previous_error = 0;
float pid_p = 0;            //proportional part of the pid
float pid_i = 0;            //integral part of the pid
float pid_d = 0;            //differential part of the pid

double kp = 20;              //proportional factor
double ki = 0.01;              //integrating factor
double kd = 6;            //differentiating factor

int speed_A;
int speed_B;

// this function takes an error variable as parameter and performs corrections
void PID(int error)                       
{
  Prev_time = curr_time;              //millis() returns the number of milliseconds passed since the Arduino board began running the current program
  curr_time = millis();              //some time has passed after the above operation , so now the millis() returns some different value
  elapsedTime = (curr_time - Prev_time) / 1000;       //we calculate the time elapsed by subtracting both the values and diving them by 1000


//this factor requires an error to be present to produce a correction, if there is 0 error 
//then no correction will be produced
//this produces an steady state error

  pid_p = kp * error;                                   

//the integrating part is used to reduce the steady state error by summing all the previous errors
  pid_i = pid_i + (error * elapsedTime);                
                                                        

//the derivative part is used to control the system based on the slope of the error signal
//calculating the derivative of the error and using the kd to tell how much we want the 
//derivative part of the system to affect the total correction
  pid_d = kd * ((error - previous_error) / elapsedTime);    

  pid = pid_p + ki * pid_i + pid_d;                         //total correction
//Serial.println(pid);    



  speed_A = base_a + pid;     
  //adjusting the speed of motor A with the help of PID corrections                  
  speed_B = base_b - pid;
  //adjusting the speed of motor B with the help of PID corrections

//setting a limit for the maximum speed a motor can turn on
  if (speed_A > max_a) {
    speed_A = max_a;
  }
  if (speed_B > max_b) {
    speed_B = max_b;
  }

  if (speed_A < 0) {
    speed_A = 0;
  }
  if (speed_B < 0) {
    speed_B = 0;
  }

  previous_error = error;                       //replacing the previous error with the new error
}

void setup() {

    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

    

    delay(2000);

    Serial.begin(115200);


    // ------------------------------------------ Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(SSID, PASS);

    while(WiFi.status() != WL_CONNECTED){
        Serial.println(".");
        delay(1000);
    }

    Serial.println("\nNetwork connected.");

    Serial.println(WiFi.localIP());

    // ------------------------------------------ socket()
    while(client == -1){
        client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        delay(1000);
    }

    Serial.println("Socket creation was successful.");

    // ------------------------------------------ connect()
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(HOST);

    int stat = -1;
    while(stat != 0){
        stat = connect(client, (struct sockaddr*) &server_addr, sizeof(server_addr));
        delay(1000);
    }

    Serial.println("Connection was successful.");

    delay(1000);

    xTaskCreate(stream, "stream", 10000, NULL, 1, NULL);
    

  // All the arduino pins used are outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(13, OUTPUT);

  curr_time = millis();                            //begin serial communication with arduino at the baud rate of 115200 bits/second

  Serial.setTimeout(1);
}


void moveForward(int speed1, int speed2) {
  
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enA, speed1); // Speed Range 0-255

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  analogWrite(enB, speed2);
}



void loop() {

  //recieves the error variable value from the OpenCV code based on the camera's current position
  //and stores in the errors variable
             
  
  //this error is now given to the PID function as an input to perform corrections on
  PID(errors);
  Serial.println(errors);   

  //the PID function determines the speed_A, and speed_B of the motor based on those corrections                                       
  moveForward(speed_A, speed_B);        //moveForward function is used to give the motors their speeds after this whole process
}

//this process repeats with feedback in the loop again and again until the arduino is disconnected 