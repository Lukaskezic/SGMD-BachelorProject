#include "HandDetector/handDetector.h"
#include "Measurement/Measurement.h"
#include "Motor/Motor.h"
#include "Motor/PID.h"
#include "Somo/somo.h"
#include "TableDetector/QrCode.hpp"
#include "BossBoundary/bossBoundary.h"

using namespace std;

enum programState // Definition af states i state machine
{
  startMotor,
  stopMotor,
  detectQRCode,
  detectHand,
  receiveOrder,
  deliverOrder
};

void programStateSwitch(programState &state);

pthread_t *PIDThread; // PID traad 
pthread_t *PIDMThread; // Measurement traad
pthread_t *QrThread; // QR traad

somo *       s1    = new somo(); // Somo objekt
Measurement *mInfo = new Measurement(); //Measurement objekt
PID *        p1    = new PID(mInfo); // PID objekt der faar Measurement objekt ind
PIDInfo *    pInfo = new PIDInfo(); // PID objekt der indeholder PID data
Motor *      m1    = new Motor(p1); // Motor objekt der faar PID objekt ind
QrCode        q1        = QrCode(); // QR scanner objekt
handDetector *h1 = new handDetector(); // Hand Detector objekt

string        tableNo   = "1"; // init af bord
int           direction = forward; // init af retning
BossBoundary *boss      = new BossBoundary(); //Boss boundary objekt

int main()
{
  programState state = receiveOrder; // start state

  // Initialization and prep for loop
  s1->initSomo();

  // Thread for PID controller calculation
  pthread_create(
      PIDThread, NULL,
      PIDController_Update_thread, // Function der skal kører og udregne når et
                                   // flag bliver sat
      (void *)p1->getInfo()); // Parameter til funktionen (void*) som indeholder
                              // al data der skal gå mellem funktion og main

  // Thread for measurements for PID controller
  pthread_create(PIDMThread, NULL, PIDMeasurement, (void *)mInfo->getInfo());

  // Thread for QR detector
  pthread_create(QrThread, NULL, QrCode_thread, (void*)boss.tableNo);
  for (;;)
  {
    programStateSwitch(state); // funktion der koerer states i for-loop
  }
}
// Funktion der indeholder state machine
void programStateSwitch(programState &state)
{
  switch (state)
  {
  case receiveOrder:
  {
    tableNo   = boss->receive(); // Modtager bord nr, der skal leveres til
    direction = forward;
    state     = startMotor;
  }
  break;

  case startMotor:
  {
    m1->startMotor(direction);
    pInfo->calcFlag = 1;// Saetter flag til PID og measurement maaling og udregning til start
    state           = detectQRCode;
  }
  break;

  case stopMotor:
  {
    m1->stopMotor(direction); // Starter motor i rigtig retning
    pInfo->calcFlag = 0; // Saetter flag til PID og measurement maaling og udregning til stop
    if (direction = forward)
      state = deliverOrder;
    else
      state = receiveOrder;
  }
  break;

  case detectQRCode:
  {
    QrCode_thread(); //Laeser QR kode
    state = stopMotor;
  }
  break;

  case detectHand:
  {
    h1->detectHand(); // Detekterer haand
    direction = backward;
    tableNo   = "0"; // Saetter bord til start position
    state     = startMotor;
  }
  break;

  case deliverOrder:
  {
    s1->playSound(); // Afspiller lyd
    state = detectHand;
  }
  break;

  default:
  {
    // nothing
  }
  break;
  }
}