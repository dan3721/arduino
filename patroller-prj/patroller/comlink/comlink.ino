#include <ESP8266WiFi.h>

const char* ssid = "Hoth";
const char* password = "";

WiFiServer server(80);

const int STOP = 2;

void setup()
{
  pinMode(STOP, OUTPUT);

  Serial.begin(115200);
  Serial.printf("-LINKING %s\n", ssid);
  Serial.flush();

  //  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) // TODO: timeout
  {
    delay(500);
  }
  server.begin();
  Serial.printf("-LINKED %s", WiFi.localIP().toString().c_str());
  Serial.flush();
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    IPAddress ip = client.remoteIP();
    //    Serial.printf("-%s connected", ip.toString().c_str());
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();

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
          digitalWrite(STOP, HIGH);
          break;
        }

        // send the cmd to the controller
        Serial.print(c);
        Serial.print(i);
      }
    }
    delay(1); // give the web browser time to receive the data
    client.stop();
    //    Serial.printf("-%s disconnected", ip.toString().c_str());
  }
}
