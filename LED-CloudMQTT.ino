//Nesse código utilizamos o servidor Cloud MQTT para conexão com o Broker

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char* SSID          = "A001";
const char* PASSWORD      = "avsi@015";

const char* MQTT_SERVER   = "m11.cloudmqtt.com";
const int MQTT_PORT       =  16270;
const char* MQTT_USER     = "qidenxsf";
const char* MQTT_PASSWORD = "lr9ov3PhqxIk";
const char* MQTT_CLIENT   = "ESP8266-Client";

const char* TOPICO_ESCUTA  = "topicoEscuta";
const char* TOPICO_ENVIO   = "topicoEnvio";

// GPIO 4 (D3)
const int PIN_LED_1 = 13;
// GPIO 5 (D4)
const int PIN_LED_2 = 5;

WiFiClient espClient;
PubSubClient MQTT(espClient);


void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED)
     return;
     
  Serial.println("Conectando-se na rede: " + String(SSID));
  Serial.println("Aguarde");
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg;

  for(int i = 0; i < length; i++) 
  {
     char c = (char)payload[i];
     msg += c;
  }
  
  Serial.print("Mensagem [");
  Serial.print(topic);
  Serial.print("]: " + msg);
  Serial.println();

  if (msg.equals("1")) {
    digitalWrite(PIN_LED_1, HIGH);
    digitalWrite(PIN_LED_2, HIGH);
    MQTT.publish(TOPICO_ENVIO, "1");
  } 
  else if (msg.equals("0")){
    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);
    MQTT.publish(TOPICO_ENVIO, "0");
  }

}

void initMQTT() {
  MQTT.setServer(MQTT_SERVER, MQTT_PORT);
  MQTT.setCallback(callback);
}

// Função para conexão
void reconnectMQTT() {
    while (!MQTT.connected()) {
      Serial.print("Tentado conectar ao broker MQTT: " + String(MQTT_SERVER) + ":" + String(MQTT_PORT) + " :");
      if (MQTT.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD)) {
        Serial.println("conectado");
        MQTT.publish(TOPICO_ENVIO, "Cliente conectado ao broker");
        MQTT.subscribe(TOPICO_ESCUTA);
      } 
      else {
        Serial.print("falha, rc=");
        Serial.print(MQTT.state());
        Serial.println(" tenta novamente após 5 segundos");
        delay(5000);
      }
    }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_LED_1, OUTPUT);
  pinMode(PIN_LED_2, OUTPUT);
  digitalWrite(PIN_LED_1, LOW);
  digitalWrite(PIN_LED_2, LOW);
  
  reconnectWiFi();
  initMQTT();
}


void loop() {
  reconnectWiFi();
  reconnectMQTT();
  MQTT.loop();
}
