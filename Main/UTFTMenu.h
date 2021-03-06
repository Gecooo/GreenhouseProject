#ifndef _UTFTMENU_H
#define _UTFTMENU_H
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "Globals.h"

#ifdef USE_TFT_MODULE

#include "TinyVector.h"

#include <UTFT.h>
#include <URTouchCD.h>
#include <URTouch.h>
#include "UTFT_Buttons_Rus.h"
#include "UTFTRus.h"
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define TFT_BACK_COLOR 0xFF,0xFF,0xFF
#define TFT_BUTTON_COLORS VGA_WHITE, VGA_GRAY, VGA_WHITE, /*VGA_SILVER*/VGA_RED, VGA_BLUE
#define TFT_IDLE_SCREEN_BUTTON_WIDTH 128
#define TFT_IDLE_SCREEN_BUTTON_HEIGHT 90
#define TFT_IDLE_SCREEN_BUTTON_SPACING 10
#define TFT_FONT_COLOR 0x4B, 0x4C, 0x4B //0x4A69
#define TFT_CHANNELS_BUTTON_COLORS 0x3A8D, VGA_SILVER, VGA_GRAY, /*VGA_SILVER*/VGA_RED, 0xEF7D
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define INFO_BOX_WIDTH 240
#define INFO_BOX_HEIGHT 80
#define INFO_BOX_V_SPACING 20
#define INFO_BOX_CONTENT_PADDING 8
#define INFO_BOX_BACK_COLOR 0xF2, 0xF2, 0xF2//0xEF7D
#define INFO_BOX_BORDER_COLOR VGA_GRAY
#define INFO_BOX_CAPTION_COLOR 0x30, 0x7B, 0xB5//0x3A8D

#define SENSOR_BOX_WIDTH 240
#define SENSOR_BOX_HEIGHT 90
#define SENSOR_BOX_V_SPACING 20
#define SENSOR_BOX_BORDER_COLOR VGA_GRAY
#define SENSOR_BOX_FONT_COLOR VGA_TEAL
#define SENSOR_BOX_UNIT_COLOR 0x80, 0xB0, 0x51//0x7D2A
#define SENSOR_BOXES_PER_LINE 3

#define MODE_ON_COLOR VGA_GREEN
#define MODE_OFF_COLOR VGA_MAROON

#define CHANNELS_BUTTONS_TEXT_COLOR VGA_WHITE
#define CHANNELS_BUTTONS_BG_COLOR 0xEF7D
#define CHANNEL_BUTTONS_TEXT_COLOR 0x3A8D
#define CHANNELS_BUTTONS_PER_LINE 4
#define CHANNELS_BUTTON_WIDTH 165
#define CHANNELS_BUTTON_HEIGHT 50
#define ALL_CHANNELS_BUTTON_WIDTH 226
#define ALL_CHANNELS_BUTTON_HEIGHT 70

#define TFT_TEXT_INPUT_WIDTH 170
#define TFT_TEXT_INPUT_HEIGHT 80
#define TFT_ARROW_BUTTON_WIDTH 70
#define TFT_ARROW_BUTTON_HEIGHT 80
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTMenu;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  const char* moduleName;
  byte sensorType;
  byte sensorIndex;
  const char* sensorLabel;
  
} TFTSensorInfo;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  int x;
  int y;
  int w;
  int h;
} TFTInfoBoxContentRect;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTInfoBox
{
  public:
    TFTInfoBox(const char* caption, int width, int height, int x, int y, int captionXOffset=0);
    ~TFTInfoBox();

    void draw(TFTMenu* menuManager);
    void drawCaption(TFTMenu* menuManager, const char* caption);
    int getWidth() {return boxWidth;}
    int getHeight() {return boxHeight;}
    int getX() {return posX;}
    int getY() {return posY;}
    const char* getCaption() {return boxCaption;}

    TFTInfoBoxContentRect getContentRect(TFTMenu* menuManager);


   private:

    int boxWidth, boxHeight, posX, posY, captionXOffset;
    const char* boxCaption;
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// абстрактный класс экрана для TFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class AbstractTFTScreen
{
  public:

    virtual void setup(TFTMenu* menuManager) = 0;
    virtual void update(TFTMenu* menuManager,uint16_t dt) = 0;
    virtual void draw(TFTMenu* menuManager) = 0;
  
    AbstractTFTScreen();
    virtual ~AbstractTFTScreen();
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс экрана ожидания
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  bool isWindowsOpen : 1;
  bool windowsAutoMode : 1;

  bool isWaterOn : 1;
  bool waterAutoMode : 1;

  bool isLightOn : 1;
  bool lightAutoMode : 1;
  
} IdleScreenFlags;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTIdleScreen : public AbstractTFTScreen
{
  public:
  
    TFTIdleScreen();
    ~TFTIdleScreen();
    
    void setup(TFTMenu* menuManager);
    void update(TFTMenu* menuManager,uint16_t dt);
    void draw(TFTMenu* menuManager);

private:

  #ifdef USE_DS3231_REALTIME_CLOCK
  int lastMinute;
  void DrawDateTime(TFTMenu* menuManager);
  #endif

  UTFT_Buttons_Rus* screenButtons;
  IdleScreenFlags flags;

  TFTInfoBox* sensors[TFT_SENSOR_BOXES_COUNT];
  uint16_t sensorsTimer;

  void updateStatuses(TFTMenu* menuManager);
  void drawStatusesInBox(TFTMenu* menuManager,TFTInfoBox* box, bool status, bool mode, const char* onStatusString, const char* offStatusString, const char* autoModeString, const char* manualModeString);
  void drawSensorData(TFTMenu* menuManager,TFTInfoBox* box, int sensorIndex, bool forceDraw=false);

#ifdef USE_TEMP_SENSORS
  int windowsButton;
  TFTInfoBox* windowStatusBox;
  void drawWindowStatus(TFTMenu* menuManager);
#endif

#ifdef USE_WATERING_MODULE
  int waterButton;
  TFTInfoBox* waterStatusBox;
  void drawWaterStatus(TFTMenu* menuManager);
#endif

#ifdef USE_LUMINOSITY_MODULE
  int lightButton;
  TFTInfoBox* lightStatusBox;
  void drawLightStatus(TFTMenu* menuManager);
#endif

  int optionsButton;
    
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс экрана информации по окнам
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_TEMP_SENSORS

typedef Vector<String*> WindowsChannelsCaptions;

class TFTWindowScreen : public AbstractTFTScreen
{
  public:
  
    TFTWindowScreen();
    ~TFTWindowScreen();
    
    void setup(TFTMenu* menuManager);
    void update(TFTMenu* menuManager,uint16_t dt);
    void draw(TFTMenu* menuManager);

    private:
      int backButton;
      UTFT_Buttons_Rus* screenButtons;

      unsigned long lastWindowsState; // последнее состояние окон
      bool inited;
      bool lastAnyChannelActive;
      bool lastWindowsAutoMode;

      WindowsChannelsCaptions labels;

};
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_WATERING_MODULE
typedef Vector<String*> WaterChannelsCaptions;

class TFTWateringScreen : public AbstractTFTScreen
{
  public:
  
    TFTWateringScreen();
    ~TFTWateringScreen();
    
    void setup(TFTMenu* menuManager);
    void update(TFTMenu* menuManager,uint16_t dt);
    void draw(TFTMenu* menuManager);

    private:
      int backButton;
      UTFT_Buttons_Rus* screenButtons;

      bool inited;
      uint16_t lastWaterChannelsState; // последнее состояние полива
      bool lastWateringAutoMode;

      WaterChannelsCaptions labels;

};
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef USE_LUMINOSITY_MODULE
class TFTLightScreen : public AbstractTFTScreen
{
  public:
  
    TFTLightScreen();
    ~TFTLightScreen();
    
    void setup(TFTMenu* menuManager);
    void update(TFTMenu* menuManager,uint16_t dt);
    void draw(TFTMenu* menuManager);

    private:
      int backButton;
      UTFT_Buttons_Rus* screenButtons;

      bool inited;
      bool lastLightIsOn; // последнее состояние досветки
      bool lastLightAutoMode;


};
#endif
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTSettingsScreen : public AbstractTFTScreen
{
  public:
  
    TFTSettingsScreen();
    ~TFTSettingsScreen();
    
    void setup(TFTMenu* menuManager);
    void update(TFTMenu* menuManager,uint16_t dt);
    void draw(TFTMenu* menuManager);

    private:
      int backButton, decOpenTempButton, incOpenTempButton, decCloseTempButton, incCloseTempButton, incIntervalButton, decIntervalButton;
      UTFT_Buttons_Rus* screenButtons;

      bool inited;

      uint8_t openTemp, closeTemp;
      uint16_t interval;

      TFTInfoBox* closeTempBox;
      TFTInfoBox* openTempBox;
      TFTInfoBox* intervalBox;

      void drawValueInBox(TFTMenu* menuManager, TFTInfoBox* box, uint16_t val); 


};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  const char* screenName;
  AbstractTFTScreen* screen;
  
} TFTScreenInfo;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef Vector<TFTScreenInfo> TFTScreensList; // список экранов
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// класс-менеджер работы с TFT
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef struct
{
  bool isLCDOn : 1;
  bool buzzerActive : 1;
  byte pad : 6;
  
} TFTMenuFlags;
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class TFTMenu
{

public:
  TFTMenu();

  void setup();
  void update(uint16_t dt);

  void switchToScreen(const char* screenName);
  
  UTFT* getDC() { return tftDC; };
  URTouch* getTouch() { return tftTouch; };
  UTFTRus* getRusPrinter() { return &rusPrint; };
 

  void resetIdleTimer();

  void buzzer(); // пищим пищалкой
  void updateBuzzer();

private:

  TFTScreensList screens;
  UTFT* tftDC;
  URTouch* tftTouch;
  UTFTRus rusPrint;

  int currentScreenIndex;

  unsigned long idleTimer;
  unsigned long buzzerTimer;
  
  TFTMenuFlags flags;

  void lcdOn();
  void lcdOff();

  
};
#endif // USE_TFT_MODULE
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
