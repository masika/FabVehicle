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


// モーター用パラメータ変数
volatile int iMotor;        // DCモータ駆動用のパラメータ
volatile int iServo;        // サーボーモータのパラメータ(角度)

//
int diS;
int diM;

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
// クリッピング関数
//---------------------------------------------------
int Clip( int vmin, int vmax, int value )
{
  int vclip = value;

  if ( vmin > vclip ) {
    vclip = vmin;
  }
  else if ( vmax < vclip ) {
    vclip = vmax;
  }

  return vclip;
}

//---------------------------------------------------
// メインループ関数
//---------------------------------------------------
void loop()
{  

  iMotor = Clip(minM,maxM,iMotor);
  MotorDrive(dcPin1,dcPin2,iMotor);
    
  iServo = Clip(minS,maxS,iServo);
  myservo.write(iServo);
  
  iServo += (incS * diS);
  if ( maxS < iServo ) {
    iServo -= incS;
    diS *= -1;
  } else if ( minS > iServo ) {
    iServo += incS;
    diS *= -1;
  }

  delay(2000);
  
}

