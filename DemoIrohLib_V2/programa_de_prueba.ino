#include<KnightRoboticsLibs_Iroh.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>
#include<NewPing.h>
#include<Wire.h>

int sensorIzquierda = 0;  // Tacto izquierda.
int sensorDerecha = 0;    // Tacto derecha.
int sensorDistancia = 0;  // Distancia.
int LI = 0; // Luz izquierda.
int LC = 0; // Luz central.
int LD = 0; // Luz derecha.

int a = 0;
int b = 1;
int c = 0;
int umbral = 200;

void setup() {
  Serial.begin(9600);
  inicializarPantallaRobot();
  inicializarSensoresRobot();
  inicializarMovimientoRobot();
  inicializarCabezaRobot();
  inicializarGolpe();
  botonInicio();
  borrarPantalla();
  moverServoPitch(20);
}

void loop() {
  LC = leerSensorLineaCentral();
  sensorDerecha = leerSensorObstaculoDerecho();
  sensorIzquierda = leerSensorObstaculoIzquierdo();
  sensorDistancia = leerDistanciaSonar();

  //Prueba Motores
  escribirPantalla(0, 0, "Prueba motores");
  escribirPantalla(0, 1, "IR Der para sgte");
  while (a == 0) {
    sensorDerecha = leerSensorObstaculoDerecho();
    sensorIzquierda = leerSensorObstaculoIzquierdo();
    if (!sensorDerecha) {
      a = 1;
      delay(200);
    }

    if (!sensorIzquierda) {
      retroceder(200);
    } else {
      avanzar(200);
    }

  }

  detenerse();

  escribirPantalla(0, 0, "Prueba sonar    ");
  //Ultrasonido
  while (a == 1) {
    sensorDistancia = leerDistanciaSonar();
    sensorDerecha = leerSensorObstaculoDerecho();
    sensorIzquierda = leerSensorObstaculoIzquierdo();
    if (!sensorDerecha) {
      a = 2;
      borrarPantalla();
      escribirPantalla(0, 0, "Siguiente");
      delay(200);
    }
    if (!sensorIzquierda) {
      borrarPantalla();
      escribirPantalla(0, 0, "b = ");
      escribirPantalla(4, 0, b);
      if (b == 1)
        b = -1;
      else if (b == -1)
        b = 1;
    }

    if ( b == 1) {
      avanzar(150);
    } else {
      detenerse();
    }

    escribirPantalla(0, 1, sensorDistancia);
    delay(200);
    borrarPantalla();
  }

  //Seguidor de Linea
  //escribirPantalla(0, 0, "Seguidor linea  ");
  borrarPantalla();
  while (a == 2) {
    sensorDerecha = leerSensorObstaculoDerecho();
    LC = leerSensorLineaCentral();
    LI = leerSensorLineaIzquierdo();
    LD = leerSensorLineaDerecho();

    /*borrarPantalla();
    escribirPantalla(0, 1, LI);
    escribirPantalla(7, 1, LC);
    escribirPantalla(11, 1, LD);
    */

    if (!sensorDerecha) {
      a = 3;
      delay(200);
    }

    delay(100);
    if (LI > umbral && LC > umbral && LD > umbral) { //NNN, intersection
      escribirPantalla(0, 1, "               ");
      detenerse();
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "NNN");
    } else if (LI > umbral && LC > umbral && LD < umbral) { //NNB
      avanzar(70, 180);
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "NNB");
    } else if (LI > umbral && LC < umbral && LD > umbral) { //NBN, not possible
      escribirPantalla(0, 1, "               ");
      detenerse();
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "NBN");
    } else if (LI > umbral && LC < umbral && LD < umbral) { //NBB
      avanzar(90, 210);
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "NBB");
    } else if (LI < umbral && LC > umbral && LD > umbral) { //BNN
      avanzar(180, 90);
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "BNN");
    } else if (LI < umbral && LC > umbral && LD < umbral) { //BNB
      avanzar(150);
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "BNB");
    } else if (LI < umbral && LC < umbral && LD > umbral) { //BBN
      avanzar(210, 90);
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "BBN");
    } else if (LI < umbral && LC < umbral && LD < umbral) { //BBB, lost
      escribirPantalla(0, 1, "               ");
      detenerse();
      borrarPantalla();
      escribirPantalla(0, 0, LI);
      escribirPantalla(6, 0, LC);
      escribirPantalla(12, 0, LD);
      escribirPantalla(5, 1, "BBB");
    }
  }

  //Servomotores
  escribirPantalla(0, 0, "Prueba servos  ");
  escribirPantalla(0, 1, "Alternar con IRs");
  detenerse();
  while (a == 3) {
    sensorDerecha = leerSensorObstaculoDerecho();
    sensorIzquierda = leerSensorObstaculoIzquierdo();

    if (!sensorDerecha)
      c += 1;
    else if (!sensorIzquierda)
      c -= 1;
    escribirPantalla(15, 0, c);

    switch (c) {
      case 1:
        moverServoGolpe(0);
        delay(500);
        moverServoPitch(45);
        delay(500);
        moverServoYaw(90);
        delay(500);
        moverServoGolpe(1);
        delay(500);
        moverServoGolpe(0);
        delay(500);
        moverServoGolpe(-1);
        delay(500);
        break;
      case 2:
        moverServoGolpe(0);
        delay(500);
        moverServoPitch(45);
        delay(500);
        moverServoYaw(90);
        delay(500);
        moverServoPitch(80);
        delay(1000);
        moverServoPitch(45);
        delay(1000);
        moverServoPitch(10);
        delay(1000);
        break;
      case 3:
        moverServoGolpe(0);
        delay(500);
        moverServoPitch(45);
        delay(500);
        moverServoYaw(90);
        delay(500);
        moverServoYaw(10);
        delay(1000);
        moverServoYaw(90);
        delay(1000);
        moverServoYaw(170);
        delay(1000);
        break;
      case 4:
        detenerse();
        a = 4;
        delay(200);
        break;
      default:
        detenerse();
        break;
    }
  }

  //Stand by
  borrarPantalla();
  escribirPantalla(0, 0, "Stand By        ");
  escribirPantalla(1, 0, "IR Der reinicia");
  while (a == 4) {
    sensorDerecha = leerSensorObstaculoDerecho();
    if (!sensorDerecha) {
      a = 0;
      borrarPantalla();
      delay(200);
    }
  }
}

