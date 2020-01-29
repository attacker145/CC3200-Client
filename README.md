# CC3200-Client
This code does remote control and monitoring of the CC3200 board with AK9753 (Qwiic) (https://www.sparkfun.com/products/14349) sensor connected to I2C port. The board reports data collected from the AK9753 Human Presence sensor, the onboard I2C digital temperature sensor, and current status of the Red LED to the http://cnktechlabs.com/webapp_pub/grid.php webpage. The same webpage can be used to turn ON or OFF the onboard red LED.
AK9753 is connected to I2C lines on the board - PIN_01 is the SCL and PIN_02 is the SDA. Compile and download CC3200_client.bin file into CC3200 development board. Connect Dev board to a PC. View USB messages on a Serial terminal on your PC. Monitor status of CC3200 sensors and Red LED as well as control the Red LED from http://cnktechlabs.com/webapp_pub/grid.php webpage.
Serial Port Settings:
115200
8 bit
1 stop
No parity
