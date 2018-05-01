# ESP_2018 TeamProject

## Requirement

- Hardware
	- Raspberry pi 3
	- RPI 8MP CAMERA V2 (http://www.devicemart.co.kr/1077951/support/#support)
	- Servo Motor
	- L293D
	- Sonar senser
- Software
	- Python 3.3+
	- OpenCV 3.2
	- WiringPi-Python 


## 프로젝트명

- 얼굴인식 방문자 확인 시스템 (미정)

## 프로젝트 설명

- 방문자가 접근을 하게되면 초음파 센서로 방문자를 파악하고 얼굴인식 영상처리를 통해 저장되어있는 방문자인지 확인한다. 저장되어있는 방문자면 사진과 이름을 저장하고 문을 자동으로 열어준다. 저장되어 있지 않은 방문자라면 방문자의 사진을 저장하고 이메일 또는 안드로이드 푸시 메세지로 미확인 방문 사실을 전송한다.

## 프로젝트 설계

- 추가 예정

## 사용 기술

- 방문자 확인을 위한 Face Recognition 영상처리
- 초음파 센서 기반의 거리측정
- L293D와 서보모터 제어

## 참고자료

### python face_recognition opensource

- https://github.com/ageitgey/face_recognition

### WiringPi-Python install

- https://github.com/WiringPi/WiringPi-Python

### openCV 3.2 install

- http://pklab.net/?id=392&lang=EN

### icon

![python](images/python.png)
![raspberrypi](images/raspberrypi.png)
![opencv](images/opencv.png)