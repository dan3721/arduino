#include <ESP8266WiFi.h>

const char* ssid = "Hoth";
const char* password = "";

WiFiServer server(80);

void setup()
{
  //  Serial.begin(115200);
  Serial.begin(9600); // because we are communicating to the uno
  Serial.println("CLEAR");

  delay(1000);

  Serial.printf("LINKING %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) // TODO: timeout
  {
    delay(500);
  }
  server.begin();
  Serial.printf("LINKED %s", WiFi.localIP().toString().c_str());
}

void loop()
{
  WiFiClient client = server.available();
  // wait for a client (web browser) to connect
  if (client)
  {
    Serial.print("[Client connected]\n");
    while (client.connected())
    {
      if (client.available())
      {
        String line = client.readStringUntil('\r');
//        Serial.print(line);
        Serial.printf("%s\n", line.c_str());

        break;

        // wait for end of client's request, that is marked with an empty line
//        if (line.length() == 1 && line[0] == '\n')
//        {
//          client.println("ACK");
//          break;
//        }
        
      }
    }
    delay(1); // give the web browser time to receive the data

    // close the connection:
    client.stop();
    Serial.printf("[Client disonnected]/n");
  }
}
