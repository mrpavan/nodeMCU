#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
/* define port */
WiFiClient espClient;
PubSubClient client(espClient);
/* WIFI settings */
const char* ssid = "KumarsiPhone";
const char* password = "0105000008";
const char* mqttusername = "lolin-nodemcuv3-ultrasonic_1958";
const char* mqttpassword = "1552535810_1958";
String DEVICE_SERIAL = "NodeMCUUltrasonic01";
//Backend credentials
const char* mqtt_server = "mqtt.iot.ideamart.io"; //mqtt.exampledomain.com
char* EVENT_TOPIC = "lolin/nodemcuv3/ultrasonic/common";
String SUB_TOPIC_STRING = "+/" + DEVICE_SERIAL + "/lolin/nodemcuv3/ultrasonic/sub";
String PUB_TOPIC_STRING = "/" + DEVICE_SERIAL + "/lolin/nodemcuv3/ultrasonic/pub";
char msg[1024];
//String data = "";
//String actionData = "";
//int actionParameter1;
//int actionParameter2;

//====================================================Connecting to Wifi
void setup_wifi() {
	delay(10);
	// We start by connecting to a WiFi network
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
		}
	randomSeed(micros());
	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	//=====================================================MQTT
	}
/*
//receiving a message
void callback(char* topic, byte* payload, long length) {
	yield();
	delay(300);
	for (int i = 0; i < length; i++) {
		msg[i] = NULL;
	}
	Serial.print("Message arrived [");
	Serial.print(SUB_TOPIC_STRING);
	Serial.print("] ");
	//NodeMCU.print("L1:");
	for (int i = 0; i < length; i++) {
		msg[i] = (char)payload[i];
		Serial.print(msg[i]);
		//NodeMCU.print(msg[i]);
		}
		// NodeMCU.println();
		// delay(3000);
    StaticJsonDocument<800> root;
    deserializeJson(root, msg);
   
		if (strcmp(root["action"], "turnOn") == 0) {
			actionParameter1 = (int)root["param"]["intensity"];
			analogWrite(D1, actionParameter1);
			}
		else if (strcmp(root["action"], "turnOff") == 0) {
			analogWrite(D1, 0);
			//actionParameter2 = root["param"]["msg"].as<String>();
			}
			else if(strcmp(root["action"], "deviceStatus") == 0) {
					//OK action is success. Let's report this to backend
					//Let's derive the unique ID of the received action message
					String topicStr(topic);
					String uniqueID = topicStr.substring(0, topicStr.indexOf('/'));
					//Let's add this unique ID to pub topic of the device
					String pubtopic = uniqueID + PUB_TOPIC_STRING;
					//Publish the response message to backend
					char* pubmsg = "{\"deviceStatus\":\"OK\"}\"";
					client.publish(pubtopic.c_str(), pubmsg);
					Serial.println("");
					Serial.println("Response published to:");
					Serial.print(pubtopic);
						}
					Serial.println(data);
					Serial.println();
					yield();
			} 
      */
void reconnect() {
	// Loop until we're reconnected
	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Create a random client ID
		String clientId = "ESP8266Client-";
		clientId += String(random(0xffff), HEX);
		// Attempt to connect
		if (client.connect(clientId.c_str(), "lolin-nodemcuv3-ultrasonic_1958", "1552535810_1958")) {
			Serial.println("connected");
			//subscribe to the topic
			const char* SUB_TOPIC = SUB_TOPIC_STRING.c_str();
			client.subscribe(SUB_TOPIC);
			} else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
				}
			}
		}

void publish_message(const char* message) {
	Serial.println(message);
	client.publish(EVENT_TOPIC, message);
		}
		void readSensorValues(){
			float h = 40 ;//dht.readHumidity();
			//float t = 23 ;//dht.readTemperature(); 
			String message = 	"{\"eventName\":\"getDistance\",\"status\":\"none\",\"evenparam\":{\"distance\":\"" + 	String(h) + "\"},\"id\":\""+ DEVICE_SERIAL +"\"}";		
      char* msg = (char*)message.c_str();
			publish_message(msg); //send the event to backend
			//Serial.println(message);
		}

void send_event() {
	//Should call publish_message() function to send events. Pass your message as parameter
	publish_message(msg);
}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	pinMode(D1, OUTPUT);
	setup_wifi();
	client.setServer(mqtt_server, 1883);
	Serial.println("Distance from Ultrasonic Sensor\n\n");
	}
void loop() {
	// put your main code here, to run repeatedly:
	if (WiFi.status() != WL_CONNECTED) {
		setup_wifi();
		}
		if (WiFi.status() == WL_CONNECTED && !client.connected()) {
			reconnect();
			}
	
	readSensorValues(); 
	delay(10000);
	client.loop();
	}
