#include <Arduino.h>
#include <U8g2lib.h>

#define BUTTON_UP 12
#define BUTTON_DOWN 14

String menuItems[] = {
  "Start",
  "Settings",
  "About",
  "Exit"
};

static constexpr int MENU_COUNT = sizeof(menuItems) / sizeof(menuItems[0]);

int menuIndex = 0;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,     // 屏幕旋转方向
  /* clock=*/ 22,   // SCL
  /* data=*/  21,   // SDA
  /* reset=*/ U8X8_PIN_NONE
);

void displayMenu(int selected){
  u8g2.clearBuffer();
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_6x12_tr);
    u8g2.drawStr(0,10,"Menu:");
    u8g2.drawHLine(0, 12, 128);
    for(int i = 0; i < MENU_COUNT; i++){
      int y = 25 + i * 12;
      if(i == selected){
        u8g2.drawStr(0, y, ">");
        u8g2.drawStr(15, y, menuItems[i].c_str());
      }else{
        u8g2.drawStr(0, y, menuItems[i].c_str());
      }
    }
  } while (u8g2.nextPage());
}

// --- 按键：非阻塞消抖 + 连发 ---
struct Button {
  uint8_t pin;

  bool raw = true;               // INPUT_PULLUP: true=未按，false=按下
  bool debounced = true;

  uint32_t lastRawChangeMs = 0;
  uint32_t pressedSinceMs = 0;
  uint32_t lastRepeatMs = 0;

  // 关键：提供构造函数，允许用 Button{PIN} 初始化
  explicit Button(uint8_t p) : pin(p) {}
  Button() = default;
};

static constexpr uint32_t DEBOUNCE_MS     = 18;   // 15~25 常用
static constexpr uint32_t REPEAT_DELAY_MS = 280;  // 按住多久开始连发
static constexpr uint32_t REPEAT_RATE_MS  = 85;   // 连发间隔（越小越快）

Button btnUp   (static_cast<uint8_t>(BUTTON_UP));
Button btnDown (static_cast<uint8_t>(BUTTON_DOWN));

static int updateButton(Button &b, int deltaWhenPressed){
  const uint32_t now = millis();
  const bool newRaw = (digitalRead(b.pin) == HIGH); // true=未按，false=按下

  if(newRaw != b.raw){
    b.raw = newRaw;
    b.lastRawChangeMs = now;
  }

  // 还在消抖时间内，不产生事件
  if(now - b.lastRawChangeMs < DEBOUNCE_MS){
    return 0;
  }

  // 稳定后更新 debounced 状态
  if(b.debounced != b.raw){
    b.debounced = b.raw;

    // 发生“按下”边沿（INPUT_PULLUP：按下为 false）
    if(b.debounced == false){
      b.pressedSinceMs = now;
      b.lastRepeatMs = now;
      return deltaWhenPressed; // 单击立刻响应
    }
    // 松开不处理
    return 0;
  }

  // 按住连发
  if(b.debounced == false){
    if(now - b.pressedSinceMs >= REPEAT_DELAY_MS &&
       now - b.lastRepeatMs   >= REPEAT_RATE_MS){
      b.lastRepeatMs = now;
      return deltaWhenPressed;
    }
  }

  return 0;
}

static void clampMenuIndex(){
  if(menuIndex < 0) menuIndex = MENU_COUNT - 1;
  if(menuIndex >= MENU_COUNT) menuIndex = 0;
}

void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);

  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);

  // 初始显示一次
  displayMenu(menuIndex);
}

void loop(){
  int delta = 0;
  delta += updateButton(btnUp,   -1);
  delta += updateButton(btnDown, +1);

  if(delta != 0){
    menuIndex += delta;
    clampMenuIndex();
    displayMenu(menuIndex);
  }

  // 不要 delay()，保持扫描频率；如需让出一点 CPU，可用：
  // delay(1);
}