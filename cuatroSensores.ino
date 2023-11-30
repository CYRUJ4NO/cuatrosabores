#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

///los pines para el trabajo en efecto lm35:A1  ,hrsr04 t=40,e=41 ,boton d22 ,led d30 ,ldr A0 

const int Trigger = 40;   //Pin digital 2 para el Trigger del sensor
const int Echo = 41;   //Pin digital 3 para el Echo del sensor
int ldr;
int boton=22;
int led = 30;
int estadoled;
int tempC;


void setup() {
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.begin(9600);//iniciailzamos la comunicación
  pinMode(boton,INPUT);
  pinMode(led,OUTPUT);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
}

void loop()
{
// Con analogRead leemos el sensor, recuerda que es un valor de 0 a 1023
  tempC = analogRead(A1); 
   
  // Calculamos la temperatura con la fórmula
  tempC = (5.0 * tempC * 100.0)/1024.0;
  
//se declara la info para hcsr04
  long t; //timepo que demora en llegar el eco
  long d; //distancia en centimetros
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger, LOW);
  t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
  d = t/59;             //escalamos el tiempo a una distancia en cm

  //Info para ldr
  ldr = analogRead(A0);

  
  //info para el boton
  if(digitalRead(boton)){
    digitalWrite(led,1);
    estadoled=1;
    }else{digitalWrite(led,0);estadoled=0;}
    
  //info monitor serial
  if (estadoled){
    Serial.println("El boton esta : Encendido");
    }else{Serial.println("El boton esta : Apagado"); }

  //Imprime la temperatura
  Serial.print("La temperatura es : ");
  Serial.print(tempC);
  Serial.println("C°");
  
  //Se imprime la info en el monitor serial
  Serial.print("La luminisencia es:");
  Serial.println(ldr);
  Serial.print("Distancia: ");
  Serial.print(d);      //Enviamos serialmente el valor de la distancia
  Serial.print("cm");
  Serial.println();
  
  //imprime en oled
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(tempC);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C");
  
  delay(1000);          //Hacemos una pausa de 100ms
}
