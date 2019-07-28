# ws2812-network-controller
This is a simple Arduino project, that controls WS2812b led-strips over network. It is kept REALLY stupid and simple.

The ESP8266 or ESP32 will open a UDP-socket and wait for incoming packets. The packets contains RGB information, which are displayed on all leds right away.

# How to control?
You could build a program, which sends UDP-packets containing the RGB-data. I built an Android app, which shows a colorpicker-wheel to easily pick the color. You can find it [here](https://github.com/jakobod/Color-picker-app)

