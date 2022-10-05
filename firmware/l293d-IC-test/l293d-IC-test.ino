#define IN1 7
#define IN2 8
#define ENA 10

const int speed = 40;

void forward(int s){
  // s = speed
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, s);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  forward(speed);

}

void loop() {
  // put your main code here, to run repeatedly:
  

}
