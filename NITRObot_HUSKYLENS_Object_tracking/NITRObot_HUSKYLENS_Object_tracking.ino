/***************************************************
 HUSKYLENS An Easy-to-use AI Machine Vision Sensor
 <https://www.dfrobot.com/product-1922.html>
 
 ***************************************************
 This example shows how to play with object tracking.
 
 Created 2020-03-13
 By [Angelo qiao](Angelo.qiao@dfrobot.com)
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here
 <https://wiki.dfrobot.com/HUSKYLENS_V1.0_SKU_SEN0305_SEN0336#target_23>
 2.This code is tested on Arduino Uno, Leonardo, Mega boards.
 ****************************************************/


#include "HUSKYLENS.h"

#define LEFT_FOR 9 // PWMB
#define LEFT_BACK 5  // DIRB  ---  left
#define RIGHT_FOR 6  // PWMA
#define RIGHT_BACK 10  // DIRA  ---  right
HUSKYLENS huskylens;
//HUSKYLENS green line >> SDA; blue line >> SCL

void setup() {
    Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING); //Switch the algorithm to object tracking.
 delay(2000);
}

int widthLevel = 60;  // 50

int xLeft = 160-40;
int xRight = 160+40;

const int SPEED = 100; 
const int SPEED_OFFSET = 100; // 50 default


bool isTurning = false;
bool isTurningLeft = true;

bool isInside(int value, int min, int max){
    return (value >= min && value <= max);
}

void printResult(HUSKYLENSResult result);

void loop() {
    int32_t error;

    int left = 0, right = 0;

    if (!huskylens.request()) Serial.println(F("Fail to request objects from HUSKYLENS!"));
    else if(!huskylens.isLearned()) {Serial.println(F("Object not learned!")); }
    else if(!huskylens.available()) Serial.println(F("Object disappeared!"));
    else
    {
        HUSKYLENSResult result = huskylens.read();

       if (isInside(result.xCenter, 30, xLeft)){  
             analogWrite(LEFT_FOR,  SPEED-20);
             analogWrite(LEFT_BACK, LOW);
             analogWrite(RIGHT_FOR,  SPEED+SPEED_OFFSET);
             analogWrite(RIGHT_BACK, LOW);  
           }
       else if (isInside(result.xCenter, xLeft, xRight)){ 
             analogWrite(LEFT_FOR,  SPEED);
             analogWrite(LEFT_BACK, LOW);
             analogWrite(RIGHT_FOR,  SPEED);
             analogWrite(RIGHT_BACK, LOW);           
           } 
       else if (isInside(result.xCenter, xRight, 290)){
             analogWrite(LEFT_FOR,  SPEED+SPEED_OFFSET);
             analogWrite(LEFT_BACK, LOW);
             analogWrite(RIGHT_FOR,  SPEED-20);
             analogWrite(RIGHT_BACK, LOW);
           } 

        else
        {
             analogWrite(LEFT_FOR,  HIGH);
             analogWrite(LEFT_BACK, HIGH);
             analogWrite(RIGHT_FOR,  HIGH);
             analogWrite(RIGHT_BACK, HIGH);
        }
        printResult(result);
    }

}
void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}
