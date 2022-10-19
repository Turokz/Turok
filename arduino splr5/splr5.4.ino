
#define samp_siz 20 // количество расчетов - можно увеличить до 20
#define rise_threshold 5
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>         //download here: https://www.electronoobs.com/eng_arduino_Adafruit_GFX.php
#include <Adafruit_SSD1306.h>     //downlaod here: https://www.electronoobs.com/eng_arduino_Adafruit_SSD1306.php
#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#include <U8g2lib.h>
#include <Wire.h>
#include <U8g2Graphing.h>





//This example is using SSD1306 128x64 I2C monochrome OLED display





U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); //full buffer
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); //page buffer





//Constructor, takes a reference to the current instance of U8g2 display.
U8g2Graphing graph(&u8g2);





float result = 0;
float _ndx = 0;
int state = 0;
int analogIn = 0;





uint32_t preMil = 0;
uint32_t preMil2 = 0;
uint32_t preMil3 = 0;






void setup() {
    Serial.begin(9600);
    pinMode(A0, INPUT);
    pinMode(A0, OUTPUT);



 delay(100);
  u8g2.begin();
  u8g2.setFont(u8g2_font_tom_thumb_4x6_tf);
  u8g2.setFontMode(1);
  u8g2.setDrawColor(2);





//There are 2 method to initiate the graph, .beginInt and .begin
  //.beginInt is used for integer data and lower RAM allocation.
  //.begin is used for floating point data but higher RAM allocation.
  //Using .begin on Uno will cause glitch and crash as it'll be over the RAM limit.
  //The arguments for the function is (from x, from y, to x, to y) in pixels.
  graph.beginInt(0, 9, 127, 63);





//Display type init, explained below.
  graph.displaySet(true, false);
}








void loop() {







 realtimeGraph();
  }
  
  





void realtimeGraph() {
  
  






  //Initialize setting
  graph.displaySet(true, false);
  graph.pointerSet(false);
  graph.rangeSet(false);





preMil2 = millis();
  //10 second loop
  while (millis() - preMil2 <= 100000) {





  for (float f = 0.0; f < 2.0; f += 0.02) {
      _ndx = (sin(f * PI) * 250) + 250;
      result = 100;





    if (millis() - preMil >= 10000) {
        preMil = millis();

 float reads[samp_siz], sum;
  long int now, ptr;
  float last, reader, start;
  int first, second, third, before, resultt, print_value;
  bool rising;
  int rise_count;
  int n;
  long int last_beat;





 for (int i=0; i<samp_siz; i++)
    reads[i] = 0;
  sum = 0;
  ptr = 0;



 



 // в течение 20 мс вычисляем среднее значение датчика
  while (1) {
    n = 0;
    start = millis();
    reader = 0.;
    do {
      reader += analogRead(20);
      n++;
      now = millis();
    }





   // записываем полученное среднее значение в массив
    while (now < start + 20);
    reader /= n;  // we got an average
    sum -= reads[ptr];
    sum += reader;
    reads[ptr] = reader;
    last = sum / samp_siz;







   // проверяем, появляется ли увеличение среднего значения
    if (last > before) {
      rise_count++;





     // если значение увеличивается, что подразумевает сердцебиение
      // включаем светодиод и записываем время с последнего удара
      // отслеживаем два предыдущих удара, чтобы получить взвешенное среднее
      if (!rising && rise_count > rise_threshold) {
        digitalWrite(10, HIGH);
        rising = true;
        first = millis() - last_beat;
        last_beat = millis();





       // рассчитываем средневзвешенную частоту сердечных сокращений
        resultt = 60000. / (0.4 * first + 0.1 * second + 0.1 * third);
        Serial.println(resultt);
        third = second;
        second = first;
      }
    }





  else {
      rising = false;
      rise_count = 0;
      digitalWrite(10, LOW);
    }
    before = last;
    ptr++;
    ptr %= samp_siz;






      





    //Set the pointer to be displayed or not
      //the _ndx is an arbitary number from 0-500
      //being 0 is the rightmost/recent point and 500 is leftmost point.
      //If you don't want the arbitrary number, use .pointerSetI(bool, int) instead with 0 to max data point _ndx,
      //you can get the maximum number of data point value by using .getDataLen() function.
      graph.pointerSet(true, (int)_ndx);
     
      //Input the data to graph
      result = resultt;
      graph.inputValue(resultt);

 



    u8g2.firstPage();
      do {
        u8g2.setCursor(4, 7);
        u8g2.setFont(u8g2_font_6x10_tr);
        u8g2.print("Heart Rate");





      //Display the graph
        graph.displayGraph();
      } while (u8g2.nextPage());
    }
  
  //Clear data points to prepare for the next function
  graph.clearData();





u8g2.firstPage();
  do {
  } while (u8g2.nextPage());





preMil2 = millis();
}
}
}
}