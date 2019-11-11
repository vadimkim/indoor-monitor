EESchema Schematic File Version 4
LIBS:schematic-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:WeMos_D1_mini U1
U 1 1 5D81E412
P 3550 3200
F 0 "U1" H 3550 4000 50  0000 C CNN
F 1 "WeMos_D1_mini" H 3550 2400 50  0000 C CNN
F 2 "Module:WEMOS_D1_mini_light" H 3550 2050 50  0001 C CNN
F 3 "https://wiki.wemos.cc/products:d1:d1_mini#documentation" H 1700 2050 50  0001 C CNN
	1    3550 3200
	1    0    0    -1  
$EndComp
$Comp
L schematic-rescue:Nokia_5110_LCD-nokia_5110-3310_lcd U2
U 1 1 5D81EE51
P 5600 2900
F 0 "U2" H 5550 3450 60  0000 L CNN
F 1 "Nokia_5110_LCD" H 5250 2375 60  0000 L CNN
F 2 "" H 5600 2900 60  0000 C CNN
F 3 "" H 5600 2900 60  0000 C CNN
	1    5600 2900
	1    0    0    -1  
$EndComp
$Comp
L Sensor:BME280 U3
U 1 1 5D81F64A
P 5850 4375
F 0 "U3" H 6200 3875 50  0000 R CNN
F 1 "BME280" H 6350 4875 50  0000 R CNN
F 2 "Package_LGA:Bosch_LGA-8_2.5x2.5mm_P0.65mm_ClockwisePinNumbering" H 7350 3925 50  0001 C CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf" H 5850 4175 50  0001 C CNN
	1    5850 4375
	-1   0    0    1   
$EndComp
$Comp
L Sensor_Gas:CCS811 U4
U 1 1 5D82033B
P 7400 4375
F 0 "U4" H 7175 4875 50  0000 C CNN
F 1 "CCS811" H 7175 3875 50  0000 C CNN
F 2 "Package_LGA:AMS_LGA-10-1EP_2.7x4mm_P0.6mm" H 7400 3775 50  0001 C CNN
F 3 "http://ams.com/eng/Products/Environmental-Sensors/Air-Quality-Sensors/CCS811" H 7400 4175 50  0001 C CNN
	1    7400 4375
	1    0    0    -1  
$EndComp
Text Notes 5825 3225 0    50   ~ 0
1 - RST\n2 - CE\n3 - DC\n4 - DIN\n5 - CLK\n6 - VCC\n7 - LIGHT\n8 - GND
Text Label 5100 2900 0    50   ~ 0
CE
Wire Wire Line
	3950 2800 4600 2800
Wire Wire Line
	4600 2800 4600 2500
Wire Wire Line
	4600 2500 5250 2500
Text Label 5100 2500 0    50   ~ 0
RST
Text Label 5100 3000 0    50   ~ 0
DC
Wire Wire Line
	3950 3100 5250 3100
Text Label 5100 3100 0    50   ~ 0
DIN
Wire Wire Line
	3950 3400 4600 3400
Wire Wire Line
	4600 3400 4600 3200
Wire Wire Line
	4600 3200 5250 3200
Text Label 5100 3200 0    50   ~ 0
CLK
Wire Wire Line
	3650 2400 3650 2300
Wire Wire Line
	3650 2300 4775 2300
Wire Wire Line
	4775 3300 5250 3300
Text Label 3650 2300 0    50   ~ 0
3V3
$Comp
L power:+5V #PWR?
U 1 1 5D82F288
P 3450 1800
F 0 "#PWR?" H 3450 1650 50  0001 C CNN
F 1 "+5V" H 3465 1973 50  0000 C CNN
F 2 "" H 3450 1800 50  0001 C CNN
F 3 "" H 3450 1800 50  0001 C CNN
	1    3450 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 1800 3450 2400
Connection ~ 4775 3300
Wire Wire Line
	5750 5150 6725 5150
$Comp
L power:GND #PWR?
U 1 1 5D831D7D
P 3550 5325
F 0 "#PWR?" H 3550 5075 50  0001 C CNN
F 1 "GND" H 3555 5152 50  0000 C CNN
F 2 "" H 3550 5325 50  0001 C CNN
F 3 "" H 3550 5325 50  0001 C CNN
	1    3550 5325
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 4000 3550 5225
Wire Wire Line
	7400 5225 7000 5225
Connection ~ 3550 5225
Wire Wire Line
	3550 5225 3550 5325
Wire Wire Line
	5950 3775 6600 3775
Wire Wire Line
	6600 3775 6600 5225
Connection ~ 6600 5225
Wire Wire Line
	5950 3775 5750 3775
Connection ~ 5950 3775
Text Label 3625 5225 0    50   ~ 0
GND
Wire Wire Line
	4775 2300 4775 3300
Wire Wire Line
	6725 3625 4775 3625
Connection ~ 6725 3625
Wire Wire Line
	6725 5150 6725 3625
Wire Wire Line
	4775 3625 4775 3300
Wire Wire Line
	7400 3625 6725 3625
Wire Wire Line
	7400 3875 7400 3625
Wire Wire Line
	5750 5150 5750 4975
Wire Wire Line
	7400 5225 7400 4875
Wire Wire Line
	7000 4275 6850 4275
Wire Wire Line
	6850 4275 6850 3675
Wire Wire Line
	6850 3675 5000 3675
Wire Wire Line
	4150 3675 4150 3000
Wire Wire Line
	4150 3000 3950 3000
Wire Wire Line
	3950 2900 4225 2900
Wire Wire Line
	4225 2900 4225 3575
Wire Wire Line
	4225 3575 5075 3575
Wire Wire Line
	6925 3575 6925 4375
Wire Wire Line
	6925 4375 7000 4375
Text Label 4275 3675 0    50   ~ 0
SDA
Text Label 4275 3575 0    50   ~ 0
SCL
Wire Wire Line
	5250 4475 5075 4475
Wire Wire Line
	5075 4475 5075 3575
Connection ~ 5075 3575
Wire Wire Line
	5075 3575 6925 3575
Wire Wire Line
	5250 4675 5000 4675
Wire Wire Line
	5000 4675 5000 3675
Connection ~ 5000 3675
Wire Wire Line
	5000 3675 4150 3675
Wire Wire Line
	3950 3200 4450 3200
Wire Wire Line
	4450 3200 4450 2900
Wire Wire Line
	4450 2900 5250 2900
Wire Wire Line
	3950 3300 4500 3300
Wire Wire Line
	4500 3300 4500 3000
Wire Wire Line
	4500 3000 5250 3000
Wire Wire Line
	3550 5225 4875 5225
Wire Wire Line
	5250 2700 4875 2700
Wire Wire Line
	4875 2700 4875 5225
Connection ~ 4875 5225
Wire Wire Line
	4875 5225 6600 5225
Wire Wire Line
	7000 4675 7000 5225
Connection ~ 7000 5225
Wire Wire Line
	7000 5225 6600 5225
$EndSCHEMATC
