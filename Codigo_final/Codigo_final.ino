#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define I2C_ADDR 0x27 
#define BACKLIGHT_PIN 3 
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

const int pirPin = 2;
const int button = 5;
Servo miservo;

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  pinMode(button, INPUT_PULLUP);//Botón
  mlx.begin();//Sensor de temperatura
  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH); 
  lcd.home ();
  pinMode(pirPin, INPUT);//Sensor de movimiento
  Serial.begin(9600);
  miservo.attach(3);
  miservo.write(90);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("     Puerta");
  lcd.setCursor(0,1);
  lcd.print("   Automatica");
  boolean isMesured = false;
  if(isDetected()){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  Mida su temp: ");
    lcd.setCursor(0,1);
    lcd.print("   3 intentos ");  
    int intento = 0;
    int old = digitalRead(button);
    int newie;
    while(intento < 3){
      do{
        newie = digitalRead(button);
      } while(old == newie); 
      old = newie;
        lcd.clear();
        if(isHealthy()) {
          Serial.println("Acceso");
          lcd.setCursor(0,1);
          lcd.print("      PASE");
          openDoor();
          delay(1000);
          intento = 3;
        } else {
          lcd.setCursor(0,1);
          if(intento < 2) {
            lcd.print("Intente de nuevo");
          } else {
            lcd.print(" Acceso DENEGADO");
            delay(1000);
          }
          delay(1000);
        }
        intento++;
      
    }
    lcd.clear();
  }
}

boolean isDetected() {
  int val = 0;
  val = digitalRead(pirPin); 
  if(val == HIGH) {
    Serial.println("Motion detected!");
    return true;
  }
  //Serial.println("NO Motion");
  return false;
}

boolean isHealthy() {
  int i;
  float temp;
  lcd.setCursor(0,0);
  lcd.print("  Temp: ");
  temp = mlx.readObjectTempC();
  lcd.print(temp);
  lcd.print(" C");
  if(temp <= 37.5) { //
    return true;
  }
  return false;
}

void openDoor() {
  int pos = 0; 
  for(pos = 90; pos < 170; pos += 1) {
    miservo.write(pos);
    delay(15);
  }
  delay(5000);
  for(pos = 170; pos >= 90; pos -= 1) {
    miservo.write(pos);
    delay(15);
  } 
}
