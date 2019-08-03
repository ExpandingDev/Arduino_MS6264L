/*
 * MS6264 Test
 * This is an example program of how to interface with the Mosel 6264 static RAM chip.
 * This example is limited as only 8 of the 13 available address pins (A0-A12) were attached to the arduino, so instead of 8 Kb of RAM being available, only a measly 256 bytes are exposed.
 * However, you can easily extend this code to include all 13 pins. Modify the setAddress() function and include pinout definitions in the start of the program and remember to initialize your pins at the start of your code.
 * 
 * Author: Tyler Sengia
 */

/* Pin definitions */
#define W_PIN 24
#define E1_PIN 23
#define E2_PIN 22
#define G_PIN 25

//Note: I have only attached pins A0-A7 to the arduino, the rest are attached to ground. To extend your address space, attach the rest of the address pins to the arduino and add in the pin numbers here.
#define A0_PIN 36
#define A1_PIN 37
#define A2_PIN 38
#define A3_PIN 39
#define A4_PIN 40
#define A5_PIN 41
#define A6_PIN 42
#define A7_PIN 43

#define DQ0_PIN 46
#define DQ1_PIN 47
#define DQ2_PIN 48
#define DQ3_PIN 49
#define DQ4_PIN 50
#define DQ5_PIN 51
#define DQ6_PIN 52
#define DQ7_PIN 53

void deactivateChip() {
  digitalWrite(G_PIN, HIGH);
  digitalWrite(W_PIN, HIGH);
  digitalWrite(E1_PIN, HIGH);
  digitalWrite(E2_PIN, LOW);
}

void activateChip() {
    digitalWrite(G_PIN, HIGH);
    digitalWrite(W_PIN, HIGH);
    digitalWrite(E1_PIN, LOW);
    digitalWrite(E2_PIN, HIGH);
}

void setDQMode(int mode) {
  pinMode(DQ0_PIN, mode);
  pinMode(DQ1_PIN, mode);
  pinMode(DQ2_PIN, mode);
  pinMode(DQ3_PIN, mode);
  pinMode(DQ4_PIN, mode);
  pinMode(DQ5_PIN, mode);
  pinMode(DQ6_PIN, mode);
  pinMode(DQ7_PIN, mode);
}

void setAddress(byte address) {
    //NOTE: I have only attached 8 wires of the address port to the arduino, the rest (A8-A11) I have attached to ground.
    //If you would like to extend your available address space, just copy and paste these lines, and also change your data type to something larger than a byte (probably an unsigned short would work).
    digitalWrite(A0_PIN, (address & 0x01) == 0x01); 
    digitalWrite(A1_PIN, (address & 0x02) == 0x02); 
    digitalWrite(A2_PIN, (address & 0x04) == 0x04); 
    digitalWrite(A3_PIN, (address & 0x08) == 0x08); 
    digitalWrite(A4_PIN, (address & 0x10) == 0x10); 
    digitalWrite(A5_PIN, (address & 0x20) == 0x20); 
    digitalWrite(A6_PIN, (address & 0x40) == 0x40); 
    digitalWrite(A7_PIN, (address & 0x80) == 0x80); 
}

short setData(byte data) {
    setDQMode(OUTPUT);
    digitalWrite(DQ0_PIN, ((data & 0x01) == 0x01) ? HIGH : LOW); 
    digitalWrite(DQ1_PIN, ((data & 0x02) == 0x02) ? HIGH : LOW); 
    digitalWrite(DQ2_PIN, ((data & 0x04) == 0x04) ? HIGH : LOW); 
    digitalWrite(DQ3_PIN, ((data & 0x08) == 0x08) ? HIGH : LOW); 
    digitalWrite(DQ4_PIN, ((data & 0x10) == 0x10) ? HIGH : LOW); 
    digitalWrite(DQ5_PIN, ((data & 0x20) == 0x20) ? HIGH : LOW); 
    digitalWrite(DQ6_PIN, ((data & 0x40) == 0x40) ? HIGH : LOW); 
    digitalWrite(DQ7_PIN, ((data & 0x80) == 0x80) ? HIGH : LOW);
}

void initPins() {
  pinMode(A0_PIN, OUTPUT);
  pinMode(A1_PIN, OUTPUT);
  pinMode(A2_PIN, OUTPUT);
  pinMode(A3_PIN, OUTPUT);
  pinMode(A4_PIN, OUTPUT);
  pinMode(A5_PIN, OUTPUT);
  pinMode(A6_PIN, OUTPUT);
  pinMode(A7_PIN, OUTPUT);
  
  pinMode(DQ0_PIN, OUTPUT);
  pinMode(DQ1_PIN, OUTPUT);
  pinMode(DQ2_PIN, OUTPUT);
  pinMode(DQ3_PIN, OUTPUT);
  pinMode(DQ4_PIN, OUTPUT);
  pinMode(DQ5_PIN, OUTPUT);
  pinMode(DQ6_PIN, OUTPUT);
  pinMode(DQ7_PIN, OUTPUT);

  setAddress(0x00);
  setData(0x00);
  
  pinMode(W_PIN, OUTPUT);
  pinMode(E1_PIN, OUTPUT);
  pinMode(E2_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  
  deactivateChip();
}

void writeData(byte addr, byte data) {
  setAddress(addr);
  setData(data);
  digitalWrite(W_PIN, LOW);
  digitalWrite(W_PIN, HIGH);
}

byte readData(byte address) {
  setData(0x00);
  setDQMode(INPUT);
  setAddress(address);
  digitalWrite(G_PIN, LOW);
  byte b = 0x00;
  b |= (digitalRead(DQ0_PIN) == HIGH) ? 0x01 : 0x00;
  b |= (digitalRead(DQ1_PIN) == HIGH) ? 0x02 : 0x00;
  b |= (digitalRead(DQ2_PIN) == HIGH) ? 0x04 : 0x00;
  b |= (digitalRead(DQ3_PIN) == HIGH) ? 0x08 : 0x00;
  b |= (digitalRead(DQ4_PIN) == HIGH) ? 0x10 : 0x00;
  b |= (digitalRead(DQ5_PIN) == HIGH) ? 0x20 : 0x00;
  b |= (digitalRead(DQ6_PIN) == HIGH) ? 0x40 : 0x00;
  b |= (digitalRead(DQ7_PIN) == HIGH) ? 0x80 : 0x00;
  digitalWrite(G_PIN, HIGH);
  return b;
}

void writeZeros() {
  for(byte b = 0; b < 0xFF; b = b + 0x01) {
    writeData(b, 0x00);
  }
}

void setup() {
  
  initPins();
  delay(10);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("MS6264 Test Begin");
  Serial.flush();
  delay(500);

  activateChip();
  writeZeros();
  unsigned short range = 5;
  unsigned short i = 0;
  byte d = 'R';
  byte buff[] = {'h', 'e', 'l', 'l', 'o'};
  for(unsigned short i = 0; i < range; i++) {
    writeData(i, buff[i]);
    Serial.print("Writing to 0x");
    Serial.print(i, HEX);
    Serial.print("\n");
    Serial.flush();
  }

  for(byte i = 0x00; i < range; i = i + 0x01) {
    byte b = readData(i);
    Serial.print("Reading 0x");
    Serial.print(i, HEX);
    Serial.print(": 0x");
    Serial.print(b, HEX);
    Serial.print(", ");
    Serial.print((char) b);
    Serial.print('\n');
  }

  //deactivateChip();
}

void loop() {
  if(Serial.available() > 0) {
    String command = Serial.readStringUntil("|");
    if(command.startsWith("READ|")) {
      Serial.println("Address?");
      while(Serial.available() <= 0) {
        //Wait
      }
      unsigned short addr = (unsigned short) Serial.parseInt();
      Serial.print("Reading from 0x"); Serial.print(addr, HEX); Serial.print(": 0x");
      byte b = readData(addr);
      Serial.print(b, HEX);
      Serial.println("\n");
    }
    else if(command.startsWith("WRITE|")) {
      Serial.println("Address?");
      while(Serial.available() <= 0) {
        //Wait
      }
      unsigned short addr = (unsigned short) Serial.parseInt();

      Serial.println("Value?");
      while(Serial.available() <= 0) {
        //Wait
      }
      unsigned short data = (unsigned short) Serial.parseInt();
      
      Serial.print("Writing 0x"); Serial.print(addr, HEX); Serial.print(": 0x");
      Serial.print(data, HEX);
      Serial.println("\n");
      writeData(addr, data);
    }
    else if (command.startsWith("WRITE_TEXT|")) {
      Serial.println("Text to write? End with a pipe char |");
      while(Serial.available() <= 0) {
        //Wait
      }
      String text = Serial.readStringUntil("|");
      Serial.println("Writing from 0x00 to 0xFF...");
      for(unsigned short i = 0; i < text.length() -1; i++) { //text.length() -1 is so we don't include the pipe char |
        char c = text.charAt(i);
        writeData((byte) i, (byte) c);
      }
      Serial.println("done");
    }
    else if (command.startsWith("READ_TEXT|")) {
      Serial.println("Reading from 0x00 to 0xFF");
      for(unsigned short i = 0; i < 255; i++) {
        Serial.write(readData(byte(i)));
      }
      Serial.println("Done");
    }
    else {
       Serial.println("Unknown command, version 0.1 code.");
    }
  }
}
