// sensor connection
#define RIGHT_IR_IN 2
#define LEFT_IR_IN 3

// L293 motor driver conection
#define in1 4 // left motor
#define in2 5

#define in3 6 // right motor
#define in4 7

#define ena 10 // left motor
#define enb 11 // right motor

// motor speeds
int right_motor_speed = 45; // sped of the motors when they are running on a straight line 
int left_motor_speed = 45; // left motor runs at a slightly higher speed compared to the right motor
int turn_speed = 20;
int my_delay = 50;

//=======================PD Control variables==============================
float kp = 20;  // proportional term
float kd = 100;  // derivative term

float old_error = 0;  
float new_error; 
float old_time = millis();
float new_time;
float dt; // change in error
float de; // change in time
float correction; // correction value
int right_val, left_val; // variables to hold ir sensor readings

float pid(float sensor, float setpoint){
/*
* calculate the correction to be applied to the motor movement
*/

  new_time = millis(); 
  dt = new_time - old_time;  // get the elapsed time

  new_error = setpoint - sensor;  
  de = new_error - old_error;  // change in error
  
  correction = (kp * new_error) + (kd * (de / dt));

  old_time = new_time;  // save time to use in the next loop
  old_error = new_error;  // use the current error as old error in the next iteration

  return correction;  // return the speed correction to be applied to the motor
}

void forward(int r_speed, int l_speed){

  // turn ON all motors  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  

  // left motor on 
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  analogWrite(ena, l_speed);
  analogWrite(enb, r_speed);

}

void left(int r_speed, int l_speed){
  // turn motors left - right motor on, left motor reversed direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);  

  analogWrite(ena, turn_speed);
  analogWrite(enb, r_speed);

}

void right(int r_speed, int l_speed){
  // turn motors right
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);  

  analogWrite(ena, l_speed);
  analogWrite(enb, turn_speed);

}

void stop(){
  // turn motors OFF
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);  

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RIGHT_IR_IN, INPUT);
  pinMode(LEFT_IR_IN, INPUT);

  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);

  // all motors initially off

  delay(2000);

}

void loop() {
  int right_val = digitalRead(RIGHT_IR_IN);
  int left_val = digitalRead(LEFT_IR_IN);

  //Serial.print("Left: ");Serial.print(right_val);Serial.print("\t\tRight: ");Serial.print(left_val); Serial.println();

  right_motor_speed = right_motor_speed - pid(right_val, 1); // apply correction to the right motor
  left_motor_speed = left_motor_speed + pid(left_val, 1);  // apply correction to the left motor

  right_motor_speed = constrain(right_motor_speed, 45, 200); 
  left_motor_speed = constrain(left_motor_speed, 45, 200);  

  Serial.print("LeftSpeed: ");Serial.print(left_motor_speed);Serial.print("\t\tRightSpeed: ");Serial.print(right_motor_speed); Serial.println();
   
  if((right_val == 1) && (left_val == 0)){
    // steer to the left
    left(right_motor_speed, left_motor_speed);
  }
  
  else if((right_val == 0) && (left_val == 1)){
    // steer to the right
    right(right_motor_speed, left_motor_speed);
  }
 
  else if((right_val == 0) && (left_val == 0)){
    forward(right_motor_speed, left_motor_speed);
  } 
 
  else if((right_val == 1) && (left_val == 1)){ 
    // handle crosspaths
    // both sensors are on a black line, continue moving forward
    forward(right_motor_speed, left_motor_speed);    
  }  
  else if((right_val == 1) && (left_val == 1)){
    stop();
  } 

  

  delay(my_delay);

}
