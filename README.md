# arduino-watering
The goal of this project build a watering system based on the need to open the water or not with information getted from moisture sensors.
The sensors should cover an area of 1x30m.
With this I intent to join programming with electronics, remember c++, and lear more about ardunios.

Problems:
 - To cover a so long distance, 3 arduinos will be used. One master, and two slaves with two moisture sensors each and an electro valve each.
 - The communications will be RS485 as is the most reliable serial comminication to cover such distances.
 
Spec:
- Should have 4 sensors to monitor the moisture on soil.
- Should have a RGB led, for status indication.
- Should have a LCD and a Keypad do configure.
- Should store the configuration on a SD card.
- Should have a Clock system to run on scheduled.
- Should publish the data to some web site. (Fase 2)
- Should run on "multitasking" mode.
- Should use the less libraries as possible.
- Will have a master application, wich is responsible for all configuration and running.
- Will have a slave application, wich is responsible for reading the sensors and open the valve, controlled by the master


 

