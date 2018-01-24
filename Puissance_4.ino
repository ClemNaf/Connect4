/*Var for decalage*/
int shiftPin = 13;/*pin 11*/
int dataPin = 14;/*pin 14*/

/*Var for line pin control*/
int lines[7] = {2,3,4,5,6,7,8}; /*A0,A1,2,3,4,5,6*/
int redLeds =15;/*pin 12*/
int greenLeds =16;

/*Var for button control*/
int buttonLeft =12;
int buttonRight =10;
int buttonValidate =11;

unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

int buttonStateLeft= HIGH;             // the current reading from the input pin Left
int lastButtonStateLeft = HIGH;   // the previous reading from the input pin Left
unsigned long lastDebounceTimeLeft = 0;  // the last time the output pin Left was toggled

int buttonStateRight= HIGH;             // the current reading from the input pin Right
int lastButtonStateRight = HIGH;   // the previous reading from the input pin Right
unsigned long lastDebounceTimeRight = 0;  // the last time the output pin Right was toggled

int buttonStateValidate= HIGH;             // the current reading from the input pin Validate
int lastButtonStateValidate = HIGH;   // the previous reading from the input pin Validate
unsigned long lastDebounceTimeValidate = 0;  // the last time the output pin Validate was toggled

/*Var Game*/
int dataRed[7][8]={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int dataGreen[7][8]={{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0}};
int dataNull[8]={0,0,0,0,0,0,0,0};
int dataChoosen[8]={1,0,0,0,0,0,0,0};

int positionChoosen = 0;
bool waitGreen = true;
bool win=false;
int winner=redLeds;

void setup() {
  // put your setup code here, to run once:
  pinMode(shiftPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  for(int i=0; i<=6;i++){
    pinMode(lines[i],OUTPUT);
  }
  pinMode(redLeds,OUTPUT);
  pinMode(greenLeds,OUTPUT);
  
  pinMode(buttonRight,INPUT);
  pinMode(buttonLeft,INPUT);
  pinMode(buttonValidate,INPUT);

  digitalWrite(buttonRight, HIGH) ;
  digitalWrite(buttonLeft, HIGH) ;
  digitalWrite(buttonValidate, HIGH) ;

}

void loop() {
		
	/*Verification buton*/
  if(win){
    printWin(winner);
  }else{
    buttonLeftFunction();
    buttonRightFunction();
    buttonValidateFunction();
    printGame(dataRed,dataGreen);
  
    if (!waitGreen){
      checkWin(dataGreen);
    }else{
      checkWin(dataRed);
    }
  }
	
}
/*---------- BUTTON FUNCTION ----------*/
void buttonLeftFunction(){
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonLeft);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonStateLeft) {
    // reset the debouncing timer
    lastDebounceTimeLeft = millis();
  }

  if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonStateLeft) {
      buttonStateLeft = reading;

      // only toggle the LED if the new button state is HIGH
      if(buttonStateLeft!=HIGH && positionChoosen > 0){
        dataChoosen[positionChoosen]=0;
        positionChoosen--;
        dataChoosen[positionChoosen]=1;
        if(waitGreen){
          memcpy(dataRed[6], dataNull, sizeof(dataRed[6]));
          memcpy(dataGreen[6], dataChoosen, sizeof(dataGreen[6]));
        }else{
          memcpy(dataGreen[6], dataNull, sizeof(dataGreen[6]));
          memcpy(dataRed[6], dataChoosen, sizeof(dataRed[6]));
        }
       }
    }
  }
  lastButtonStateLeft = reading;
}

void buttonRightFunction(){
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonRight);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonStateRight) {
    // reset the debouncing timer
    lastDebounceTimeRight = millis();
  }

  if ((millis() - lastDebounceTimeRight) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonStateRight) {
      buttonStateRight = reading;

      // only toggle the LED if the new button state is HIGH
      if(buttonStateRight!=HIGH && positionChoosen < 6){
        dataChoosen[positionChoosen]=0;
        positionChoosen++;
        dataChoosen[positionChoosen]=1;
        if(waitGreen){
          memcpy(dataRed[6], dataNull, sizeof(dataRed[6]));
          memcpy(dataGreen[6], dataChoosen, sizeof(dataGreen[6]));
        }else{
          memcpy(dataGreen[6], dataNull, sizeof(dataGreen[6]));
          memcpy(dataRed[6], dataChoosen, sizeof(dataRed[6]));
        }
      }
    }
  }
  lastButtonStateRight = reading;
}

void buttonValidateFunction(){
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonValidate);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonStateValidate) {
    // reset the debouncing timer
    lastDebounceTimeValidate = millis();
  }

  if ((millis() - lastDebounceTimeValidate) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonStateValidate) {
      buttonStateValidate = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonStateValidate == HIGH) {
        if(waitGreen){          
          addDataGreenColonne(positionChoosen);
          dataChoosen[positionChoosen]=0;
          positionChoosen=0;
          dataChoosen[positionChoosen]=1;
          memcpy(dataRed[6], dataChoosen, sizeof(dataRed[6]));
          memcpy(dataGreen[6], dataNull, sizeof(dataGreen[6]));
          waitGreen=false;
        }else{
          addDataRedColonne(positionChoosen);
          dataChoosen[positionChoosen]=0;
          positionChoosen=0;
          dataChoosen[positionChoosen]=1;
          memcpy(dataRed[6], dataNull, sizeof(dataRed[6]));
          memcpy(dataGreen[6], dataChoosen, sizeof(dataGreen[6]));
          waitGreen=true;
        }
      }
    }
  }
  lastButtonStateValidate = reading;
}

/*---------- PRINT FUNCTION ----------*/
/*Print data1 & data2*/
void printGame(int data1[7][8],int data2[6][8]){
	putData(data1,redLeds);
	putData(data2,greenLeds);
}
/*Print all lines for 1 colour*/
void putData(int data[7][8], int colour){
  for(int i=0;i<=6;i++){
    resetPins();
    putLine(6-i,data[i],colour);
    delay(0.1);
    putLineNull(i,dataNull,colour);
  }
}
/*Print 1 line for 1 colour*/
void putLine(int i, int data[], int colour){
  resetPins();
  putPins(data);
  digitalWrite(lines[i],HIGH); 
  digitalWrite(colour,HIGH);
  delay(1);
  digitalWrite(colour,LOW);
  digitalWrite(lines[i],LOW);
}

void putLineNull(int i, int data[], int colour){
  resetPins();
  putPins(data);
  digitalWrite(lines[i],HIGH); 
  digitalWrite(colour,HIGH);
  delay(0.1);
  digitalWrite(colour,LOW);
  digitalWrite(lines[i],LOW);
}

/*Set all pin for printing device at LOW*/
void resetPins(){ 
  digitalWrite(shiftPin,LOW);
  digitalWrite(dataPin,LOW);
}

/*Add 7 led information into data from micropro*/
void putPins(int data[]){
  for (int i=7;i >=0;i--){
    resetPins();
    digitalWrite(dataPin,data[i]);delay(0.5);
    digitalWrite(shiftPin,HIGH);delay(0.5);
  }
}

/*Print who win (all leds are on for the winner)*/
void printWin(int colour){
  int dataWin[6][8]={{1,1,1,1,1,1,1},{1,1,1,1,1,1,1},{1,1,1,1,1,1,1},{1,1,1,1,1,1,1},{1,1,1,1,1,1,1},{1,1,1,1,1,1,1}};
  putData(dataWin, colour);
}

/*---------- GAME FUNCTION ----------*/
bool addLinePossible(int data[7][8],int line,int position){
	bool possible;	
	possible=true;
	
	if (dataRed[line][position]==1||dataGreen[line][position]==1){
		possible=false;
	}
	return possible;
}

void addDataRedColonne(int colonne){
  int line=7;
  for (int i=0;i<=7;i++){  
    if (dataRed[i][colonne]==0 && dataGreen[i][colonne]==0){
      if (i<=line){
        line=i;
      }
    } 
  }

  dataRed[line][colonne]=1;
}

void addDataGreenColonne(int colonne){
  int line=7;
  for (int i=0;i<=7;i++){  
    if (dataRed[i][colonne]==0 && dataGreen[i][colonne]==0){
      if (i<=line){
        line=i;
      }
    } 
  }
  dataGreen[line][colonne]=1;
}

// Cette fonction déclare la victoire si 4 pions de même type sont alignés dans le tableau.
void checkWin (int data[7][8]){
    int victoire=0 ;

    if (test_diagD(data) || test_diagG(data)|| test_horiz(data)|| test_vertical(data)) //validation des fonctions tests.
    {
        win = true ;
        if (!waitGreen){
          winner=greenLeds;
        }else{
          winner=redLeds;
        }
    }
}

// Cette fonction teste si il y a 4 jetons du même type X ou O alignée de façon horizontale dans le tableau et ainsi déclare la victoire.
bool test_horiz(int t[7][8]){
    int i,j;
  bool  ok=false;
    for(i=0; i<=5; i++)
    {
        for(j=0; j<=3; j++)
        {
            if(t[i][j]==1 && t[i][j+1]==1  && t[i][j+2]==1  && t[i][j+3]==1)
            {
                ok = true;
            }
        }
    }
    return ok;
}

// Cette fonction teste si il y a 4 jetons du même type X ou O alignée de façon verticale dans le tableau et ainsi déclare la victoire.
bool test_vertical(int t[7][8]){
    int i,j;
    bool ok = false;
    for(i=0; i<=2; i++)
    {
        for(j=0; j<=6; j++)
        {
            if(t[i][j]==1 && t[i+1][j]==1  && t[i+2][j]==1  && t[i+3][j]==1)
            {
                ok = true;
            }
        }
    }
    return ok;
}

// Cette fonction teste si il y a 4 jetons du même type X ou O alignée dans une diagonale en partant de la droite dans le tableau et ainsi déclare la victoire.
bool test_diagD(int t[7][8]){
    int i,j;
  bool  ok;
    ok = false;
    for(i=0; i<=2; i++)
    {
        for(j=0; j<=6; j++)
        {
            if(t[i][j]==1 && t[i+1][j+1]==1  && t[i+2][j+2]==1  && t[i+3][j+3]==1)
            {
                ok = true;
            }
        }
    }
    return ok;
}

// Cette fonction teste si il y a 4 jetons du même type X ou O alignée dans une diagonale en partant de la droite dans le tableau et ainsi déclare la victoire.
bool test_diagG(int t[7][8]){
    int i,j;
  bool  ok;
    ok = false;
    for(i=0; i<=5; i++)
    {
        for(j=6; j>0; j--)
        {
            if(t[i][j]==1 && t[i+1][j-1]==1  && t[i+2][j-2]==1  && t[i+3][j-3]==1)
            {
                ok = true;
            }
        }
    }
    return ok;
}


