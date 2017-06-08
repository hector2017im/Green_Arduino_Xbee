#include <SoftwareSerial.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Direccion Mac para el arduino.
//IPAddress server(207,249,127,215);  // IP del servidor al cual se hara la conexion. 
char server[] = "tatallerarquitectura.com";
IPAddress ip(192, 168, 56, 100); // IP para el Arduino.

EthernetClient client;

int datos[4]; // Vector para obtener los datos enviados por Xbee.
String humedadSuelo = "", humedadRelativa = "", temperatura = "", sector = "", tipo = "";

SoftwareSerial  xbee(2,3); 

void setup(){          
  xbee.begin(9600); 
  Serial.begin(9600);

  while (!Serial){;}
  
  if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");    
        Ethernet.begin(mac, ip);
  } 
  delay(1000);
  Serial.println("Conectando con Internet");
  
}//llave del setup.

void loop(){
  
  if (xbee.available()>0){                
              
  for (int i = 0; i < 4; i++)
    datos[i] = xbee.read();       
  
  humedadSuelo = datos[0];
  //Serial.println(datos[0]);
  humedadRelativa = datos[1];
  //Serial.println(datos[1]);
  temperatura = datos[2];
  //Serial.println(datos[2]);
  sector = datos[3];
  //Serial.println(datos[3]);

  if(sector.equals("3"))
    tipo = "SectorTipo3";
  if(sector.equals("4"))
    tipo = "SectorTipo4";

  Serial.println(humedadSuelo);
  Serial.println(humedadRelativa);
  Serial.println(temperatura);
  Serial.println(sector);
  Serial.println(tipo);

  
  if (client.connect(server, 80)>0) {  // Conexion con el servidor
    client.print("GET /fiware/json.php?temperatura="); // Enviamos los datos por GET
    client.print(temperatura);
    client.print("&humedadRelativa=");
    client.print(humedadRelativa);
    client.print("&humedadSuelo=");
    client.print(humedadSuelo);
    client.print("&type=");
    client.print(tipo);
    client.print("&position=");
    client.print(sector);
    client.println(" HTTP/1.0");
    client.println("Host: tatallerarquitectura.com");
    client.println("User-Agent: Arduino 1.0");
    client.println();

    Serial.println("Datos Enviados Satisfactoriamente papu");
  } else {
    Serial.println("Fallo en la conexion");
  }
  if (!client.connected()) {
    Serial.println("Disconnected!");
  }
  client.stop();
  client.flush();
  delay(5000); // Espero un minuto antes de tomar otra muestra
  }
}//llave del loop.





