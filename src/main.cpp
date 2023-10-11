#include <Arduino.h>
#include <Notecard.h>
//#include <EEPROM.h>

#define DEBUG 0

#if DEBUG == 0
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugln2Param(x, y) Serial.println(x, y)


#else
#define debug(x)
#define debugln(x)
#endif

//#define usbSerial Serial
#define productUID "com.gmail.onyemandukwu:handypager"
Notecard notecard;

J *req=nullptr;
char *toNumber; // number to send message to

// the setup function runs once when you press reset or power the board
void setup()
{
  delay(2500);
Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(11,INPUT);
  digitalWrite(LED_BUILTIN,1);

notecard.begin();
notecard.setDebugOutputStream(Serial);

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
debugln(JConvertToJSONString(rsp));
J *body= JGetObject(rsp,"body");
toNumber = JGetString(body,"phone_number");
debugln(toNumber);
}


// set up auxgpio: make aux1 input and turns off  all other  auxgpio
req = NoteNewRequest("card.aux");
JAddStringToObject(req, "mode", "gpio");

J *pins = JAddArrayToObject(req, "usage");
JAddItemToArray(pins, JCreateString("input-pullup"));   // AUX1
JAddItemToArray(pins, JCreateString("off"));   // AUX2
JAddItemToArray(pins, JCreateString("off"));  // AUX3
JAddItemToArray(pins, JCreateString("off")); // AUX4
NoteRequest(req);


// sending message
 debugln("starting request");
  req = notecard.newRequest("note.add");
  
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "msg.qo");
    JAddBoolToObject(req, "sync", true);
    J *body = JAddObjectToObject(req, "body");
    if (body)
    {
      JAddStringToObject(body, "To",toNumber);
  //get number from eeprom
   }
    }
    notecard.sendRequest(req);
    delay(1000);
debugln("message sent");
  //when done go back to sleep
  // the arm command set the ATTN pin on the notecarrier low,
  // I have connected the ATTN pin to the EN pin of SWAN
  // so when the ATTN pin is LOW swan will go to sleep
debugln("going back to sleep");
digitalWrite(LED_BUILTIN,1);
req = NoteNewRequest("card.attn");
JAddStringToObject(req, "mode", "arm,auxgpio");
NoteRequest(req);
}

// the loop function runs over and over again forever
void loop()
{
  //Serial.println("now in loop");
 // Serial.println(digitalRead(11));
//   digitalWrite(LED_BUILTIN, digitalRead(11));
// if(digitalRead(PAIGER_BUTTON)){
//   Serial.println("starting request");
//   req = notecard.newRequest("note.add");
  
//   if (req != NULL)
//   {
//     JAddStringToObject(req, "file", "msg.qo");
//     JAddBoolToObject(req, "sync", true);
//     J *body = JAddObjectToObject(req, "body");
//   //   if (body)
//   //   {
//   //     JAddStringToObject(body, "To","+2349056149453" );
//   // get number from eeprom
//   //  }
//     }
//     notecard.sendRequest(req);
//     delay(1000);
//   }
}

