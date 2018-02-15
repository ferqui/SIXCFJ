# PROJECT SIXCFJ

## Table of Contents
1. [Description](#description)
2. [Requirements](#requirements)
3. [Software Requirements](#software-requirements)
4. [Installation](#installation)
5. [Connections](#connections)
6. [Android/iOS App](#androidios-application)
6. [Mentions](#mentions)
7. [Authors](#credits)

## Description
The objective was the creation of a maze solver using an Arduino Leonardo and some additional sensors. Moreover, once the robot has find the exit, it has to go back to the initial box using the minimal number of steps.


<p align="center">
<a href="https://www.youtube.com/watch?v=b67RzzSxT4M&feature=youtu.be"> <img src="https://github.com/sixcfj/SIXCFJ/blob/master/Memoria/img/MazeVideo.PNG?raw=true"></a>
</p>

## Requirements
* 1 Arduino Leonardo
* 1 Ultrasonic Sensor
* 2 Sharp Infrared Sensor
* 1 Bluetooth Module
* 2 Wheel
* 1 Motor driver
* 2 DC Motors
* 3 CNY70
* 3 LED RGB

## Software Requirements
* Arduino IDE
* React-Native

## Installation
First,

> git clone https://github.com/sixcfj/SIXCFJ.git

Once you have the repo, create a new Arduino project with the codes that are in *Arduino Codes/main*.  Then, you just need to upload it to your Arduino Leonardo and that's all.

## Connections

Pin  | Conection
-----| -----
A0 | CNY70 (Left)
A1 | CNY70 (Right)
A3 | Ultrasonic Sensor
A4 | Sharp Sensor (Right)
A5 | CNY70  (Back)
A6  | % of remaining Battery
A8 | Sharp Sensor (Left)
11 | LED3
12 | LED1
13 | LED2
RX,TX | Bluetooth

## Slides
In the slides directory you can run:

> npm start

This will runs the app in the development mode. Open http://localhost:3000 to view it in the browser.

## Android/iOS Application
First of all, download the dependencies with

> npm start

Then, you need to edit */node_modules/react-native-bluetooth-serial/android/src/main/java/com/rusel/RCTBluetoothSerial/RCTBluetoothSerialPackage* and delete those *__override__*.

Finally, just run one of these commands

> react-native run-android

> react-native run-ios

<p align="center">
<a href="https://youtu.be/UIcNs9Nyqnc"> <img src="https://github.com/sixcfj/SIXCFJ/blob/master/Memoria/img/DemoVideo.PNG?raw=true"></a>
</p>

About the songs, we can't share them due to copyright issues, but we encourage you to test it with your own music.

## Mentions
We would like to thanks maniacbug for creating this library, it was really helpful for our project. https://github.com/maniacbug/StandardCplusplus .

## Credits
  - Juan Francisco Cabrera Sánchez ([JF95](https://github.com/JF95))
  - Carlos Gallardo Polanco ([gpcarlos95](https://github.com/gpcarlos95))
  - Fernando Manuel Quintana Velazquez ([ferqui](https://github.com/ferqui))
