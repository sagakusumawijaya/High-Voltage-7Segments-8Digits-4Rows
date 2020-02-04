/*
 Name:		Wireless_Seven_Segment.ino
 Created:	29-Aug-19 06:46:21
 Author:	Saga Kusuma Wijaya
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/*
MAX7219 8-digit 7-segment LED test

Basic code for configuring the device and outputting info to the display.  Writes 'Arduino' on display
and then counts up on the display until "99999999" is reached.
Uses the "LedControl" Library which must be included
*/
#include "LedControl.h"
/*
Define pins for LedControl.  These can be assigned to any digital output pins on a microcontroller.
*/
#define CLK_PIN   D5 // or SCK
#define DATA_PIN  D7 // or MOSI
#define CS_PIN    D8 // or SS
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, 4);

unsigned long delaytime = 500;  // delay between character updates
long num = 0;                   // Variable to hold counter number

const char* ssid = "AndroidAP";
const char* password = "zocu1353";

ESP8266WebServer server(80);

const int led = D10;

//===============================================================================
//  Functions
//===============================================================================
void message(int p, char c, int dig, bool point = false) {
	lc.setChar(dig, p, c, point);
}

void digits2screen() {
	lc.clearDisplay(0); // Clear dsplay register
	lc.clearDisplay(1); // Clear dsplay register
	lc.clearDisplay(2); // Clear dsplay register
	lc.clearDisplay(3); // Clear dsplay register

	//serChar needs single ' not double "
	lc.setChar(0, 6, 'H', false);
	lc.setChar(0, 5, 'E', false);
	lc.setChar(0, 4, 'L', false);
	lc.setChar(0, 3, 'L', false);
	lc.setDigit(0, 2, 0, false);
	lc.setDigit(0, 1, 0, false);
	lc.setDigit(0, 0, 0, false);

	
	lc.setDigit(1, 5, 5, false);
	lc.setChar(1, 4, 'E', false);
	lc.setChar(1, 3, 'C', false);
	lc.setChar(1, 2, 'o', false);
	lc.setChar(1, 1, 'n', false);
	lc.setChar(1, 0, 'd', false);

	for (int i = 0; i < 8; i++) {
		lc.setDigit(2, i, 3, false);
	}
	for (int i = 0; i < 8; i++) {
		lc.setDigit(3, i, 4, false);
	}
}

void handleRoot() {
	if (server.args() > 0) {
		lc.clearDisplay(0);
		lc.clearDisplay(1);
		lc.clearDisplay(2);
		lc.clearDisplay(3);
		for (uint8_t i = 0; i < server.args(); i++) {
			if (server.argName(i) == "msg") {
				server.send(200, "text/plain", server.arg(i));
			}
			if (server.argName(i) == "d1") {
				String d1 = server.arg(i);
				server.send(200, "text/plain", d1);
				int pos = 0;
				bool pointExist = false;
				for (int p = d1.length() - 1; p >= 0; p--) {
					if ( (d1.charAt(p) == '.') && (!pointExist)) {
						p--;
						message(pos, d1.charAt(p), 0, true);
						pointExist = true;
					}
					else {
						message(pos, d1.charAt(p), 0);
					}
					pos++;
				}
			}
			if (server.argName(i) == "d2") {
				String d2 = server.arg(i);
				server.send(400, "text/plain", d2);
				int pos = 0;
				bool pointExist = false;
				for (int p = d2.length() - 1; p >= 0; p--) {
					if ((d2.charAt(p) == '.') && (!pointExist)) {
						p--;
						message(pos, d2.charAt(p), 1, true);
						pointExist = true;
					}
					else {
						message(pos, d2.charAt(p), 1);
					}
					pos++;
				}
			}
			if (server.argName(i) == "d3") {
				String d3 = server.arg(i);
				server.send(400, "text/plain", d3);
				int pos = 0;
				bool pointExist = false;
				for (int p = d3.length() - 1; p >= 0; p--) {
					if ((d3.charAt(p) == '.') && (!pointExist)) {
						p--;
						message(pos, d3.charAt(p), 2, true);
						pointExist = true;
					}
					else {
						message(pos, d3.charAt(p), 2);
					}
					pos++;
				}
			}
			if (server.argName(i) == "d4") {
				String d4 = server.arg(i);
				server.send(400, "text/plain", d4);
				int pos = 0;
				bool pointExist = false;
				for (int p = d4.length() - 1; p >= 0; p--) {
					if ((d4.charAt(p) == '.') && (!pointExist)) {
						p--;
						message(pos, d4.charAt(p), 3, true);
						pointExist = true;
					}
					else {
						message(pos, d4.charAt(p), 3);
					}
					pos++;
				}
			}
		}
	}
	else {
		digitalWrite(led, 1);
		server.send(200, "text/plain", "hello from esp8266!");
		digitalWrite(led, 0);
		server.send(200, "text/plain", "My Configuration");
		server.send(200, "text/plain", ssid);
		digits2screen();
	}
}

void handleNotFound() {
	digitalWrite(led, 1);
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";
	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}
	server.send(404, "text/plain", message);
	digitalWrite(led, 0);

}

//===============================================================================
//  Initialization
//===============================================================================
void setup() {
	// Initialize the MAX7219 device
	lc.shutdown(0, false); // Wakeup display
	lc.setIntensity(0, 8); // Set brightness to a medium level
	lc.clearDisplay(0);   // Clear the display
	lc.shutdown(1, false); // Wakeup display
	lc.setIntensity(1, 8); // Set brightness to a medium level
	lc.clearDisplay(1);   // Clear the display
	lc.shutdown(2, false); // Wakeup display
	lc.setIntensity(2, 8); // Set brightness to a medium level
	lc.clearDisplay(2);   // Clear the display
	lc.shutdown(3, false); // Wakeup display
	lc.setIntensity(3, 8); // Set brightness to a medium level
	lc.clearDisplay(3);   // Clear the display

	Serial.println("This is HH NodeMCU");
	Serial.println("My Configuratio is :");
	Serial.print("* SSID : ");
	Serial.println(ssid);
	Serial.print("* Password : ");
	Serial.println(password);
	Serial.println("");

	pinMode(led, OUTPUT);
	digitalWrite(led, 0);
	Serial.begin(115200);
	WiFi.begin(ssid, password);
	Serial.println("");
	Serial.println("Start to connection Network");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	if (MDNS.begin("esp8266")) {
		Serial.println("MDNS responder started");
	}

	server.on("/", handleRoot);

	server.on("/inline", []() {
		server.send(200, "text/plain", "this works as well");
			  });

	server.onNotFound(handleNotFound);

	server.begin();
	Serial.println("HTTP server started");
}
//===============================================================================
//  Main
//===============================================================================
void loop() {
	server.handleClient();
}
