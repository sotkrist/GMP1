#include <digitalWriteFast.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define vibration_sensor A0
#define IR_Sensor_1 2
#define IR_Sensor_2 3
#define free_fall_led 4

int activate = 1;
int vibration_state = 0;

double startMillis;
double endMillis;
double v = 0.00;              //velocity (m/s)
double t = 0.00;             //free fall time (s)
double h = 0.71;            //free fall ditance (m)
double F = 0.00;           //force (N)
double g = 0.00;          //accelaration due to gravity (m/s2)
double m = 0.01213;      //object's mass (kg)
double Length = 0.035;  //object's lenght (m)

void setup() {
  Serial.begin(9600);
  pinMode(IR_Sensor_1, INPUT);
  pinMode(IR_Sensor_2, INPUT);
  pinMode(vibration_sensor, INPUT_PULLUP);
  pinMode(free_fall_led, OUTPUT);

  h = h - Length;

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("t:");
  lcd.setCursor(7, 0);
  lcd.print("v:");
  lcd.setCursor(0, 1);
  lcd.print("g:");
  lcd.setCursor(7, 1);
  lcd.print("F:");
}


void loop() {
  vibration_fix();

  if (digitalReadFast(IR_Sensor_1) == LOW || digitalReadFast(IR_Sensor_2) == LOW)
  {
    startMillis = micros();
    activate = 0;
    digitalWriteFast(free_fall_led, HIGH);
  }

  if (digitalReadFast(vibration_sensor) == vibration_state && activate == 0)
  {
    endMillis = micros();
    digitalWriteFast(free_fall_led, LOW);
    calculate_units();
    print_lcd_datas();
    print_serial_datas();
    activate = 1;
  }
}

void calculate_units()
{
  t = (endMillis - startMillis) / 1000000;
  g = 2 * h / pow(t, 2);
  v = g * t;
  F = m * g;
}

void vibration_fix()
{
  int x = digitalReadFast(vibration_sensor);

  if (x == 1)
  {
    vibration_state = 0;
  }
  else
  {
    vibration_state = 1;
  }
}

void print_serial_datas()
{
  Serial.println();
  Serial.println("<------------------------------------------------>");

  Serial.print("Acceleration due to gravity: ");
  Serial.print(g, 2);
  Serial.println("m/s2");

  Serial.print("Time: ");
  Serial.print(t, 3);
  Serial.println("s");

  Serial.print("Free fall started: ");
  Serial.print(startMillis);
  Serial.println("µs");

  Serial.print("Free fall ended: ");
  Serial.print(endMillis);
  Serial.println("µs");

  Serial.print("Distance: ");
  Serial.print(h, 3);
  Serial.println("m");

  Serial.print("Velocity: ");
  Serial.print(v, 3);
  Serial.println("m/s");

  Serial.print("Force: ");
  Serial.print(F, 3);
  Serial.println("N");

  Serial.print("Object's mass: ");
  Serial.print(m, 3);
  Serial.print("kg ~ ");
  Serial.print(m * 1000, 3);
  Serial.println("g");

  Serial.print("Object's length: ");
  Serial.print(Length, 2);
  Serial.print("m ~ ");
  Serial.print(Length * 100, 2);
  Serial.println("cm");

  Serial.println("<------------------------------------------------>");
  Serial.println();
}

void print_lcd_datas()
{
  lcd.setCursor(2, 0);
  lcd.print(t);
  lcd.setCursor(9, 0);
  lcd.print(v);
  lcd.setCursor(2, 1);
  lcd.print(g);
  lcd.setCursor(9, 1);
  lcd.print(F);
}
