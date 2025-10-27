// Nama = Ronaldo Hafizh Aji Dzakwan
// NIM  = 25/568034/TK/64076

/*
  Program ini mengontrol 5 servo menggunakan ESP32
  Berdasarkan MPU6050 (roll, pitch, yaw) dan PIR
  - Servo 1 & 2: berlawanan arah roll
  - Servo 3 & 4: searah pitch
  - Servo 5: mengikuti yaw, jeda 1 detik, kembali posisi awal
  - PIR aktif: semua servo ke preset lalu kembali posisi awal
*/

#include <Wire.h>           // I2C
#include <ESP32Servo.h>     // Servo untuk ESP32
#include <MPU6050_tockn.h>  // Sensor MPU6050

// Objek sensor dan servo
MPU6050 mpu6050(Wire);
Servo s1, s2, s3, s4, s5;

// Pin koneksi
#define PIR_PIN     13
#define SERVO1_PIN  12
#define SERVO2_PIN  14
#define SERVO3_PIN  27
#define SERVO4_PIN  26
#define SERVO5_PIN  25

// Posisi awal dan preset
int posisiAwal = 90;
int pos1 = 120, pos2 = 60, pos3 = 100, pos4 = 80, pos5 = 150;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Inisialisasi MPU6050
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Attach servo
  s1.attach(SERVO1_PIN);
  s2.attach(SERVO2_PIN);
  s3.attach(SERVO3_PIN);
  s4.attach(SERVO4_PIN);
  s5.attach(SERVO5_PIN);

  pinMode(PIR_PIN, INPUT);

  resetServo();
  Serial.println("Sistem aktif. ESP32 siap membaca sensor dan mengontrol servo.");
}

void loop() {
  mpu6050.update();
  float roll  = mpu6050.getAngleX();
  float pitch = mpu6050.getAngleY();
  float yaw   = mpu6050.getAngleZ();

  Serial.print("Roll: "); Serial.print(roll);
  Serial.print(" Pitch: "); Serial.print(pitch);
  Serial.print(" Yaw: "); Serial.println(yaw);

  if (digitalRead(PIR_PIN)) { // PIR aktif
    Serial.println("PIR mendeteksi gerakan!");
    gerakKhusus();
    delay(1000);
    resetServo();
  } else { // Kontrol berdasarkan sensor
    kontrolRoll(roll);
    kontrolPitch(pitch);
    kontrolYaw(yaw);
  }

  delay(100);
}

// Fungsi kontrol Roll
void kontrolRoll(float roll) {
  if (roll > 5) {
    s1.write(posisiAwal - 20);
    s2.write(posisiAwal + 20);
    Serial.println("Roll positif → Servo 1 & 2 berlawanan arah.");
  } else if (roll < -5) {
    s1.write(posisiAwal + 20);
    s2.write(posisiAwal - 20);
    Serial.println("Roll negatif → Servo 1 & 2 berlawanan arah.");
  } else {
    s1.write(posisiAwal);
    s2.write(posisiAwal);
  }
}

// Fungsi kontrol Pitch
void kontrolPitch(float pitch) {
  if (pitch > 5) {
    s3.write(posisiAwal + 20);
    s4.write(posisiAwal + 20);
    Serial.println("Pitch positif → Servo 3 & 4 searah rotasi.");
  } else if (pitch < -5) {
    s3.write(posisiAwal - 20);
    s4.write(posisiAwal - 20);
    Serial.println("Pitch negatif → Servo 3 & 4 searah rotasi.");
  } else {
    s3.write(posisiAwal);
    s4.write(posisiAwal);
  }
}

// Fungsi kontrol Yaw
void kontrolYaw(float yaw) {
  if (abs(yaw) > 5) {
    int yawMapped = map((int)yaw, -180, 180, -30, 30);
    s5.write(posisiAwal + yawMapped);
    Serial.println("Yaw aktif → Servo 5 mengikuti rotasi.");
    delay(1000);
    s5.write(posisiAwal);
  } else {
    s5.write(posisiAwal);
  }
}

// Reset semua servo ke posisi awal
void resetServo() {
  s1.write(posisiAwal);
  s2.write(posisiAwal);
  s3.write(posisiAwal);
  s4.write(posisiAwal);
  s5.write(posisiAwal);
  Serial.println("Semua servo kembali ke posisi awal.");
}

// Gerakan khusus PIR
void gerakKhusus() {
  s1.write(pos1);
  s2.write(pos2);
  s3.write(pos3);
  s4.write(pos4);
  s5.write(pos5);
  Serial.println("Gerakan khusus PIR dijalankan.");
}