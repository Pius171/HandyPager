#include <Arduino.h>
#include <Notecard.h>
//#include <EEPROM.h>

#define usbSerial Serial
#define productUID "com.gmail.onyemandukwu:handypager"
#define PAIGER_BUTTON 12
Notecard notecard;

J *req=nullptr;


// the setup function runs once when you press reset or power the board
void setup()
{
  delay(2500);
usbSerial.begin(115200);

notecard.begin();
notecard.setDebugOutputStream(usbSerial);

req = notecard.newRequest("hub.set");
JAddStringToObject(req, "product", productUID);
JAddStringToObject(req, "mode", "continuous");
notecard.sendRequest(req);

delay(2000);

req = notecard.newRequest("note.get");
if(req !=NULL ){
JAddStringToObject(req, "file", "mydata.dbs");
JAddBoolToObject(req, "sync", true);
J* rsp=notecard.requestAndResponse(req);
usbSerial.println(JConvertToJSONString(rsp));
}
  // initialize digital pin LED_BUILTIN as an output.
// {
//   "req": "card.aux",
//   "mode": "gpio",
//   "usage": ["off", "low", "high", "input"]
// }

req = NoteNewRequest("card.aux");
JAddStringToObject(req, "mode", "gpio");

J *pins = JAddArrayToObject(req, "usage");
JAddItemToArray(pins, JCreateString("input"));   // AUX1
JAddItemToArray(pins, JCreateString("off"));   // AUX2
JAddItemToArray(pins, JCreateString("off"));  // AUX3
JAddItemToArray(pins, JCreateString("off")); // AUX4

NoteRequest(req);



  pinMode(PAIGER_BUTTON,INPUT_PULLDOWN);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(11,INPUT);
  digitalWrite(LED_BUILTIN,1);
}

// the loop function runs over and over again forever
void loop()
{
  //Serial.println("now in loop");
  Serial.println(digitalRead(11));
  digitalWrite(LED_BUILTIN, digitalRead(11));
if(digitalRead(PAIGER_BUTTON)){
  Serial.println("starting request");
  req = notecard.newRequest("note.add");
  
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "msg.qo");
    JAddBoolToObject(req, "sync", true);
    J *body = JAddObjectToObject(req, "body");
  //   if (body)
  //   {
  //     JAddStringToObject(body, "To","+2349056149453" );
  // get number from eeprom
  //  }
    }
    notecard.sendRequest(req);
    delay(1000);
  }
}

