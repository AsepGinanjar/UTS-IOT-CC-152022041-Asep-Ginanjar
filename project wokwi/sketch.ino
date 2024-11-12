#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Konfigurasi Pin
#define DHTPIN 8
#define DHTTYPE DHT22
#define GREEN_LED 5
#define YELLOW_LED 10
#define RED_LED 12
#define PUMP_RELAY 7
#define BUZZER 9

// Inisialisasi DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// WiFi dan MQTT
const char* ssid ="RIZKI ANANDA LT3";    // Ganti dengan SSID WiFi Anda
const char* password = "Kosan435"; // Ganti dengan password WiFi Anda
const char* mqtt_server = "broker.hivemq.com"; // Broker MQTT publik

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);

  // Inisialisasi Sensor dan Aktuator
  dht.begin();
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(PUMP_RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Koneksi ke WiFi
  setup_wifi();

  // Setup MQTT
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Mencoba koneksi MQTT...");
    if (client.connect("HydroponicDevice")) {
      Serial.println("terhubung");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Membaca data sensor DHT
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  // Menampilkan hasil pembacaan suhu dan kelembapan ke Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  

  // Logika threshold untuk LED dan Relay
  if (t > 35) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
  } else if (t <=35 && t >30) {
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
  }

  // Streaming data ke MQTT
  client.publish("152022041Asep_IOT/temperature", String(t).c_str());
  client.publish("152022041Asep_IOT/humidity", String(h).c_str());


if (h < 45 ) {
  digitalWrite(PUMP_RELAY, HIGH);
} else{
  digitalWrite(PUMP_RELAY, LOW);
}

// Buzzer untuk alert suhu tinggi
if (t > 35) {
  tone(BUZZER, 900); // Menghasilkan suara dengan frekuensi 1000 Hz
} else {
  noTone(BUZZER); // Matikan buzzer
}
delay(2000);

}

