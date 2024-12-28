#include <WiFi.h>
#include <ESP32Servo.h>

// Crear objetos Servo para controlar los diferentes dedos
Servo miMenique;
Servo miAnular;
Servo miMedio;
Servo miIndice;
Servo miPulgar;

// Pines GPIO para cada servo
static const int anular26 = 26;
static const int menique27 = 27;
static const int medio25 = 25;
static const int indice33 = 33;
static const int pulgar32 = 32;

// Credenciales de red
const char* ssid = "Livebox6-5853";
const char* password = "Do6ZEsZvt7vS";

// Configuración del servidor web en el puerto 80
WiFiServer server(80);
String header; // Almacena la solicitud HTTP

void setup() {
  Serial.begin(115200);

  // Adjuntar servos a sus respectivos pines
  miMenique.attach(menique27);
  miAnular.attach(anular26);
  miMedio.attach(medio25);
  miIndice.attach(indice33);
  miPulgar.attach(pulgar32);

  // Conectar al WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.println(WiFi.status()); // Agregar esto para depurar
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

// Definición de las funciones de movimiento de la mano
void mano_cerrada() {
  miMenique.write(0);
  miAnular.write(0);
  miMedio.write(20);
  miIndice.write(20);
  miPulgar.write(20);
  delay(3000);
}

void mano_abierta() {
  miMenique.write(20);
  miAnular.write(20);
  miMedio.write(0);
  miIndice.write(0);
  miPulgar.write(0);
  delay(3000);
}

void dedo_1a1() {
  miMenique.write(20);
  delay(1000);
  miAnular.write(20);
  delay(1000);
  miMedio.write(0);
  delay(1000);
  miIndice.write(0);
  delay(1000);
  miPulgar.write(0);
  delay(2000);

  miMenique.write(0);
  delay(1000);
  miAnular.write(0);
  delay(1000);
  miMedio.write(20);
  delay(1000);
  miIndice.write(20);
  delay(1000);
  miPulgar.write(20);
  delay(3000);
}

void loop() {
  WiFiClient client = server.available();   // Escuchar para clientes entrantes

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) { 
      if (client.available()) {             
        char c = client.read();             
        Serial.write(c);                    
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Respuesta HTTP estándar
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Generar la página HTML con botones para cada función
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; }");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<body><h1>ESP32 Control de Servos</h1>");
            
            // Botón para cerrar la mano
            client.println("<p><a href=\"/mano_cerrada\"><button class=\"button\">Cerrar Mano</button></a></p>");
            // Botón para abrir la mano

            client.println("<p><a href=\"/mano_abierta\"><button class=\"button\">Abrir Mano</button></a></p>");
            // Botón para mover los dedos uno a uno
            client.println("<p><a href=\"/dedo_1a1\"><button class=\"button\">Mover Dedo 1 a 1</button></a></p>");
            

            client.println("</body></html>");
            
            // Procesar la solicitud GET y ejecutar la función correspondiente
            if (header.indexOf("GET /mano_cerrada") >= 0) {
              mano_cerrada();
              Serial.println("Ejecutando: Mano Cerrada");
            } 
            else if (header.indexOf("GET /mano_abierta") >= 0) {
              mano_abierta();
              Serial.println("Ejecutando: Mano Abierta");
            } 
            else if (header.indexOf("GET /dedo_1a1") >= 0) {
              dedo_1a1();
              Serial.println("Ejecutando: Mover Dedo 1 a 1");
            }

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }
}