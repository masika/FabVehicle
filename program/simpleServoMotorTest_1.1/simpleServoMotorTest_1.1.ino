#include <Servo.h>

Servo myservo;

// ピン番号
const int SERVO_PIN = 10;
const int DC_PIN1 = 7;
const int DC_PIN2 = 8;
const int PM_PIN = 6;//パラメータピン？アナログ


// モーター用パラメータ変数
int dcMotorSpeed;        // DCモータ駆動用のパラメータ
int servoMotorAngle = 90;      // サーボの初期値;        // サーボーモータのパラメータ(角度)



// モータ用パラメータ定数
const int EACH_SERVO_MOTOR_CHANGE=  12;     // 操作一回当りのサーボモータパラメータ変化量
const int MIN_DC_MOTOR_RANGE = -100;   // DCモータパラメータの最小値
const int MAX_DC_MOTOR_RANGE = 150;    // DCモータパラメータの最大値
const int MIN_SERVO_MOTOR_RANGE =  78;    // サーボモータパラメータの最小値
const int MAX_SERVO_MOTOR_RANGE = 102;    // サーボモータパラメータの最大値

int changeCycle = 500;//ループ初期値ms;//ループ周期

int direction=0; //-1:左　0:直進 　1:右
int direction_arr[]={0,1,1,1,1,0,-1,-1,-1,-1};
int direction_time=0;

//---------------------------------------------------
// セットアップ関数
//---------------------------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(DC_PIN1 ,OUTPUT);
  pinMode(DC_PIN2 ,OUTPUT);
  myservo.attach(SERVO_PIN);
  dcMotorSpeed = MAX_DC_MOTOR_RANGE;    // DCモータの初期値
}

//---------------------------------------------------
// DCモータ駆動関数 止まる/前進/後退
//---------------------------------------------------
void MotorDrive( int iIn1Pin, int iIn2Pin, int dcMotorSpeed )
{
  if( dcMotorSpeed == 0 ) { // 
    digitalWrite(iIn1Pin, LOW);
    digitalWrite(iIn2Pin, LOW);
  } 
  else if( 0 < dcMotorSpeed ) {
    digitalWrite(iIn1Pin, LOW);
    digitalWrite(iIn2Pin, HIGH);
    analogWrite(PM_PIN,dcMotorSpeed);
  } 
  else {
    digitalWrite(iIn1Pin, LOW);
    digitalWrite(iIn2Pin, HIGH);
    analogWrite(PM_PIN,-dcMotorSpeed);
  }
}

//---------------------------------------------------
// クリッピング関数(ユーティリティ)
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
runEight();

//turn(-1);//左旋回

delay ( changeCycle );
}


//--------------------------------------------------
//	8の字走行(ただし8の字を描くはモーターの回転スピードに依存)
//--------------------------------------------------



//8の字
void runEight(){
	dcMotorSpeed = Clip(MIN_DC_MOTOR_RANGE,MAX_DC_MOTOR_RANGE,dcMotorSpeed);
	MotorDrive(DC_PIN1,DC_PIN2,dcMotorSpeed);
    
	direction=direction_arr[direction_time];
	servoMotorAngle=Clip(MIN_SERVO_MOTOR_RANGE,MAX_SERVO_MOTOR_RANGE,90+(direction*EACH_SERVO_MOTOR_CHANGE));  
	myservo.write(servoMotorAngle);
  
  if(sizeof direction_arr<10){
  	  direction_time++;
  }else{
    direction_time=0;
  }

  
}

//旋回
void turn(int dir){
	dcMotorSpeed = Clip(MIN_DC_MOTOR_RANGE,MAX_DC_MOTOR_RANGE,dcMotorSpeed);
	MotorDrive(DC_PIN1,DC_PIN2,dcMotorSpeed);

	direction=dir;
	servoMotorAngle=Clip(MIN_SERVO_MOTOR_RANGE,MAX_SERVO_MOTOR_RANGE,90+(direction*EACH_SERVO_MOTOR_CHANGE));  
	myservo.write(servoMotorAngle);
}

