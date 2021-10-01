#include <Arduino.h>
#include <SPIFFS.h>


String filename = "/test.txt";
File f;
char ch;

void setup(){
  SPIFFS.begin();
  Serial.begin(115200);
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
  return;
  }

  delay(3000);
}

void loop(){
  if(SPIFFS.exists(filename)){
    f = SPIFFS.open(filename);

    int16_t i = 0;
    for(i=0; i<f.size(); i++){
      delay(500);
      ch = f.read();
      Serial.print(ch);
    }
    Serial.println();
    Serial.println("===File Ends.===");
  }
  else{
    Serial.println("File not exists.");
  }
  

  f.close();
}