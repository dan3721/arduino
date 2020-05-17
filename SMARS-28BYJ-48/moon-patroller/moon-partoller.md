# Moon Partoller

## Summary

SMARS modular robot driven with 28JYB-48 steppers. When switched on it will slowly crawl forward until blocked at which point it will reverse drive the left track to turn left and then proceed forward again. Yeah it's a super simplistic program but, it does use the standard Arduino [Stepper](https://www.arduino.cc/en/Reference/Stepper) library and interrupts to handle the ultrasonic sensor. Color scheme and name are tributes to [Moon Patrol](https://en.wikipedia.org/wiki/Moon_Patrol).

## Parts

Slave wheels, tracks, and ultrasonic housing printed from original [SMARS modular robot](https://www.thingiverse.com/thing:2662828) files. Chassis and drive wheels printed from [SMARS - 28BYJ-48](https://www.thingiverse.com/thing:2778904). I designed the [switch mount](https://www.thingiverse.com/thing:4369578).

1. [Arduino Uno](https://store.arduino.cc/usa/arduino-uno-rev3) (yep if you look closely, I used a [Duemilanove](https://www.arduino.cc/en/Main/arduinoBoardDuemilanove)) because that's what I had lying around but its pretty dated.
1. [Longruner 5X Geared Stepper Motor 28byj 48 Uln2003 5v Stepper Motor Uln2003 Driver Board Compatible with ArduinoIDE (with Wire)](https://www.amazon.com/gp/product/B06X982PSL/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
1. [SainSmart HC-SR04 Ranging Detector](https://www.amazon.com/SainSmart-HC-SR04-Ranging-Detector-Distance/dp/B004U8TOE6/ref=sr_1_3?crid=1S420M3423PCD&dchild=1&keywords=ultrasonic+sensor&qid=1589747734&s=electronics&sprefix=ultras%2Celectronics%2C151&sr=1-3)
1. [Delipow USB 9V Lithium ion Rechargeable Battery](https://www.amazon.com/gp/product/B07VDHWSQF/ref=ppx_yo_dt_b_asin_image_o09_s00?ie=UTF8&psc=1)
1. [M2 M3 M4 Alloy Steel Screws Nuts and Washers](https://www.amazon.com/gp/product/B07WP64ZHK/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)

Thanks to e-radionica.com for the [Fritzing 28JYB-48 steppers and ULN2003A breakout boards](https://github.com/e-radionicacom/e-radionica.com-Fritzing-Library-parts-)


