/* JoyStick module receiver code
 - CONNECTIONS: nRF24L01 Modules See:
   1 - GND
   2 - VCC 3.3V !!! NOT 5V
   3 - CE to Arduino pin 6
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
 */
//-----( Import needed libraries )-----
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
Servo myservo;
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN 8
#define CSN_PIN 7
//NOTE: the "LL" at the end of the constant is "LongLong" type
const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
/*
It's connected to the rest of the digital pins: D10, D11, D12 and D13. 
The two PWM pins connect to the enable functions for the two motors, 
and act as speed controls via the PWM duty cycle, D10 for Motor A, 
and D11 for Motor B. The corresponding GPIO - D12 for Motor A and D13 for motor B - 
are connected to each motors two input controls, once directly and once through 
an inverter, and thus act as direction controls: the motor spins one direction 
when it's high, the other when it's low, and stops - by coasting - 
when the speed is zero.
*/
int right_motor_PWM=10;          // PWM Motor A
int left_motor_PWM=11;          // PWM Motor B
int right_motor_direction=12;          // Motor A  
int left_motor_direction=13;          // Motor B
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int joystick[6];  // 6 element array holding Joystick readings
int speed = 0;
int speedb = 0;
int  xAxis, yAxis;
// the four button variables from joystick
int buttonUp;
int buttonRight;
int buttonDown;
int buttonLeft;
int servo_angle;

void setup()
{
  pinMode(right_motor_PWM, OUTPUT);
  pinMode(left_motor_PWM, OUTPUT);
  pinMode(right_motor_direction, OUTPUT);
  pinMode(left_motor_direction, OUTPUT);
  Serial.begin(9600);
  Serial.println("Nrf24L01 Receiver Starting");
  myservo.attach(9);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}

void loop()
{
  if ( radio.available() )
  {
      radio.read( joystick, sizeof(joystick) );
      xAxis = joystick[0];
      yAxis = joystick[1];
      
      // the four button variables from joystick array
      int buttonUp    = joystick[2];
      int buttonRight = joystick[3];
      int buttonDown  = joystick[4];
      int buttonLeft  = joystick[5];
      
// Y-axis used for forward and backward control  
 if (yAxis < 500 || buttonUp==0) {
    // Motors backward
    // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed 
    speed  = map(yAxis, 500, 0, 0, 255);
    Serial.print(" Speed reverse = ");  
    Serial.println(speed);
    if (buttonUp=0)
    {speedb=110;}
    else
    {speedb=0;}
    
    digitalWrite(right_motor_direction,LOW);
    analogWrite(right_motor_PWM, speed+speedb); 
    digitalWrite(left_motor_direction, LOW);
    analogWrite(left_motor_PWM, speed+speedb);

    //delay(30);
  }
else if (yAxis > 510 || buttonDown==0) {
    // Motors forward
    // Convert the increasing Y-axis readings for going forward from 550 to 1023 into 0 to 255 value for the PWM signal for increasing the motor speed
    speed  = map(yAxis, 510, 1023, 0, 255);
    
    Serial.print(" Speed forward = ");  
    Serial.println(speed);
    
    if (buttonDown=0)
    {speedb=110;}
    else
    {speedb=0;}
    digitalWrite(right_motor_direction,HIGH);
    analogWrite(right_motor_PWM, speed+speedb); 
    digitalWrite(left_motor_direction, HIGH);
    analogWrite(left_motor_PWM, speed+speedb);

    //delay(30);
  }
  // If joystick stays in middle the motors are not moving
  else {
    speed  = 0;
    Serial.print(" Speed zero = ");  
    Serial.println(speed);
    
    digitalWrite(right_motor_direction,LOW);
    analogWrite(right_motor_PWM, speed);
    digitalWrite(left_motor_direction, LOW);
    analogWrite(left_motor_PWM, speed);
    
    //delay(30);
  }

// X-axis used for left and right control
    // Convert the declining X-axis readings from 0 to 1023 into increasing 0 to 255 value
    servo_angle = xAxis;
    servo_angle = map(xAxis, 0, 1023, 105, 70);
   myservo.write(servo_angle); 
  } 
      Serial.print(" Servo = ");  
      Serial.print(servo_angle);
      Serial.print("X = ");
      Serial.print(xAxis);
      Serial.print(" Y = ");  
      Serial.print(yAxis);
      Serial.print(" Speed = ");  
      Serial.print(speed);
      Serial.print(" Up = ");
      Serial.print(joystick[2]);
      Serial.print(" Right = ");  
      Serial.print(joystick[3]);
      Serial.print(" Down = ");
      Serial.print(joystick[4]);
      Serial.print(" Left = ");  
      Serial.println(joystick[5]);
    
      } 
  
