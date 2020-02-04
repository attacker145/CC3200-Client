To test the IOT project with your CC3200 development board:
Download and install the CC3200 SDK on your PC.
Download files from the Github. One of the files listed on the Github is a zip file of the CCS project (CC3200_client(CCS-project).zip).
You can download and unzip the file.
Import project files into your CCS workspace (link to CCS project import example).
Enter your WiFi credentials (SSID name and password) in the common.h file.
Monitor and control CC3200 board from cnktechlabs.com/webapp_pub/grid.php.
Project Updates:
The C-source code has beed updated. The new code allows remote control and monitoring of the CC3200 board with AK9753 (Qwiic) sensor connected to I2C port. The SparkFun AK9753 Human Presence Sensor Breakout is a Qwiic-enabled, 4-channel Nondispersive Infrared (NDIR) sensor. The board reports data collected from the AK9753 Human Presence sensor to the grid.php webpage, the onboard TMP006 I2C digital temperature sensor, and current status of the Red LED. The same webpage can be used to turn ON or OFF the onboard red LED. AK9753 is connected to I2C lines on the board - PIN_01 is the SCL and PIN_02 is the SDA. Compile and download CC3200_client.bin file into CC3200 development board. Connect Dev board to a PC. View USB messages on a Serial terminal on your PC. Monitor status of CC3200 sensors and Red LED as well as control the Red LED from cnktechlabs.com/webapp_pub/grid.php webpage.
Serial Port Settings:
115200
8 bit
1 stop
No parity
