#include <seeed_line_chart.h> //include the library

TFT_eSPI tft;

// #define USE_ANALOG

#ifndef USE_ANALOG
  #include "ecg_data.h"
  #define MAX_DATA  500
  int idx;
#endif

#define max_size 50 //maximum size of data
doubles data; //Initilising a doubles type to store data
int recv_data;
TFT_eSprite spr = TFT_eSprite(&tft);  // Sprite 

void setup() {
    Serial.begin(115200);  
    pinMode(A0, INPUT);
    tft.begin();
    spr.createSprite(TFT_HEIGHT,TFT_WIDTH);
    spr.setRotation(3);
    tft.setRotation(3);
#ifndef USE_ANALOG    
    idx = 0;
#endif    
    Serial.println("Test Started!!");
}

void loop() {
    spr.fillSprite(TFT_WHITE);
    #ifdef USE_ANALOG
      recv_data = analogRead(A0);
    #else      
      // recv_data = ecg_data[idx++];

      recv_data = map(ecg_data[idx++]*100,-29,72,0,165);    
      if (idx > MAX_DATA) idx = 0;
    #endif
    // debug
    Serial.println(recv_data);      
    
    if (data.size() == max_size) {
        data.pop();//this is used to remove the first read variable
    }
    data.push(recv_data); //read variables and store in data

    //Settings for the line graph title
    auto header =  text(0, 0)
                #ifdef USE_ANALOG
                  .value("Light Sendor data")
                #else
                  .value("ECG data")
                #endif
                .align(center)
                .valign(vcenter)
                .width(tft.width())
                .thickness(2);

    header.height(header.font_height() * 2);
    header.draw(); //Header height is the twice the height of the font

  //Settings for the line graph
    auto content = line_chart(20, header.height()); //(x,y) where the line graph begins
         content
                .height(tft.height() - header.height() * 1.5) //actual height of the line chart
                .width(tft.width() - content.x() * 2) //actual width of the line chart
                .based_on(0.0) //Starting point of y-axis, must be a float
                .show_circle(false) //drawing a cirle at each point, default is on.
                .value(data) //passing through the data to line graph
                .color(TFT_RED) //Setting the color for the line
                .draw();
    spr.pushSprite(0, 0);
    delay(10);
}
