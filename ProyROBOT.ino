/*
 ARDUINO   L293D(Puente H)        
 5          10
 6          15
 9          7
 10         2
 5V         1, 9, 16
 GND        4, 5, 12, 13
 
 El motor 1 se conecta a los pines 3 y 6 del Puente H
 El motor 2 se conecta a los pines 11 y 14 del Puente H
 
 La fuente de alimentacion de los Motores se conecta a tierra y
 el positivo al pin 8 del puennte H. 
 
 Conexion del Modulo Bluetooth HC-05 y el Arduino
 ARDUINO    Bluetooth HC-05 
 0 (RX)       TX
 1 (TX)       RX
 5V           VCC
 GND          GND
 !!Cuidado!! Las conexiones de TX y RX al modulo Bluetooth deben estar desconectadas
 en el momento que se realiza la carga del codigo (Sketch) al Arduino.
 
 Conexion Sensor Ultrasonido HC-SR04
 ARDUINO    Ultrasonido HC-SR04 
 2            Echo
 3            Trig
 5V           VCC
 GND          Gnd
 */
int izqA = 5; 
int izqB = 6; 
int derA = 9; 
int derB = 10; 
int vel = 255;            // Velocidad de los motores (0-255)
int estado = 'g';         // inicia detenido

int pecho = 2;            // define el pin 2 como (pecho) para el Ultrasonido
int ptrig = 3;            // define el pin 3 como (ptrig) para el Ultrasonido
int duracion, distancia;  // para Calcular distacia

void setup()  { 
  Serial.begin(9600);    // inicia el puerto serial para comunicacion con el Bluetooth
  pinMode(derA, OUTPUT);
  pinMode(derB, OUTPUT);
  pinMode(izqA, OUTPUT);
  pinMode(izqB, OUTPUT);
  
  pinMode(pecho, INPUT);   // define el pin 2 como entrada (pecho) 
  pinMode(ptrig,OUTPUT);   // define el pin 3 como salida  (ptrig) 
  pinMode(13,OUTPUT);
} 

void loop()  { 

  if(Serial.available()>0){        // lee el bluetooth y almacena en estado
    estado = Serial.read();
  }
  if(estado=='e'){           // Boton desplazar al Frente
    analogWrite(derB, 0);     
    analogWrite(izqB, 0); 
    analogWrite(derA, vel);  
    analogWrite(izqA, vel);       
  }
  if(estado=='d'){          // Boton IZQ 
    analogWrite(derB, 0);     
    analogWrite(izqB, 0); 
    analogWrite(derA, 0);  
    analogWrite(izqA, vel);      
  }
  if(estado=='c'){         // Boton Parar
    analogWrite(derB, 0);     
    analogWrite(izqB, 0); 
    analogWrite(derA, 0);    
    analogWrite(izqA, 0); 
  }
  if(estado=='b'){          // Boton DER
    analogWrite(derB, 0);     
    analogWrite(izqB, 0);
    analogWrite(izqA, 0);
    analogWrite(derA, vel);  
  } 

  if(estado=='a'){          // Boton Reversa
    analogWrite(derA, 0);    
    analogWrite(izqA, 0);
    analogWrite(derB, vel);  
    analogWrite(izqB, vel);      
  }
  
  if (estado =='f'){               // Boton ON,  se mueve sensando distancia 
    digitalWrite(ptrig, HIGH);   // genera el pulso de trigger por 10us
    delay(0.01);
    digitalWrite(ptrig, LOW);
    duracion = pulseIn(pecho, HIGH);          // Lee el tiempo del Echo
    distancia = (duracion/2) / 29;            // calcula la distancia en centimetros
    delay(10); 
     
    if (distancia <= 15 && distancia >=2){    // si la distancia es menor de 15cm
     digitalWrite(13,HIGH);                 // Enciende LED
        
     analogWrite(derB, 0);             // Parar los motores por 600 mili segundos
     analogWrite(izqB, 0); 
     analogWrite(derA, 0);    
     analogWrite(izqA, 0); 
     delay (600);
        
     analogWrite(derA, vel);          // Reversa durante 500 mili segundos
     analogWrite(izqA, vel); 
     delay(500);           
        
     analogWrite(derB, 0);            // Girar durante 700 milisegundos   
     analogWrite(izqB, 0); 
     analogWrite(derA, 0);  
     analogWrite(izqA, vel);  
     delay(700);
     digitalWrite(13,LOW);
    } 
    else{                             // Si no hay obstaculos se desplaza al frente  
        analogWrite(derB, vel);     
        analogWrite(izqB, vel); 
        analogWrite(derA, 0);  
        analogWrite(izqA, 0); 
     }
  }
  if(estado=='g'){                   // Boton OFF, detiene los motores, deja estatico
    digitalWrite(derB,0);
    digitalWrite(izqB,0);
    digitalWrite(derA,0);
    digitalWrite(izqA,0);
  }
} 
