#include <Servo.h>

#define PIN_IR    A0        // 적외선 센서 핀 A0에 연결
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 550  // 서보 최대 시계방향 위치 (0도)
#define _DUTY_NEU 1650  // 서보 중립 위치 (90도)
#define _DUTY_MAX 2640  // 서보 최대 반시계방향 위치 (180도)

#define _DIST_MIN  100.0   // 최소 거리 100mm
#define _DIST_MAX  250.0   // 최대 거리 250mm

#define EMA_ALPHA  0.2     // EMA 필터용 알파값

#define LOOP_INTERVAL 20   // 루프 간격 (단위: 밀리초)

Servo myservo;
unsigned long last_loop_time;   // 단위: 밀리초

float dist_prev = _DIST_MIN;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);    // 1,000,000 bps
}

void loop()
{
  unsigned long time_curr = millis();
  float a_value, dist_raw;

  // 다음 이벤트 시간까지 대기
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);
  dist_raw = ((6762.0 / (a_value - 9.0)) - 4.0) * 10.0 - 60.0;

  // 범위 필터링 코드 추가 (_DIST_MIN ~ _DIST_MAX)
  // 거리가 범위 내에 있으면 LED 켜기
  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
    dist_raw = dist_prev;           // 최대값보다 크면 이전 값으로 설정
    digitalWrite(PIN_LED, 1);       // LED 끄기
  } else if (dist_raw < _DIST_MIN) {
    dist_raw = dist_prev;           // 최솟값보다 작으면 이전 값으로 설정
    digitalWrite(PIN_LED, 1);       // LED 끄기
  } else {    // 원하는 범위 내에 있음
    dist_prev = dist_raw;
    digitalWrite(PIN_LED, 0);       // LED 켜기      
  }                                                 

  dist_ema = EMA_ALPHA * dist_raw + (1 - EMA_ALPHA) * dist_ema; // EMA 필터

  int duty = ((_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN)) * (dist_ema - _DIST_MIN) + _DUTY_MIN;
  
  myservo.writeMicroseconds(duty);

  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print("_DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
