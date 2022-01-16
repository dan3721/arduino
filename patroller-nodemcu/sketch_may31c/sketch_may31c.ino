
#include <ESP8266WiFi.h>

const char* ssid = "Hoth";
const char* password = "";

WiFiServer server(80);

//const int STOP = 2;

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    //    IPAddress ip = client.remoteIP();
    //    Serial.printf("-%s connected", ip.toString().c_str());
    while (client.connected())
    {
      while (client.available())
      {
        char c = client.read();
        //        Serial.write(c);
        //                Serial.printf("c:[%s]", c); // throwing Exception?

        if (c == '\r') // eat carriage returns
        {
          continue;
        }

        if (c == '\n') // newline indicates end of cmd set
        {
          client.println("ACK");
          break;
        }

        int i = client.parseInt();

        if (c == 'x') // abort
        {
          //          digitalWrite(STOP, HIGH);
          break;
        }

        // send the cmd to the controller
        //        Serial.print(c);
        //        Serial.print(i);

        execute(c, i);
      }
    }
    delay(10); // give the web browser time to receive the data
    client.stop();
    //    Serial.printf("-%s disconnected", ip.toString().c_str());
  }
}



/**
   Remember to set the current limit on the A4988 stepper!
   Current rating: 500 mA per coil
   VREF=8*0.500*0.068=0.272V

   TODO:
   - add periodic beckon sound
   - add dump cmd
   - add dedicated 3v regulator
   - hook up dump servo (6-7.4v)
   - add resistor to speaker
   - flush linking messages
   - auto power off/sleep
   - misc/out?
   - self distruct on abort?
*/

//const int ABORT = 5;        // d1
//const int CANNON = 4;       // d2
//const int SPEAKER = 0;      // d3*
////const int STEP_ENABLE = 2;  // d4
//const int A_DIR = 14;       // d5
//const int A_STEP = 12;      // d6
//const int B_DIR = 13;       // d7
//const int B_STEP = 15;      // d8

const int ABORT = 0;        // d0 16
const int SPEAKER = 5;      // d1 5
const int DUMP = 4;         // d2 4
const int STEP_ENABLE = 0;  // d3 0   (active low)
const int CANNON = 2;       // d4 2   (active low)
const int A_DIR = 14;       // d5 14
const int A_STEP = 12;      // d6 12
const int B_DIR = 13;       // d7 13
const int B_STEP = 15;      // d8 15  (active high)

const int STEP_DELAY = 2; // millis (tested down to 500 μs via delayMicroseconds)

void setup() {

//  attachInterrupt(digitalPinToInterrupt(ABORT), xxx, RISING);

  // motor setup
  pinMode(STEP_ENABLE, OUTPUT);
  pinMode(A_STEP, OUTPUT);
  pinMode(B_STEP, OUTPUT);
  enableMotors(false);
  pinMode(A_DIR, OUTPUT); digitalWrite(A_DIR, LOW);
  pinMode(B_DIR, OUTPUT); digitalWrite(B_DIR, LOW);

  pinMode(CANNON, OUTPUT);

  pinMode(SPEAKER, OUTPUT);

  delay(1000);
  Serial.begin(115200);
  while (!Serial) {}

  WiFi.begin(ssid, password);

  Serial.println();
  Serial.print("Linking ");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  server.begin();
  Serial.printf("\nLinked and awaiting orders at %s\n", WiFi.localIP().toString().c_str());
  Serial.flush();
}

void xxx()
{
  enableMotors(false);
  noTone(SPEAKER);
  Serial.println("*** ABORTED ***");
  Serial.flush();
  exit(0);
}

void execute(char cmd, int i)
{
  //  Serial.printf("cmd:[%s]($d)", cmd, i);

  Serial.print("cmd:[");
  Serial.print(cmd);
  Serial.print("]");
  Serial.print("i:[");
  Serial.print(i);
  Serial.println("]");

  switch (cmd)
  {
    case 'f': // forward
    case 'b': // backward
      move(cmd, i);
      break;
    case 'r': // right
    case 'l': // left
      turn(cmd, i);
      break;
    case '*': fire(i); break; // fire
    case 'h' : hold(i); break; // hold
    case 'd' : sndDone();
  }
}

void enableMotors(bool enable)
{
  digitalWrite(STEP_ENABLE, enable ? LOW : HIGH);
}

void move(char direction, int steps)
{
  if (direction == 'b') {
    digitalWrite(A_DIR, HIGH);
    digitalWrite(B_DIR, HIGH);
  }
  enableMotors(true);
  sndDrive(true);
  for (int i = 0; i < steps; i++)
  {
    digitalWrite(A_STEP, HIGH);
    digitalWrite(B_STEP, HIGH);
    delay(STEP_DELAY);
    digitalWrite(A_STEP, LOW);
    digitalWrite(B_STEP, LOW);
    delay(STEP_DELAY);
  }
  enableMotors(false);
  sndDrive(false);
  digitalWrite(A_DIR,  LOW);
  digitalWrite(B_DIR,  LOW);
}

void turn(char direction, int steps)
{
  digitalWrite(direction == 'r' ? A_DIR : B_DIR, HIGH);
  move('f', steps);
  digitalWrite(A_DIR,  LOW);
  digitalWrite(B_DIR,  LOW);
}

void fire(int times)
{
  for (int i = 0; i < times; i++)
  {
    digitalWrite(CANNON, HIGH);
    sndFire(); // loop depends on delay within fx
    digitalWrite(CANNON, LOW);
    delay(80);
  }
}

void hold(int time)
{
  delay(time);
}

void sndDrive(boolean on)
{
  if (on)
  {
    tone(SPEAKER, 37); // NOTE_D1
  }
  else
  {
    noTone(SPEAKER);
  }
}

const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_C6 = 1047;
const int NOTE_D6 = 1175;

void sndDone()
{
  tone(SPEAKER, NOTE_C4, 80);
  delay(200);
  tone(SPEAKER, NOTE_C6, 80);
  delay(200);
  tone(SPEAKER, NOTE_D4, 80);
  delay(200);
  tone(SPEAKER, NOTE_D6, 80);
  delay(200);
}

void sndFire()
{
  for (int i = 80; i < 1000; i += 20)
  {
    tone(SPEAKER, i, 4);
    delay(6);
  }
  noTone(SPEAKER);
}
