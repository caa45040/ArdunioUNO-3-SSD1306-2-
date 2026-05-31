
//misaki_test4n_nibai2_UNO3_1
//2倍角「あ、埼玉」を表示

//ヘッダーファイル
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <misakiUTF16.h>  //8x8ドット教育漢字


//定義
#define SCREEN_WIDTH 128     // OLED display width, in pixels
#define SCREEN_HEIGHT 64     // OLED display height, in pixels
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int k_x = 0;  //現在のXの位置
int k_y = 0;  //現在のYの位置


//表示位置
void k_locate(int x, int y) {

  k_x = x;
  k_y = y;

}  //k_locate


//2倍角の文字列を表示
//xは、画面上の横の位置
//yは、画面上の縦の位置
//ptrは、表示したい、漢字まじりの文字列
//戻り値は、次のXの位置
//
int k_printxy(int x, int y, char *ptr) {

  unsigned char font[8];        // フォント格納バッファ 入力
  unsigned char font2[16 * 2];  // フォント格納バッファ 出力

  while (*ptr) {  // 文字列分ループ

    ptr = getFontData(font, ptr, true);  // 1文字分のフォント取得
    if (!ptr) {
      break;  // エラーの場合は終了
    }  //endif

    for (int i = 0; i < 8; i++) {  //フォントの2倍角化ループ

      int a = font[i];   //入力フォントの読み出し
      int b = (a >> 4);  //入力フォントの読み出し
      int s;             //一時変数
      int j = i << 2;    //2倍角出力フォントのカウンター 4倍 j=i*4

      //上位4ビット b=b+b = b=b<<1
      s = 0;
      s = s | (0b00000001 & b);b = b + b;
      s = s | (0b00000110 & b);b = b + b;
      s = s | (0b00011000 & b);b = b + b;
      s = s | (0b01100000 & b);b = b + b;
      s = s | (0b10000000 & b);
      font2[j + 0] = s;
      font2[j + 2] = s;

      //下位4ビット a=a+a = a=a<<1
      s = 0;
      s = s | (0b00000001 & a);a = a + a;
      s = s | (0b00000110 & a);a = a + a;
      s = s | (0b00011000 & a);a = a + a;
      s = s | (0b01100000 & a);a = a + a;
      s = s | (0b10000000 & a);
      font2[j + 1] = s;
      font2[j + 3] = s;

    }  //for i

    //ビットマップの表示
    display.drawBitmap(x, y, font2, 16, 16, WHITE);
    x = x + 16;

  }  //while

  return (x);

}  //k_print


//表示
void k_print(char *ptr) {

  k_x = k_printxy(k_x, k_y, ptr);  //ptrの文字列を表示

}  //k_print


//表示　改行あり
void k_println(char *ptr) {

  k_x = k_printxy(k_x, k_y, ptr);

  k_x = 0;
  k_y = k_y + 16;

}  //k_println


//初期化
void setup() {

  // I2Cアドレスは使用するディスプレイに合わせて変更する
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

}  //setup


//メインループ
void loop() {

  // 画面表示をクリア
  display.clearDisplay();


  //開始　2倍角の文字列の表示
  char *str = "あ、埼玉";  // 文字列
  int x = 0;
  int y = 0;

  k_locate(x, y);  //表示座標
  k_print(str);    //2倍角の文字列を表示
  //終了


  // 描画バッファの内容を画面に表示
  display.display();

  delay(5000);  //5秒待つ

}  //loop



