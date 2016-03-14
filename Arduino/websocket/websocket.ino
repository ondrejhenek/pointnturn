/*
 * WebSocketClient.ino
 *
 *  Created on: 24.05.2015
 *
 * Thanks to article http://www.instructables.com/id/Esp8266Websockets-RGB-LED-controller/
 * I have found working Websocket implementation and surprisingly live collaborator
 * DOWNLOAD AT https://github.com/Links2004/arduinoWebSockets
 * Maybe you will need to reinstall ESP8266 boards to Arduino IDE... Maybe not, try! ;)
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient.h>

#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;


#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {


    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                USE_SERIAL.printf("[WSc] Connected to url: %s\n",  payload);
				
			    // send message to server when Connected
				webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);

            // send data to server
            // webSocket.sendBIN(payload, lenght);
            break;
    }
    delay(1000);

}

void setup() {
    // USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }

    WiFiMulti.addAP("SDU-GUEST", "");

    //WiFi.disconnect();
    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    webSocket.begin("echo.websocket.org", 80);
    //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
    webSocket.onEvent(webSocketEvent);

}

void loop() {
    webSocket.loop();
    if (Serial.available()) {
        String xxx = Serial.readString();
        webSocket.sendTXT(xxx);
    }
}
