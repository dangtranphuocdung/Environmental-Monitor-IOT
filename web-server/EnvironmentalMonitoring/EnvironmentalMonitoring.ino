#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Firebase_ESP_Client.h>
#include <secret.h>

#define WIFI_SSID "Dang Tran Phuoc Dung"
#define PASSWORD "12345678"
#define API_KEY "AIzaSyCXod4ZFvS270oAhHXNGBdI1h-FLPMn8zY"
#define DATABES_URL "https://save-temp-humid-data-default-rtdb.firebaseio.com/"
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_TOPIC_COMMAND "house/livingroom/led/set"


//Khai báo biến
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic_command = "house/livingroom/led/set";
const int dhtPin = 27;
bool signUp = false;
float temp;
float humid;
unsigned long timeInterval = 5000;
unsigned long currentTime;
unsigned long lastSendTime = 0;
const int ledPin = 18;
const int freq = 5000;
const int resolution = 10;
int pwmValue;
//Khai báo đối tượng
DHT dht(dhtPin, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
PubSubClient mqttClient(espClient);

void tokenStatusCallback(TokenInfo info) {
  if (info.status == token_status_ready) {
    Serial.println("Token is ready, Firebase authenticated.");
  } else {
    Serial.printf("Token error: %s\n", info.error.message.c_str());
  }
}

void callback(const char[] topic, byte* payload, unsigned int length)
  Serial.print("Message from topic: ");
  Serial.println(topic);
  
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Context: ");
  Serial.println(message);

  if (message == "ON") {
    ledcWrite(ledPin, 1023); }
  else if (message == "OFF") {
    ledcWrite(ledPin, 0);
  }
}

/**void onConnection() {
  Serial.println("Connected to MQTT");
  mqttClient.subscribe(mqtt_topic_command);
  Serial.println("Subscribed to topic: ");
  Serial.println(mqtt_topic_command);
}**/

//reconnect and subscribe when WiFi is lost
void reconnect_mqtt() {
  while(!mqttClient.connect()) {
    Serial.print("Reconnecting MQTT ...")
    String clientId = "ESP32_Client_";
    clientId += String(random(0xffff), HEX);
    
    // Thử kết nối
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Da ket noi MQTT!");
      
      // QUAN TRỌNG: Subscribe (đăng ký) NGAY SAU KHI KẾT NỐI
      mqttClient.subscribe(MQTT_TOPIC_COMMAND);
      Serial.println("Da subscribe vao topic lenh.");
      
    } else {
      Serial.print("Fail, rc=");
      Serial.print(mqttClient.state());
      Serial.println("Try again after 5s");
      delay(5000); // Đợi 5s rồi "while" sẽ chạy lại
  }
}

void setup() {
  // attach pin
  ledcAttach(ledPin, freq, resolution);
  lcd.init(); //initialize lcd
  dht.begin(); //begin sensor
  delay(2000);
  //Connect wifi
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, PASSWORD);
  Serial.println("Connecting to Wifi ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi is not connected. ");
    delay(2000);
  }
  Serial.println();
  Serial.print("Connect to wifi: " + String(WIFI_SSID));
  Serial.println("Local IP adress: ");
  Serial.println(WiFi.localIP());

// Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABES_URL;

// Firebase anonymous login
  if (Firebase.signUp(&config, &auth, "", "")) { 
    //&config chứa thông tin của prj, apikey, dtb_url, &auth chứa thông tin đăng nhập anonymous
    Serial.println("Sign in sucessfull");
    signUp = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());

  }
// Callback and start firebase
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

//initialize MQTT
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback)
} 

void loop() {
  if (!mqttClient.connected()) {
    reconnect_mqtt();
  }

  mqttClient.loop();
  // non-blocking time
  currentTime = millis();
  if (currentTime - lastSendTime >= timeInterval) {
    lastSendTime = currentTime;
  
  
  /*if (Firebase.ready() && signUp == true && (currentTime -  lastSendTime >= timeInterval) {
    lastSendTime = currentime (lỗi vì phải set lastSendtime thành currentTime, ko phải ngược lại)
  }*/
  // đọc dữ liệu từ cảm biến
    temp = dht.readTemperature();
    humid = dht.readHumidity();
  
  //check xem dữ liệu có đúng k, nếu ko thì bỏ qua
    if (isnan(temp) || isnan(humid)){
      Serial.print("Failed to read data from sensor");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Sensor error");
      return;
    }
  //display data on LCD
    lcd.clear();
    lcd.backlight(); //make sure blacklight is on
    lcd.setCursor(0,0); //Set con trỏ to character 2 on line 1
    lcd.print("Temp is " + String(temp));
    lcd.setCursor(0,1); //Set con trỏ tại character 2, line 2
    lcd.print("Humidity is " + String(humid));
  
  //send data to google firebase
    if (Firebase.ready() && signUp) {
      if(Firebase.RTDB.setFloat(&fbdo, "Sensor/temp_data", temp)) {
        Serial.println(); 
        Serial.print(temp);
        Serial.print("Saved to " + fbdo.dataPath());
        Serial.print("("+ fbdo.dataType() + ")");
      }
      else {
        Serial.println("Failed: " + fbdo.errorReason());

      }

      if (Firebase.RTDB.setFloat(&fbdo, "Sensor/humid_data", humid)) {
        Serial.println(); 
        Serial.print(humid);
        Serial.print("Saved to " + fbdo.dataPath());
        Serial.print("("+ fbdo.dataType() + ")");
      }
      else {
        Serial.println("Failed: " + fbdo.errorReason());
      }
    }
  }
  
  // READ DATA FROM RTDB TO CONTROL A LED ATTACHED TO ESP32

  /**if (Firebase.RTDB.getInt(&fbdo, "Control/LED")) {
    if (fbdo.dataType() == "int") {
      pwmValue = fbdo.intData();
      Serial.println("Read successfully from " + fbdo.dataPath() + ":" + "("+ fbdo.dataType() + ")");
      ledcWrite(ledPin, pwmValue);
    }
    else {
      Serial.println("Failed: " + fbdo.errorReason());
    }
  }

}**/
