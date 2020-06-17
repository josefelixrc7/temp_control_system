//Sistema de control de temperatura

#include <OneWire.h>
#include <DallasTemperature.h>

// Pin donde conectaremos el sensor
  const int pinBoton1 = 5;
  const int pinBoton2 = 6;
  const int pinReleRes = 7;
  const int pinReleVal = 8;
  const int pinDatosSensor = 9;
  boolean cr = false;

// Instanciando
  OneWire objetoOneWire(pinDatosSensor);
  DallasTemperature sensorDS18B20(&objetoOneWire);

// Variables generales
  int buttonState1;
  int buttonState2;
  int cambioTemp;
  int TempSensor;

void setup() {
  // Comunicación serial
    Serial.begin(9600);
    sensorDS18B20.begin();
  // Pines
    pinMode(pinBoton1, INPUT);
    pinMode(pinBoton2, INPUT);
    pinMode(pinReleRes, OUTPUT);
    pinMode(pinReleVal, OUTPUT);
}

void loop() {
  // Proceso de evaluado
    procesoEvaluado();
    
  // Pantalla de diagnóstico
    procesoDiagnostico();
    
  // Proceso de corrección
    procesoCorreccion();
  
  delay(1000);
}

void procesoEvaluado(){
  // Temperatura
    Serial.println("Proceso de evaluado de temperatura");
    Serial.println("");
    sensorDS18B20.requestTemperatures();
}

void procesoDiagnostico(){
  // Temperatura
    Serial.print("Temperatura del sensor main: ");
    Serial.print(sensorDS18B20.getTempCByIndex(0));
    Serial.println("");
    TempSensor = sensorDS18B20.getTempCByIndex(0);
}

void procesoCorreccion(){
  buttonState1 = digitalRead(pinBoton1);
  buttonState2 = digitalRead(pinBoton2);
  
  // Manual
    if(buttonState1 == 0 && buttonState2 == 1){
      Serial.println("Estado 01");
      cambioTemp = 25;
      cr = true;
    }else if(buttonState1 == 1 && buttonState2 == 0){
      Serial.println("Estado 10");
      cambioTemp = 28;
      cr = true;
    }else if(buttonState1 == 1 && buttonState2 == 1){
      Serial.println("Estado 11");
      cambioTemp = 40;
      cr = true;
    }else if(buttonState1 == 0 && buttonState2 == 0){
      Serial.println("00: sólo diagnóstico.");
      cr = false;
    }
 
  // Automática
    if(cr == true){
        if(TempSensor < cambioTemp - 2){
          Serial.println("Encendemos la resistencia");
          digitalWrite(pinReleRes, HIGH); // Desactivamos el relé que activa la resistencia y calentamos por 5 segundos
          delay(5000);
          procesoEvaluado();
          procesoDiagnostico();
          digitalWrite(pinReleRes, LOW); //Desactivamos el relé que activa la resistencia y esperamos 30 segundos
          delay(5000);
          Serial.println("Apagamos la resistencia");
          Serial.println("");
        }else if(TempSensor > cambioTemp){
          Serial.println("La temperatura es mayor, esperamos a que enfríe");
        }
    }
}
