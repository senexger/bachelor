int incomingByte = 0; // for incoming serial data
int state = 0;
// state == 0 : setup
// state == 1 : waiting for incoming serial

void setup() {
  Serial.begin(115200);
  Serial.println("ESP: Setup done.");
  state = 1; //finishing startup state
}

void loop() {
  // state 1: asking state
  if (state == 1) {
    Serial.println("ESP: Press <a> to run a the first test");
    state = 2;
  }

  // state 2: reacting state
  if ( state == 2 && Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    //Serial.print("ESP: I received: ");
    //Serial.print(incomingByte, DEC);
    //Serial.println(" (97 means <a>)");

    if (incomingByte == 97) {
      Serial.println("ESP: running test ...");
      delay(3000);
      Serial.println("ESP: test done");
      Serial.println("done");
    }
    else {
      Serial.println("ESP: ERROR: wrong test parm");
      state = 1; // go back to asking state
    }
  }
}