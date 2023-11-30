//ESTE TRABAJO SE REALIZO CON ARDUINO MEGA 2560
//PARA ADAPTARLO A TU NECESIDAD DEBERAS CAMBIAR LOS PINES DE ALGUNOS SENSORES

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

///los pines para el trabajo en efecto lm35:A1  ,hrsr04 t=40,e=41 ,boton d22 ,led d30 ,ldr A0 

const int Trigger = 40;   //Pin digital 2 para el Trigger del sensor
const int Echo = 41;   //Pin digital 3 para el Echo del sensor
int ldr; //Se conecta a la analogina A0
int boton=22;
int led = 30;
int estadoled;
int tempC;  //Se conecta a la analogina A1
//APLICAMOS EL RETARDO PARA CADA MUESTREO 
int retardo=500;

void setup() {
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  Serial.begin(115200);//iniciailzamos la comunicación
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
  
  // imprime en oled
  display.clearDisplay();//Limpia la pantalla
  
  // temperatura
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Temperatura: ");
  display.setCursor(78,00);
  display.print(tempC);
  display.print(" ");
  display.cp437(true);
  display.write(167);
  display.print("C");

  // distancia
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.print("Distancia: ");
  display.setCursor(70, 10);
  display.print(d);
  display.print("cm"); 

  // Estado boton
   if (estadoled){
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Estado boton: ");
    display.setCursor(80, 20);
    display.print("ON");
   }else{
    display.setTextSize(1);
    display.setCursor(0, 20);
    display.print("Estado boton: ");
    display.setCursor(80, 20);
    display.print("OFF"); 
    }

  //LDR para display de 128x32 esta info no se ve
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.print("Cantidad luz: ");
    display.setCursor(80, 30);
    display.print(ldr);


  //Envia info al display
  display.display();
  delay(retardo);          //Hacemos una pausa
}
