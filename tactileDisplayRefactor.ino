   
long startMicros;
long endMicros;
long pulsePeriod;

//Modified by Paarangat Jain 
long startMillisArray[3];
//Modified ends here (All instances of startMillis have been replaced successfully.)long startMillis;
long startMillis;
long startMillis2;
long startMillis3;

long endMillis;
///////////// use these for troubleshooting refresh rate
long countTime;
long oldCountTime;
long fastestTime=1000;
long slowestTime =10;
/////////////////////////////////////

bool isBrailleSpacing= false;
int count;
int oldCount;
int trigCount;
int timeoutCount = 4;


//Modified by Paarangat Jain
bool holdArray[3];
//Modified ends here (All instances of hold,hold2,and hold3 have been replaced successfully.)
bool hold;
bool hold2;
bool hold3;


bool motorHold;
long motorStart;

int offsets[3]={-6,2,10}; //  group spacings are 8.33 mmm apart - offsets are 0.1mm.



// const byte encoderA = 4;
const byte encoderB = 4;
const int enablePin = 6;

//Modified by Paarangat Jain
int outs[3][8] = { {23, 27, 29, 33,43,47,49,53}, {24,25,34,31,40,45,50, 51},{22,26,30,36,38,44,46, 52}};
//Modified ends here (All instances of outOne,outTwo, and outThree have been replaced successfully.)
int outOne[] = {23, 27, 29, 33,43,47,49,53};//16 4 6  11
int outTwo[] ={24,25,34,31,40,45,50, 51}; //2 3 9 10
int outThree[] ={22,26,30,36,38,44,46, 52};//17 5 8 13


//Modified by Paarangat Jain
int arraySizes[3];
//Modified ends here (All instances of arrayBlahSize have been replaced successfully.)
int arrayOneSize;
int arrayTwoSize;
int arrayThreeSize;


bool firstRead;
bool secondRead;

bool up;
bool pass;
int currentVal;
int currentVal2;
int passCount;


//Modified by Paarangat Jain
bool startingPoint[3];
//Modified by Paarangat Jain ENDS HERE. All calls to startingPoint1,startingPoint2, and startingPoint3 were replaced successfully.
bool startingPoint1;
bool startingPoint2;
bool startingPoint3;


bool evenPass;
bool hitUpper;
bool hitLower;
int modulo3 = 25;
int speedOffset;
float speedOffsetScale;
int alternateOffset =0;


//Modified by Paarangat Jain
int spacingOffsets[3];


char theArray[96][24]; 
//Modified by Paarangat Jain
char pinVals[3][8][100];
//Modified by Paarangat Jain ENDS HERE. All calls to pinVals1,pinVals2, and pinVals3 were replaced successfully.
char pinVals1[8][100];
char pinVals2[8][100];
char pinVals3[8][100];

char charOne = '1';
int rows = 24;
int columns =96;

//Modified by Paarangat Jain
int columnArray[3];
//Modified by Paarangat Jain ENDS HERE. All calls to column1,column2, and column3 were replaced successfully.
int column1;
int column2;
int column3;


bool serialRead;
bool newLineRecieved;
int lineCountSerial;
bool rowSet;

int actuateCount[3];

int actuateCount1;
int actuateCount2;
int actuateCount3;

int distanceCutoff = 25;

void setup() {
  Serial.begin(115200);
  //delay (3000);
  Serial.println("running");
  arraySizes[0] = sizeof outs[0]/sizeof outs[0][0];
  arraySizes[1] = sizeof outs[1]/sizeof outs[1][0];
  arraySizes[2] = sizeof outs[2]/sizeof outs[2][0];

  pinMode(2, INPUT); // digital 2

  pinMode(encoderB, INPUT); // digital 3
  for (int i = 0; i< arraySizes[0]; i++)
  {
  pinMode(outs[0][i], OUTPUT);

  }
  for (int i = 0; i<arraySizes[1]; i++)
  {
  pinMode(outs[1][i], OUTPUT);

  }
  for (int i = 0; i<arraySizes[2]; i++)
  {
  pinMode(outs[2][i], OUTPUT);

  }
  pinMode(enablePin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(2), Interrupt, CHANGE );
  digitalWrite(enablePin, HIGH);
}

void loop() {


  // ReadLines();
  FillArray();

  if (oldCount != count)
  {
    long currentTime = micros();
    countTime = currentTime - oldCountTime;
    oldCountTime =currentTime;

    if (countTime < 1900 && count >oldCount) 
    {
      //slowestTime = countTime;
      //Serial.println("grab? " + count);
    // Serial.println(countTime);
    }
  
    
  
    CheckStartingPoint();


    oldCount = count;
    if (isBrailleSpacing)
    {
      spacingOffsets[0] = ((columnArray[0])/3) *-5; // these will be wrong. Todo abandon braille spacing. 
      spacingOffsets[1] = ((columnArray[1])/3) *-5;
      spacingOffsets[2] = ((columnArray[2])/3) *-5;
    }
    columnArray[0] = ((count -offsets[0] - spacingOffsets[0] - startingPoint[0]) /25) - 3;
    columnArray[1] = ((count -offsets[1] - spacingOffsets[1] - startingPoint[1]) /25) - 6;
    columnArray[2] = ((count - offsets[2] - spacingOffsets[2] - startingPoint[2]) /25) - 9;


    for (int i = 0; i < 3; i++){
      if ((count - offsets[i] ) %25 ==0   && startingPoint[i] && !holdArray[i]) //  && startOffset1 //spacingOffsets[0]
      {
        if (up)
        {  
          startMillisArray[i] = millis();
          actuateCount[i] = count; 
          // if (columnArray[0] < 89)
          //  {
          Actuate(i);       
          //  }   
        }
      }
    }
  }

  for (int i = 0; i < 3; i++){
    if (holdArray[i])
    {
      if (millis() - startMillisArray[i]> 15)//(10 + speedOffset ) )
      {
      
        for (int j = 0;  j < arraySizes[i]; j++)
        {
          digitalWrite(outs[i][j], LOW);
        }
      
        holdArray[i] = false;
      }
    }
  }
}
void CheckStartingPoint()
{
   if (count >85 )
    {
       startingPoint[0] = true;
    }
    else
    {
      startingPoint[0] = false;
      columnArray[0] = 0;
      spacingOffsets[0] = 0;
    }

    if (count >145 ) //145
    {
       startingPoint[1] = true;
    }
    else
    {
      startingPoint[1] = false;
      columnArray[1] = 0;
      spacingOffsets[1] = 0;
    }
     if (count >195) //206ish
    {
       startingPoint[2] = true;
    }
    else
    {
      startingPoint[2] = false;
      columnArray[2] = 0;
      spacingOffsets[2] = 0;
    }
}


void Actuate (int pinGroup) // toDo: put pin arrays in an array. If isBrailleSpacing, do the offset stuff.  
// This task has been done: Code untested on Arduino Mega Board.
{           
  for (int i = 0; i< arraySizes[pinGroup]; i++)
  {     
    
    if (pinVals[pinGroup][i][columnArray[pinGroup]] == '0')
    {  
      digitalWrite (outs[pinGroup][i], HIGH);
      holdArray[pinGroup] = true;
    }      

  }        
}

void FillArray()
{
  char endMarker = '\n';
  char rc;
  int row;
  int column;
  int pinGroupIndex;

    while (Serial.available() > 0 && !serialRead) {
          rc = Serial.read();
          if (rc == 'b')
          {
            isBrailleSpacing = true;      
            
            Serial.println("braille");
          }
        else if (rc == 't')
          {
            isBrailleSpacing = false;     
              Serial.println("tactile"); 
          
          }
        
      else{
          if (rc != endMarker) { 
            if (!rowSet)
            {
              //count =0;  // this stops drift, but requires fully  set back.
              row = 3; 
              column = 0; 
              pinGroupIndex = 0;
              rowSet = true;
            
            }
            
            if (row ==3)
            {
              pinVals[2][pinGroupIndex][column] = rc;
            

              
            }
              else if (row ==2)
              {
                  pinVals[1][pinGroupIndex][column] = rc;
                
              
              }
              else if (row ==1)
              {
                  pinVals[0][pinGroupIndex][column] = rc;
              
              }
        
              if (column < columns-1)
              {
              column++;
              }
              else
              {
                column = 0;
                if (row >1)
                {
                row --;
                }
                else
                {
                  row =3;
                  pinGroupIndex ++;
              
                }
                
              }
        
          }

          else
          {
            //Serial.println("end");
            row = 3;
            column = 0;
            pinGroupIndex = 0;
            rowSet= false;
            count = 0;
            fastestTime=1000;
            slowestTime = 10;
          }
          
  }
  }

}
void Interrupt()
{
 
  if (bitRead(PINE, 4))// reading itself to determine rising or falling. This is rising edge
  {
    firstRead = bitRead(PING, 5); // read other encoder o
  }
  else // read again on falling edge. This filters out erroneous triggers if encoder is twitched back and forth while right on the edge of a trigger.
  {
    
    secondRead = bitRead(PING, 5);
    if (firstRead && !secondRead)
    {
     
      count--;
      up = false;
      if (count <0)
      {
        count = 0;
      }
     
    }
    else if (!firstRead && secondRead)
    {
     startMicros = micros();
     
     pulsePeriod= startMicros - endMicros;
    
     endMicros = micros();
      count ++;
     up =true;
    }
  }  
}
