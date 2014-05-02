#include <Servo.h>

Servo myservo;

#define KEY_SPACE  (32)
#define KEY_S     (115)
#define KEY_R     (114)
#define KEY_L     (108)
#define KEY_U     (117)
#define KEY_D     (100)

//
#define sKEEP     (0)
#define sUP       (1)
#define sDOWN     (2)
#define sRIGHT    (3)
#define sLEFT     (4)
#define sSTRAIGHT (5)
#define sSTOP     (6)

// ピン番号
const int servoPin = 10;
const int dcPin1 = 7;
const int dcPin2 = 8;
const int pmPin = 6;
const int tsPin1 = 0;
const int tsPin2 = 1;

// モーター用パラメータ変数
int iMotor;        // DCモータ駆動用のパラメータ
int iServo;        // サーボーモータのパラメータ(角度)

//
int diS;
int diM;

//
volatile int touchF;

// モータ用パラメータ定数
const int incS =  12;     // 操作一回当りのサーボモータパラメータ変化量
const int minM = -100;   // DCモータパラメータの最小値
const int maxM = 100;    // DCモータパラメータの最大値
const int minS =  78;    // サーボモータパラメータの最小値
const int maxS = 102;    // サーボモータパラメータの最大値

//---------------------------------------------------
// セットアップ関数
//---------------------------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(dcPin1 ,OUTPUT);
  pinMode(dcPin2 ,OUTPUT);
  myservo.attach(servoPin);
  
  attachInterrupt(tsPin1, touch , RISING);
  attachInterrupt(tsPin2, touch , RISING);
  touchF = 0;
  
  diS    = 1;
  iMotor = 100;    // DCモータの初期値
  iServo = 90;   // サーボの初期値
}

//---------------------------------------------------
// DCモータ駆動関数
//---------------------------------------------------
void MotorDrive( int iIn1Pin, int iIn2Pin, int iMotor )
{
  if( iMotor == 0 ) { // 
    digitalWrite(iIn1Pin, LOW);
    digitalWrite(iIn2Pin, LOW);
  } 
  else if( 0 < iMotor ) {
    digitalWrite(iIn1Pin, LOW);
    digitalWrite(iIn2Pin, HIGH);
    analogWrite(pmPin,iMotor);
  } 
  else {
    digitalWrite(iIn1Pin, LOW);
    digitalWrite(iIn2Pin, HIGH);
    analogWrite(pmPin,-iMotor);
  }
}
//---------------------------------------------------
// タッチセンサーから信号を受けた時の動作関数
//---------------------------------------------------
void avoidingAction(int touchF)
{
  switch ( touchF ) {
    case 4:{
      iMotor = 0;
      iServo = 90;
      break;
    }
    case 3:
    case 2:{
      iMotor = -100;
      break;
    }
    case 1:{
     iServo = 80;
     break;
    }
    default:{
    }
  }
}

//---------------------------------------------------
// 割込み用のコールバック関数
//---------------------------------------------------
void touch ()
{
  if( touch == 0 ) touchF = 4;
}

//---------------------------------------------------
// メインループ関数
//---------------------------------------------------
void loop()
{  
  iMotor = constrain(iMotor,minM,maxM);
  MotorDrive(dcPin1,dcPin2,iMotor);
    
  iServo = constrain(iServo,minS,maxS);
  myservo.write(iServo);
  
  int intervalT = 2000;
  if ( touchF != 0 ) {  // タッチセンサーから信号を受けた時の動作(４回実行される)
    avoidingAction(touchF);
    intervalT = 500;
    touchF --;
  }
  else {                // 通常動作
    iServo += (incS * diS);
    if ( maxS < iServo ) {
      iServo -= incS;
      diS *= -1;
    } else if ( minS > iServo ) {
      iServo += incS;
      diS *= -1;
    }
  }
  
  delay(intervalT); 
}


