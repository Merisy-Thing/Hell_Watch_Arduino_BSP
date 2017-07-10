#include <SPI.h>
#include <avr/pgmspace.h>
#include <Gamebuino.h>
#include <winbondflash.h>
Gamebuino gb;
winbondFlashSPI spiFlash;

extern const byte logo[] PROGMEM;

extern const byte font3x5[]; //a small but efficient font (default)
extern const byte font5x7[]; //a large, comfy font

byte buffer[128+128];//only first 128B used
unsigned token;
char currentGame[9];
char userName[USERNAME_LENGTH+1];
int contrast;
int backlightMin;
int backlightMax;
int lightMin;
int lightMax;
int volumeMax;
int volumeDefault;
int startMenuTimer;
int batteryCritic, batteryLow, batteryMed, batteryFull;

const char strWizard[] PROGMEM = "Checkup Wizard";
const char strChangeSettings[] PROGMEM =  "Change settings";
const char strSeeAllSettings[] PROGMEM =  "See all settings";
const char strDefaultSettings[] PROGMEM = "Default settings";
const char strEraseSettings[] PROGMEM =   "Erase settings page";
const char strSaveSettings[] PROGMEM =   "Save changes";

#define MAINMENU_LENGTH 6
const char* const mainMenu[MAINMENU_LENGTH] PROGMEM = {
  strWizard,
  strChangeSettings,
  strSeeAllSettings,
  strDefaultSettings,
  strEraseSettings,
  strSaveSettings
};

void readSettings(); //function prototype

void setup(){
  gb.begin();
  gb.titleScreen(logo);
  spiFlash.begin(_W25Q80,SPI,SS);
  if(!gb.settingsAvailable()){
    restoreSettings();
    Wizard();
    saveSettings();
  }
  readSettings();
}

void loop(){
  switch(gb.menu(mainMenu,MAINMENU_LENGTH)){
  case -1:
    gb.titleScreen(logo);
    break;
  case 0:
    Wizard();
    saveSettings();
    break;
  case 1: //change settings
    changeSettings();
    saveSettings();
    break;
  case 2: //display settings
    seeAllSettings();
    break;
  case 3: //default settings
    restoreSettings();
    break;
  case 4: //erase settings
    eraseSettings();
    break;
  case 5: //save and exit
    saveSettings();
    break;
  default:
    break;
  }
}

void pressAtoContinue(){
  gb.display.cursorX = LCDWIDTH-2*gb.display.fontWidth;
  gb.display.cursorY = LCDHEIGHT-gb.display.fontHeight;
  gb.display.print(F("\25\20"));
  gb.display.update();
  while(1){
    gb.buttons.update();
    if(gb.buttons.pressed(BTN_A))
      break;
    delay(20);
  }
  gb.display.clear();
  gb.display.update();
}

void readSettings(){
  for(byte i=0; i<128; i++){
    buffer[i] = gb.rom_read_byte(SETTINGS_PAGE+i);
  }
  token = gb.rom_read_word(SETTINGS_PAGE);
  for(byte i=0; i<9; i++){
    currentGame[i] = (char)gb.rom_read_byte(SETTINGS_PAGE+OFFSET_CURRENTGAME+i);
  }
  for(byte i=0; i<USERNAME_LENGTH; i++){
    userName[i] = (char)gb.rom_read_byte(SETTINGS_PAGE+OFFSET_USERNAME+i);
  }
  contrast = gb.rom_read_byte(SETTINGS_PAGE+OFFSET_CONTRAST);
  backlightMin = gb.rom_read_byte(SETTINGS_PAGE+OFFSET_BACKLIGHT_MIN);
  backlightMax = gb.rom_read_byte(SETTINGS_PAGE+OFFSET_BACKLIGHT_MAX);
  lightMin = gb.rom_read_word(SETTINGS_PAGE+OFFSET_LIGHT_MIN);
  lightMax = gb.rom_read_word(SETTINGS_PAGE+OFFSET_LIGHT_MAX);
  volumeMax = gb.rom_read_byte(SETTINGS_PAGE+OFFSET_VOLUME_MAX);
  volumeDefault = gb.rom_read_byte(SETTINGS_PAGE+OFFSET_VOLUME_DEFAULT);
  startMenuTimer = gb.rom_read_byte(SETTINGS_PAGE+OFFSET_START_MENU_TIMER);
  batteryCritic = gb.rom_read_word(SETTINGS_PAGE+OFFSET_BATTERY_CRITIC);
  batteryLow = gb.rom_read_word(SETTINGS_PAGE+OFFSET_BATTERY_LOW);
  batteryMed = gb.rom_read_word(SETTINGS_PAGE+OFFSET_BATTERY_MED);
  batteryFull = gb.rom_read_word(SETTINGS_PAGE+OFFSET_BATTERY_FULL);
}

void write_flash_page(uint32_t addr, byte *buffer) {
  while(spiFlash.busy());
  spiFlash.WE();
  spiFlash.eraseSector(addr);

  while(spiFlash.busy());
  spiFlash.WE();
  spiFlash.writePage(addr, buffer);
}

void saveSettings(){
  memset(buffer, 0xFF, 256);
  *(unsigned*)buffer = token;
  strcpy((char*)(&buffer[OFFSET_USERNAME]),userName);
  buffer[OFFSET_CONTRAST] = contrast;
  buffer[OFFSET_BACKLIGHT_MIN] = backlightMin;
  buffer[OFFSET_BACKLIGHT_MAX] = backlightMax;
  *(unsigned*)(&buffer[OFFSET_LIGHT_MIN]) = lightMin;
  *(unsigned*)(&buffer[OFFSET_LIGHT_MAX]) = lightMax;
  buffer[OFFSET_VOLUME_MAX] = volumeMax;
  buffer[OFFSET_VOLUME_DEFAULT] = volumeDefault;
  buffer[OFFSET_START_MENU_TIMER] = startMenuTimer;
  *(unsigned*)(&buffer[OFFSET_BATTERY_CRITIC]) = batteryCritic;
  *(unsigned*)(&buffer[OFFSET_BATTERY_LOW]) = batteryLow;
  *(unsigned*)(&buffer[OFFSET_BATTERY_MED]) = batteryMed;
  *(unsigned*)(&buffer[OFFSET_BATTERY_FULL]) = batteryFull;

  write_flash_page (SETTINGS_PAGE, buffer);

  gb.popup(F("Settings saved"), 40);
}

void eraseSettings(){
  memset(buffer, 0, 128);
  write_flash_page (SETTINGS_PAGE, buffer);

  gb.popup(F("Page erased"), 40);
}


