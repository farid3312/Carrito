class Motor {
private:
  int pinA;
  int pinB;

public:
  Motor(int pinA, int pinB) : pinA(pinA), pinB(pinB) {
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
  }

  void avanzar(int vel) {
    digitalWrite(pinB, 0);
    analogWrite(pinA, vel);
  }

  void retroceder(int vel) {
    analogWrite(pinA, 0);
    analogWrite(pinB, vel);
  }

  void detener() {
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
  }
};

class CarroControlRemoto {
private:
  Motor motorDerecho;
  Motor motorIzquierdo;
  int ptrig;
  int pecho;
public:
  CarroControlRemoto(int m1a, int m1b, int m2a, int m2b, int ptrig, int pecho)
      : motorDerecho(m1a, m1b), motorIzquierdo(m2a, m2b), ptrig(ptrig), pecho(pecho) {
    pinMode(ptrig, OUTPUT);
    pinMode(pecho, INPUT);
    pinMode(13, OUTPUT);
  }

  void moverAdelante(int vel) {
    motorDerecho.avanzar(vel);
    motorIzquierdo.avanzar(vel);
  }
  void girarIzquierda(int vel) {
    motorDerecho.detener();
    motorIzquierdo.avanzar(vel);
  }
  void girarDerecha(int vel) {
    motorDerecho.avanzar(vel);
    motorIzquierdo.detener();
  }

  void detenerMotores() {
    motorDerecho.detener();
    motorIzquierdo.detener();
  }

  void deteccionObstaculos(int vel) {
    digitalWrite(ptrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ptrig, LOW);

    int duracion = pulseIn(pecho, HIGH);
    int distancia = (duracion / 2) / 29;

    delay(10);

    if (distancia <= 15 && distancia >= 2) {
      digitalWrite(13, HIGH);
      detenerMotores();
      delay(200);
      motorDerecho.retroceder(vel);
      motorIzquierdo.retroceder(vel);
      delay(500);
      detenerMotores();
      delay(600);
      motorDerecho.detener();
      motorIzquierdo.detener();
      motorIzquierdo.avanzar(vel);
      delay(1100);
      digitalWrite(13, LOW);
    } else {
      detenerMotores();
      delay(200);
      moverAdelante(vel);
    }
  }
};

// Instancia del objeto CarroControlRemoto
CarroControlRemoto miCarro(9, 10, 5, 6, 3, 2);

// Configuración inicial
void setup() {
  Serial.begin(9600);
}

// Bucle principal
void loop() {
  while (Serial.available() > 0) {
    char val = Serial.read();
    Serial.println(val);

    switch (val) {
      case 'a':
        miCarro.moverAdelante(255); // Velocidad máxima
        break;
      case 'b':
        miCarro.girarIzquierda(255);
        break;
      case 'c':
        miCarro.detenerMotores();
        break;
      case 'd':
        miCarro.girarDerecha(255);
        break;
      case 'e':
        miCarro.deteccionObstaculos(255);
        break;
      case 'f':
        miCarro.deteccionObstaculos(255);
        break;
      case 'g':
        miCarro.detenerMotores();
        break;
      default:
        // Manejar comandos desconocidos si es necesario
        break;
    }
  }
}
