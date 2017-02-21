#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>
#include <SPI.h>  
#include <Pixy.h>
 
Pixy pixy;

#define pwm 5
#define dir 4
#define dir2 7
#define pwm2 6
#define LED 12

int signature = 0;
int x = 0;            //positon x axis
int y = 0;            //position y axis
int width = 0;        //object's width
int height = 0;    
int Area = 0;

//###############################################################//

Fuzzy* fuzzy = new Fuzzy();


FuzzySet* small = new FuzzySet(0,0, 2000, 3000);
FuzzySet* medium = new FuzzySet(2000, 3000, 3000, 4000);
FuzzySet* big = new FuzzySet(3000,4000,6000,6000);


FuzzySet* left = new FuzzySet(0,0,0,120);
FuzzySet* center = new FuzzySet(0,130,190,320);
FuzzySet* right = new FuzzySet(190,320,320,320);

FuzzySet* right_stopped = new FuzzySet(0, 0, 0, 0);
FuzzySet* right_slow = new FuzzySet(0, 0, 0, 60);
FuzzySet* right_normal = new FuzzySet(0,0, 90, 180);
FuzzySet* right_fast = new FuzzySet(120, 180,180,180);

FuzzySet* left_stopped = new FuzzySet(0, 0, 0, 0);
FuzzySet* left_slow = new FuzzySet(0, 0, 0, 60);
FuzzySet* left_normal = new FuzzySet(0,0, 90, 180);
FuzzySet* left_fast = new FuzzySet(120, 180,180,180);


void setup(){

  Serial.begin(9600);
  pixy.init();
  
  pinMode(dir, OUTPUT);
  pinMode(pwm, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(LED, OUTPUT);
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%//

FuzzyInput* x_value = new FuzzyInput(1);
  x_value->addFuzzySet(left);
  x_value->addFuzzySet(center);
  x_value->addFuzzySet(right);

  fuzzy->addFuzzyInput(x_value);


  // FuzzyInput
  FuzzyInput* area = new FuzzyInput(2);
  area->addFuzzySet(small);
  area->addFuzzySet(medium);
  area->addFuzzySet(big);

  fuzzy->addFuzzyInput(area);

  // FuzzyOutput
   
  FuzzyOutput* RightMotorSpeed = new FuzzyOutput(1);
  RightMotorSpeed->addFuzzySet(right_stopped);
  RightMotorSpeed->addFuzzySet(right_slow);
  RightMotorSpeed->addFuzzySet(right_normal);
  RightMotorSpeed->addFuzzySet(right_fast);

  fuzzy->addFuzzyOutput(RightMotorSpeed); 
  
  
  FuzzyOutput* LeftMotorSpeed = new FuzzyOutput(2);
  LeftMotorSpeed->addFuzzySet(left_stopped);
  LeftMotorSpeed->addFuzzySet(left_slow);
  LeftMotorSpeed->addFuzzySet(left_normal);
  LeftMotorSpeed->addFuzzySet(left_fast);

  fuzzy->addFuzzyOutput(LeftMotorSpeed); 
  
  // Building FuzzyRule1
  FuzzyRuleAntecedent* ifx_valueleft = new FuzzyRuleAntecedent();
  ifx_valueleft->joinSingle(left);
  
  FuzzyRuleConsequent* thenRightMotorSpeedfastAndLeftMotorSpeedslow = new FuzzyRuleConsequent();
  thenRightMotorSpeedfastAndLeftMotorSpeedslow->addOutput(right_fast);
  thenRightMotorSpeedfastAndLeftMotorSpeedslow->addOutput(left_slow);

  FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifx_valueleft, thenRightMotorSpeedfastAndLeftMotorSpeedslow);
  fuzzy->addFuzzyRule(fuzzyRule1);
/*

  // Building FuzzyRule2
   FuzzyRuleAntecedent* ifx_valuecenter = new FuzzyRuleAntecedent();
  ifx_valuecenter->joinSingle(center);
  
  FuzzyRuleConsequent* thenRightMotorSpeedNormalAndLeftMotorSpeedNormal = new FuzzyRuleConsequent();
  thenRightMotorSpeedNormalAndLeftMotorSpeedNormal->addOutput(right_normal);
  thenRightMotorSpeedNormalAndLeftMotorSpeedNormal->addOutput(left_normal);

  FuzzyRule* fuzzyRule2 = new FuzzyRule(2, ifx_valuecenter, thenRightMotorSpeedNormalAndLeftMotorSpeedNormal);
  fuzzy->addFuzzyRule(fuzzyRule2);
  
 */
 
  // Building FuzzyRule3
  FuzzyRuleAntecedent* ifx_valueRight = new FuzzyRuleAntecedent();
  ifx_valueRight->joinSingle(right);
  
  FuzzyRuleConsequent* thenRightMotorSpeedslowAndLeftMotorSpeedfast = new FuzzyRuleConsequent();
  thenRightMotorSpeedslowAndLeftMotorSpeedfast->addOutput(right_slow);
  thenRightMotorSpeedslowAndLeftMotorSpeedfast->addOutput(left_fast);

  FuzzyRule* fuzzyRule3 = new FuzzyRule(3, ifx_valueRight, thenRightMotorSpeedslowAndLeftMotorSpeedfast);
  fuzzy->addFuzzyRule(fuzzyRule3);
  

  //Building FuzzyRule4
  FuzzyRuleAntecedent* ifx_valuecenterAndareasmall = new FuzzyRuleAntecedent();
  ifx_valuecenterAndareasmall->joinWithAND(center,small);
  
  FuzzyRuleConsequent* thenRightMotorSpeedsFastAndLeftMotorSpeedFast = new FuzzyRuleConsequent();
  thenRightMotorSpeedsFastAndLeftMotorSpeedFast->addOutput(right_fast);
  thenRightMotorSpeedsFastAndLeftMotorSpeedFast->addOutput(left_fast);

  FuzzyRule* fuzzyRule4 = new FuzzyRule(4, ifx_valuecenterAndareasmall, thenRightMotorSpeedsFastAndLeftMotorSpeedFast);
  fuzzy->addFuzzyRule(fuzzyRule4);

  
  //Building FuzzyRule5
  FuzzyRuleAntecedent* ifx_valuecenterAndareamedium = new FuzzyRuleAntecedent();
  ifx_valuecenterAndareamedium ->joinWithAND(center,medium);
  
  FuzzyRuleConsequent* thenRightMotorSpeedstoppedAndLeftMotorSpeedstopped = new FuzzyRuleConsequent();
  thenRightMotorSpeedstoppedAndLeftMotorSpeedstopped->addOutput(right_stopped);
  thenRightMotorSpeedstoppedAndLeftMotorSpeedstopped->addOutput(left_stopped);

  FuzzyRule* fuzzyRule5 = new FuzzyRule(5, ifx_valuecenterAndareamedium,thenRightMotorSpeedstoppedAndLeftMotorSpeedstopped);
  fuzzy->addFuzzyRule(fuzzyRule5);
  
  //Building FuzzyRule6
  FuzzyRuleAntecedent* ifx_valuecenterAndareabig = new FuzzyRuleAntecedent();
  ifx_valuecenterAndareabig ->joinWithAND(center,big);
  
  FuzzyRuleConsequent* thenRightMotorSpeedStoppedAndLeftMotorSpeedStopped = new FuzzyRuleConsequent();
  thenRightMotorSpeedStoppedAndLeftMotorSpeedStopped->addOutput(right_stopped);
  thenRightMotorSpeedStoppedAndLeftMotorSpeedStopped->addOutput(left_stopped);

  FuzzyRule* fuzzyRule6 = new FuzzyRule(6,ifx_valuecenterAndareabig,thenRightMotorSpeedStoppedAndLeftMotorSpeedStopped);
  fuzzy->addFuzzyRule(fuzzyRule6);
  
}


void loop(){
  
  static int i = 0;
 
  uint16_t blocks;
  
  blocks = pixy.getBlocks();  //receive data from pixy
   
  signature = pixy.blocks[i].signature;    //get object's signature
  x = pixy.blocks[i].x;                    //get x position
  y = pixy.blocks[i].y;                    //get y position
  width = pixy.blocks[i].width;            //get width
  height = pixy.blocks[i].height;          //get height
 
  
  if(signature == 1){
 
  Area = width * height;    //calculate the object area
  
  
  fuzzy->setInput(1, x);
  fuzzy->setInput(2, Area);
  
  fuzzy->fuzzify();
  
  Serial.print("x =  ");
  Serial.println(x);
  
  Serial.print("x_values: ");
  Serial.print(left->getPertinence());
  Serial.print(", ");
  Serial.print(center->getPertinence());
  Serial.print(", ");
  Serial.println(right->getPertinence());
  
  Serial.print("Area = ");
  Serial.println(Area);
  Serial.print("Area Values: ");
  Serial.print(small->getPertinence());
  Serial.print(", ");
  Serial.print(medium->getPertinence());
  Serial.print(", ");
  Serial.println(big->getPertinence());

  

  float output1 = fuzzy->defuzzify(1);
  float output2 = fuzzy->defuzzify(2);
  
  Serial.print("RightMotorSpeed: ");
  Serial.print(output1);
  Serial.print(", LeftMotorSpeed: ");
  Serial.println(output2);
  Serial.println("################################################ ");

  delay(2000);
  }
 }
 
