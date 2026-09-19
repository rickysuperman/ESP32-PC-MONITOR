#include <Wire.h>
#include <U8g2lib.h>

// 你的 OLED：SH1106 128x64 I2C
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(
  U8G2_R0,
  U8X8_PIN_NONE
);

float cpuTemp = 0;
float cpuLoad = 0;
float gpuTemp = 0;
float gpuLoad = 0;


// 畫進度條
void drawBar(int x, int y, int width, int height, float percent) {

  // 限制 0 ~ 100%
  if (percent < 0) percent = 0;
  if (percent > 100) percent = 100;

  // 外框
  u8g2.drawFrame(x, y, width, height);

  // 計算填滿多少
  int fillWidth = (int)((width - 2) * percent / 100.0);

  if (fillWidth > 0) {
    u8g2.drawBox(x + 1, y + 1, fillWidth, height - 2);
  }
}


// 更新 OLED
void drawScreen() {

  u8g2.clearBuffer();

  // ===== 標題 =====
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(31, 10, "PC MONITOR");

  // ===== CPU =====
  u8g2.setCursor(0, 24);
  u8g2.print("CPU ");
  u8g2.print((int)cpuTemp);
  u8g2.print("C  ");
  u8g2.print((int)cpuLoad);
  u8g2.print("%");

  // CPU 進度條
  drawBar(0, 27, 128, 8, cpuLoad);

  // ===== GPU =====
  u8g2.setCursor(0, 47);
  u8g2.print("GPU ");
  u8g2.print((int)gpuTemp);
  u8g2.print("C  ");
  u8g2.print((int)gpuLoad);
  u8g2.print("%");

  // GPU 進度條
  drawBar(0, 50, 128, 8, gpuLoad);

  u8g2.sendBuffer();
}


void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);

  u8g2.begin();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tf);
  u8g2.drawStr(30, 25, "PC MONITOR");
  u8g2.drawStr(40, 42, "READY!");
  u8g2.sendBuffer();
}


void loop() {

  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');
    data.trim();

    // 找四個資料的位置
    int p1 = data.indexOf("CPU_TEMP:");
    int p2 = data.indexOf("CPU_LOAD:");
    int p3 = data.indexOf("GPU_TEMP:");
    int p4 = data.indexOf("GPU_LOAD:");

    if (p1 >= 0 && p2 >= 0 && p3 >= 0 && p4 >= 0) {

      cpuTemp = data.substring(
        p1 + 9,
        data.indexOf(",", p1)
      ).toFloat();

      cpuLoad = data.substring(
        p2 + 9,
        data.indexOf(",", p2)
      ).toFloat();

      gpuTemp = data.substring(
        p3 + 9,
        data.indexOf(",", p3)
      ).toFloat();

      gpuLoad = data.substring(
        p4 + 9
      ).toFloat();

      drawScreen();
    }
  }
}
