EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Amateur Wireless Rotor Controller"
Date "2021-09-25"
Rev "v0.1"
Comp "CO7WT"
Comment1 "Author pavelmc@gmail.com"
Comment2 "Twitter: @co7wt, Telegram: @pavelmc"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L wemos_vlad:Wemos_vlad_clone U1
U 1 1 614FC305
P 2050 4650
F 0 "U1" H 2125 5481 50  0000 C CNN
F 1 "Wemos_vlad_clone" H 2125 5390 50  0000 C CNN
F 2 "wemos_clone:Wemos_Clone" H 2050 4650 50  0001 C CNN
F 3 "" H 2050 4650 50  0001 C CNN
	1    2050 4650
	1    0    0    -1  
$EndComp
Text Label 3100 5400 0    50   ~ 0
Motor_Right
Text Label 3100 5500 0    50   ~ 0
Motor_Left
Text Label 3100 5600 0    50   ~ 0
Motor_Up
Text Label 3100 5700 0    50   ~ 0
Motor_Down
Text Label 1000 5500 0    50   ~ 0
Hall_AZ
Text Label 1000 5400 0    50   ~ 0
Hall_EL
Text Label 3100 4900 0    50   ~ 0
Limit_AZ
Text Label 3100 5000 0    50   ~ 0
Limit_EL
Wire Wire Line
	2200 4000 2200 3750
Wire Wire Line
	2200 3750 2300 3750
Wire Wire Line
	2300 3750 2300 4000
Wire Wire Line
	2400 4000 2400 3750
Wire Wire Line
	2400 3750 2300 3750
Connection ~ 2300 3750
$Comp
L Device:CP1 C3
U 1 1 61502830
P 2100 1900
F 0 "C3" H 2215 1946 50  0000 L CNN
F 1 "100uF/25v" H 2215 1855 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D13.0mm_P5.00mm" H 2100 1900 50  0001 C CNN
F 3 "~" H 2100 1900 50  0001 C CNN
	1    2100 1900
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4001 D3
U 1 1 615036DF
P 1400 2050
F 0 "D3" H 1400 1834 50  0000 C CNN
F 1 "1N4001" H 1400 1925 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 1400 1875 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 1400 2050 50  0001 C CNN
	1    1400 2050
	-1   0    0    1   
$EndComp
$Comp
L Device:CP1 C4
U 1 1 61506BB1
P 3750 1250
F 0 "C4" H 3865 1296 50  0000 L CNN
F 1 "10uF/16v" H 3865 1205 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 3750 1250 50  0001 C CNN
F 3 "~" H 3750 1250 50  0001 C CNN
	1    3750 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 900  3750 900 
Wire Wire Line
	3750 900  3750 1100
Wire Wire Line
	3750 1400 3750 1550
Wire Wire Line
	3750 1550 3250 1550
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 615092B8
P 750 950
F 0 "J1" H 668 625 50  0000 C CNN
F 1 "Conn_01x02" H 668 716 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 750 950 50  0001 C CNN
F 3 "~" H 750 950 50  0001 C CNN
	1    750  950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	2150 6200 2150 6400
Wire Wire Line
	2150 6400 2050 6400
Wire Wire Line
	2050 6400 2050 6200
Wire Wire Line
	1850 6200 1850 6400
Connection ~ 2050 6400
Wire Wire Line
	1950 6200 1950 6400
Wire Wire Line
	1950 6400 1850 6400
Connection ~ 1950 6400
$Comp
L Device:R R4
U 1 1 61510C7D
P 3500 4300
F 0 "R4" H 3570 4346 50  0000 L CNN
F 1 "4k7" H 3570 4255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3430 4300 50  0001 C CNN
F 3 "~" H 3500 4300 50  0001 C CNN
	1    3500 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 61512150
P 3750 4300
F 0 "R5" H 3820 4346 50  0000 L CNN
F 1 "4k7" H 3820 4255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3680 4300 50  0001 C CNN
F 3 "~" H 3750 4300 50  0001 C CNN
	1    3750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 4150 3750 3750
Wire Wire Line
	3750 3750 3500 3750
Connection ~ 2400 3750
Wire Wire Line
	3500 4150 3500 3750
Connection ~ 3500 3750
Wire Wire Line
	3500 3750 2950 3750
Wire Wire Line
	3500 4450 3500 4900
Wire Wire Line
	3750 4450 3750 5000
Wire Wire Line
	2900 5000 3750 5000
Wire Wire Line
	2900 4900 3500 4900
$Comp
L power:GND #PWR02
U 1 1 61521F64
P 2050 6550
F 0 "#PWR02" H 2050 6300 50  0001 C CNN
F 1 "GND" H 2055 6377 50  0000 C CNN
F 2 "" H 2050 6550 50  0001 C CNN
F 3 "" H 2050 6550 50  0001 C CNN
	1    2050 6550
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:LM7805_TO220 U3
U 1 1 6152BA33
P 3250 2300
F 0 "U3" H 3250 2542 50  0000 C CNN
F 1 "LM7805_TO220" H 3250 2451 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3250 2525 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 3250 2250 50  0001 C CNN
	1    3250 2300
	1    0    0    -1  
$EndComp
$Comp
L Diode:1N4001 D4
U 1 1 6152BA3F
P 1700 1750
F 0 "D4" V 1746 1671 50  0000 R CNN
F 1 "1N4001" V 1655 1671 50  0000 R CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 1700 1575 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 1700 1750 50  0001 C CNN
	1    1700 1750
	0    1    1    0   
$EndComp
$Comp
L Device:CP1 C5
U 1 1 6152BA4C
P 3750 2650
F 0 "C5" H 3865 2696 50  0000 L CNN
F 1 "10uF/16v" H 3865 2605 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 3750 2650 50  0001 C CNN
F 3 "~" H 3750 2650 50  0001 C CNN
	1    3750 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 2300 3750 2300
Wire Wire Line
	3750 2300 3750 2500
Wire Wire Line
	3750 2800 3750 2950
Wire Wire Line
	3750 2950 3250 2950
Connection ~ 3250 2950
$Comp
L Regulator_Linear:LM7812_TO220 U2
U 1 1 615595B1
P 3250 900
F 0 "U2" H 3250 1142 50  0000 C CNN
F 1 "LM7812_TO220" H 3250 1051 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3250 1125 50  0001 C CIN
F 3 "http://www.fairchildsemi.com/ds/LM/LM7805.pdf" H 3250 850 50  0001 C CNN
	1    3250 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 1200 3250 1550
$Comp
L Relay:FINDER-32.21-x000 K1
U 1 1 615ADB52
P 6050 1300
F 0 "K1" H 6480 1346 50  0000 L CNN
F 1 "FINDER-32.21-x000" H 6480 1255 50  0000 L CNN
F 2 "awrc:Relay_THT_UPS_80H-1C-S" H 7320 1270 50  0001 C CNN
F 3 "https://gfinder.findernet.com/assets/Series/355/S32EN.pdf" H 6050 1300 50  0001 C CNN
	1    6050 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 615BBC86
P 5850 2250
F 0 "#PWR05" H 5850 2000 50  0001 C CNN
F 1 "GND" H 5855 2077 50  0000 C CNN
F 2 "" H 5850 2250 50  0001 C CNN
F 3 "" H 5850 2250 50  0001 C CNN
	1    5850 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 2250 5850 2100
$Comp
L Device:R R6
U 1 1 615BBC8D
P 5300 1900
F 0 "R6" V 5507 1900 50  0000 C CNN
F 1 "100" V 5416 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5230 1900 50  0001 C CNN
F 3 "~" H 5300 1900 50  0001 C CNN
	1    5300 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5450 1900 5550 1900
Wire Wire Line
	5850 1700 5850 1650
$Comp
L Diode:1N4001 D5
U 1 1 615C3AC8
P 5500 1300
F 0 "D5" H 5500 1084 50  0000 C CNN
F 1 "1N4001" H 5500 1175 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 5500 1125 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 5500 1300 50  0001 C CNN
	1    5500 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 1650 5850 1650
Wire Wire Line
	5500 1450 5500 1650
Connection ~ 5850 1650
Wire Wire Line
	5850 1650 5850 1600
Wire Wire Line
	5500 1150 5500 900 
Wire Wire Line
	5500 900  5650 900 
Wire Wire Line
	5850 900  5850 1000
$Comp
L Relay:FINDER-32.21-x000 K3
U 1 1 615DDD3A
P 8200 1300
F 0 "K3" H 8630 1346 50  0000 L CNN
F 1 "FINDER-32.21-x000" H 8630 1255 50  0000 L CNN
F 2 "awrc:Relay_THT_UPS_80H-1C-S" H 9470 1270 50  0001 C CNN
F 3 "https://gfinder.findernet.com/assets/Series/355/S32EN.pdf" H 8200 1300 50  0001 C CNN
	1    8200 1300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 615DDD46
P 8000 2250
F 0 "#PWR07" H 8000 2000 50  0001 C CNN
F 1 "GND" H 8005 2077 50  0000 C CNN
F 2 "" H 8000 2250 50  0001 C CNN
F 3 "" H 8000 2250 50  0001 C CNN
	1    8000 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 2250 8000 2100
$Comp
L Device:R R8
U 1 1 615DDD4D
P 7450 1900
F 0 "R8" V 7657 1900 50  0000 C CNN
F 1 "100" V 7566 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7380 1900 50  0001 C CNN
F 3 "~" H 7450 1900 50  0001 C CNN
	1    7450 1900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 1900 7700 1900
Wire Wire Line
	8000 1700 8000 1650
$Comp
L Diode:1N4001 D7
U 1 1 615DDD55
P 7650 1300
F 0 "D7" H 7650 1084 50  0000 C CNN
F 1 "1N4001" H 7650 1175 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 7650 1125 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 7650 1300 50  0001 C CNN
	1    7650 1300
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 1650 8000 1650
Wire Wire Line
	7650 1450 7650 1650
Connection ~ 8000 1650
Wire Wire Line
	8000 1650 8000 1600
Wire Wire Line
	7650 900  7800 900 
Wire Wire Line
	8000 900  8000 1000
$Comp
L Relay:FINDER-32.21-x000 K2
U 1 1 615E3A68
P 6050 3700
F 0 "K2" H 6480 3746 50  0000 L CNN
F 1 "FINDER-32.21-x000" H 6480 3655 50  0000 L CNN
F 2 "awrc:Relay_THT_UPS_80H-1C-S" H 7320 3670 50  0001 C CNN
F 3 "https://gfinder.findernet.com/assets/Series/355/S32EN.pdf" H 6050 3700 50  0001 C CNN
	1    6050 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 615E3A74
P 5850 4650
F 0 "#PWR06" H 5850 4400 50  0001 C CNN
F 1 "GND" H 5855 4477 50  0000 C CNN
F 2 "" H 5850 4650 50  0001 C CNN
F 3 "" H 5850 4650 50  0001 C CNN
	1    5850 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5850 4650 5850 4500
$Comp
L Device:R R7
U 1 1 615E3A7B
P 5300 4300
F 0 "R7" V 5507 4300 50  0000 C CNN
F 1 "100" V 5416 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 5230 4300 50  0001 C CNN
F 3 "~" H 5300 4300 50  0001 C CNN
	1    5300 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5450 4300 5550 4300
Wire Wire Line
	5850 4100 5850 4050
$Comp
L Diode:1N4001 D6
U 1 1 615E3A83
P 5500 3700
F 0 "D6" H 5500 3484 50  0000 C CNN
F 1 "1N4001" H 5500 3575 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 5500 3525 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 5500 3700 50  0001 C CNN
	1    5500 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	5500 4050 5850 4050
Wire Wire Line
	5500 3850 5500 4050
Connection ~ 5850 4050
Wire Wire Line
	5850 4050 5850 4000
Wire Wire Line
	5500 3550 5500 3300
Wire Wire Line
	5500 3300 5850 3300
Wire Wire Line
	5850 3300 5850 3400
$Comp
L Relay:FINDER-32.21-x000 K4
U 1 1 615E97BC
P 8200 3700
F 0 "K4" H 8630 3746 50  0000 L CNN
F 1 "FINDER-32.21-x000" H 8630 3655 50  0000 L CNN
F 2 "awrc:Relay_THT_UPS_80H-1C-S" H 9470 3670 50  0001 C CNN
F 3 "https://gfinder.findernet.com/assets/Series/355/S32EN.pdf" H 8200 3700 50  0001 C CNN
	1    8200 3700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 615E97C8
P 8000 4650
F 0 "#PWR08" H 8000 4400 50  0001 C CNN
F 1 "GND" H 8005 4477 50  0000 C CNN
F 2 "" H 8000 4650 50  0001 C CNN
F 3 "" H 8000 4650 50  0001 C CNN
	1    8000 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 4650 8000 4500
$Comp
L Device:R R9
U 1 1 615E97CF
P 7450 4300
F 0 "R9" V 7657 4300 50  0000 C CNN
F 1 "100" V 7566 4300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 7380 4300 50  0001 C CNN
F 3 "~" H 7450 4300 50  0001 C CNN
	1    7450 4300
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 4300 7700 4300
Wire Wire Line
	8000 4100 8000 4050
$Comp
L Diode:1N4001 D8
U 1 1 615E97D7
P 7650 3700
F 0 "D8" H 7650 3484 50  0000 C CNN
F 1 "1N4001" H 7650 3575 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 7650 3525 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 7650 3700 50  0001 C CNN
	1    7650 3700
	0    1    1    0   
$EndComp
Wire Wire Line
	7650 4050 8000 4050
Wire Wire Line
	7650 3850 7650 4050
Connection ~ 8000 4050
Wire Wire Line
	8000 4050 8000 4000
Wire Wire Line
	7650 3550 7650 3300
Wire Wire Line
	7650 3300 8000 3300
Wire Wire Line
	8000 3300 8000 3400
Wire Wire Line
	7650 1150 7650 900 
Text Notes 6450 1650 0    50   ~ 0
RIGHT
Text Notes 8600 1650 0    50   ~ 0
LEFT
Text Notes 6450 4050 0    50   ~ 0
UP
Text Notes 8600 4050 0    50   ~ 0
DOWN
$Comp
L Connector_Generic:Conn_01x05 J4
U 1 1 6166018F
P 10400 2650
F 0 "J4" H 10480 2692 50  0000 L CNN
F 1 "AC Motors" H 10480 2601 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-5_P5.08mm" H 10400 2650 50  0001 C CNN
F 3 "~" H 10400 2650 50  0001 C CNN
	1    10400 2650
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J2
U 1 1 616BD71C
P 4400 6900
F 0 "J2" H 4480 6892 50  0000 L CNN
F 1 "Power, Sensors, Limits" H 4480 6801 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 4400 6900 50  0001 C CNN
F 3 "~" H 4400 6900 50  0001 C CNN
	1    4400 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 6700 4050 5000
Connection ~ 3750 5000
Wire Wire Line
	4200 6800 3950 6800
Wire Wire Line
	3950 6800 3950 4900
Wire Wire Line
	3950 4900 3500 4900
Connection ~ 3500 4900
Wire Wire Line
	8500 900  8500 1000
Wire Wire Line
	10200 2650 6350 2650
Wire Wire Line
	6350 2650 6350 3400
Wire Wire Line
	10200 2750 8500 2750
Wire Wire Line
	8500 2750 8500 3400
Wire Wire Line
	4050 5000 3750 5000
Wire Wire Line
	4200 6700 4050 6700
$Comp
L Device:R R2
U 1 1 618FA704
P 2750 7100
F 0 "R2" V 2543 7100 50  0000 C CNN
F 1 "1k" V 2634 7100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 2680 7100 50  0001 C CNN
F 3 "~" H 2750 7100 50  0001 C CNN
	1    2750 7100
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 618FA70A
P 3300 7200
F 0 "R3" V 3093 7200 50  0000 C CNN
F 1 "1k" V 3184 7200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3230 7200 50  0001 C CNN
F 3 "~" H 3300 7200 50  0001 C CNN
	1    3300 7200
	0    1    1    0   
$EndComp
Wire Wire Line
	1950 6400 2050 6400
Wire Wire Line
	2050 6550 2050 6400
Connection ~ 1850 6400
$Comp
L Connector_Generic:Conn_01x06 J3
U 1 1 6193B948
P 5050 6100
F 0 "J3" H 5130 6092 50  0000 L CNN
F 1 "Auxiliar" H 5130 6001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 5050 6100 50  0001 C CNN
F 3 "~" H 5050 6100 50  0001 C CNN
	1    5050 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 5150 2900 5150
Wire Wire Line
	2900 5250 3750 5250
Wire Wire Line
	3750 5250 3750 5900
Wire Wire Line
	4850 6100 4650 6100
Wire Wire Line
	3550 6100 3550 6400
Wire Wire Line
	3550 6400 3000 6400
Connection ~ 2150 6400
Wire Wire Line
	4850 6200 4150 6200
Wire Wire Line
	4150 6200 4150 3750
Wire Wire Line
	4150 3750 3750 3750
Connection ~ 3750 3750
Wire Wire Line
	4850 6300 3850 6300
Wire Wire Line
	3850 6300 3850 6600
Wire Wire Line
	4850 6400 3750 6400
Wire Wire Line
	3750 6400 3750 6800
Wire Wire Line
	1250 5650 1350 5650
$Comp
L Diode:1N4001 D1
U 1 1 619F0BE2
P 1150 1750
F 0 "D1" V 1196 1671 50  0000 R CNN
F 1 "1N4001" V 1105 1671 50  0000 R CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 1150 1575 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 1150 1750 50  0001 C CNN
	1    1150 1750
	0    1    1    0   
$EndComp
$Comp
L Diode:1N4001 D2
U 1 1 619FA92F
P 1400 1450
F 0 "D2" H 1400 1234 50  0000 C CNN
F 1 "1N4001" H 1400 1325 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 1400 1275 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 1400 1450 50  0001 C CNN
	1    1400 1450
	-1   0    0    1   
$EndComp
Wire Wire Line
	1150 1600 1150 1450
Wire Wire Line
	1150 1450 1250 1450
Wire Wire Line
	1550 1450 1700 1450
Wire Wire Line
	1700 1450 1700 1600
Wire Wire Line
	1150 1900 1150 2050
Wire Wire Line
	1150 2050 1250 2050
Wire Wire Line
	1550 2050 1700 2050
Wire Wire Line
	1700 2050 1700 1900
Connection ~ 1150 1450
Wire Wire Line
	950  2250 1700 2250
Wire Wire Line
	1700 2250 1700 2050
Connection ~ 1700 2050
Connection ~ 1150 2050
Wire Wire Line
	1700 1450 2100 1450
Wire Wire Line
	2100 1450 2100 1750
Connection ~ 1700 1450
Wire Wire Line
	2100 2050 2100 2950
Wire Wire Line
	2400 900  2400 1450
Wire Wire Line
	2400 1450 2100 1450
Connection ~ 2100 1450
Wire Wire Line
	2400 1450 2400 2300
Connection ~ 2400 1450
Wire Wire Line
	2400 900  2950 900 
$Comp
L power:GND #PWR01
U 1 1 61B319A0
P 1150 3050
F 0 "#PWR01" H 1150 2800 50  0001 C CNN
F 1 "GND" H 1155 2877 50  0000 C CNN
F 2 "" H 1150 3050 50  0001 C CNN
F 3 "" H 1150 3050 50  0001 C CNN
	1    1150 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 2050 1150 2950
Wire Wire Line
	950  850  1150 850 
Wire Wire Line
	1150 850  1150 1450
$Comp
L Device:R R1
U 1 1 61B920A4
P 950 1300
F 0 "R1" H 1020 1346 50  0000 L CNN
F 1 "10" H 1020 1255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 880 1300 50  0001 C CNN
F 3 "~" H 950 1300 50  0001 C CNN
	1    950  1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  950  950  1150
Wire Wire Line
	950  1450 950  2250
Text Label 9850 2850 0    50   ~ 0
AC_input
Wire Wire Line
	10200 2850 9850 2850
Text Label 9900 3350 0    50   ~ 0
AC_input
Text Label 8400 1750 0    50   ~ 0
AC_input
Wire Wire Line
	8400 1750 8400 1600
Text Label 6250 1750 0    50   ~ 0
AC_input
Wire Wire Line
	6250 1750 6250 1600
Text Label 8400 4150 0    50   ~ 0
AC_input
Wire Wire Line
	8400 4000 8400 4150
Text Label 6250 4150 0    50   ~ 0
AC_input
Wire Wire Line
	6250 4150 6250 4000
Wire Wire Line
	2900 5700 3100 5700
Wire Wire Line
	3100 5600 2900 5600
Wire Wire Line
	2900 5500 3100 5500
Wire Wire Line
	3100 5400 2900 5400
Text Label 4700 4300 0    50   ~ 0
Motor_Up
Wire Wire Line
	4700 4300 5150 4300
Text Label 3900 900  0    50   ~ 0
+12V
Wire Wire Line
	3900 900  3750 900 
Connection ~ 3750 900 
Text Label 5650 800  0    50   ~ 0
+12V
Wire Wire Line
	5650 800  5650 900 
Connection ~ 5650 900 
Wire Wire Line
	5650 900  5850 900 
Text Label 7800 800  0    50   ~ 0
+12V
Wire Wire Line
	7800 800  7800 900 
Connection ~ 7800 900 
Wire Wire Line
	7800 900  8000 900 
Wire Wire Line
	6800 2550 6800 950 
Wire Wire Line
	6800 2550 10200 2550
Wire Wire Line
	8500 900  9350 900 
Wire Wire Line
	9350 900  9350 2450
Wire Wire Line
	9350 2450 10200 2450
Text Label 7800 3200 0    50   ~ 0
+12V
Wire Wire Line
	7800 3200 7800 3300
Text Label 5650 3200 0    50   ~ 0
+12V
Wire Wire Line
	5650 3200 5650 3300
Wire Wire Line
	2400 2300 2950 2300
Wire Wire Line
	2100 2950 2700 2950
Text Label 4700 1900 0    50   ~ 0
Motor_Right
Wire Wire Line
	4700 1900 5150 1900
Text Label 6850 1900 0    50   ~ 0
Motor_Left
Wire Wire Line
	6850 1900 7300 1900
Text Label 6850 4300 0    50   ~ 0
Motor_Down
Wire Wire Line
	6850 4300 7300 4300
Text Label 3900 2300 0    50   ~ 0
+5V
Wire Wire Line
	3900 2300 3750 2300
Text Label 1950 3600 0    50   ~ 0
+5V
Wire Wire Line
	1950 3600 1800 3600
Wire Wire Line
	1800 3600 1800 4000
Text Label 3350 6600 0    50   ~ 0
+5V
Wire Wire Line
	3750 6800 1250 6800
Wire Wire Line
	1250 6800 1250 5650
$Comp
L Device:C_Small C6
U 1 1 61DA79EB
P 4450 5200
F 0 "C6" H 4542 5246 50  0000 L CNN
F 1 "10n" H 4542 5155 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W2.5mm_P5.00mm" H 4450 5200 50  0001 C CNN
F 3 "~" H 4450 5200 50  0001 C CNN
	1    4450 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C7
U 1 1 61DAB262
P 4900 5200
F 0 "C7" H 4992 5246 50  0000 L CNN
F 1 "10n" H 4992 5155 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W2.5mm_P5.00mm" H 4900 5200 50  0001 C CNN
F 3 "~" H 4900 5200 50  0001 C CNN
	1    4900 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 5000 4450 5000
Wire Wire Line
	4450 5000 4450 5100
Connection ~ 4050 5000
Wire Wire Line
	3950 4900 4900 4900
Wire Wire Line
	4900 4900 4900 5100
Connection ~ 3950 4900
Wire Wire Line
	4450 5300 4450 5400
Wire Wire Line
	4450 5400 4650 5400
Wire Wire Line
	4900 5400 4900 5300
$Comp
L Device:C_Small C1
U 1 1 61DDBD88
P 750 7250
F 0 "C1" H 842 7296 50  0000 L CNN
F 1 "10n" H 842 7205 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W2.5mm_P5.00mm" H 750 7250 50  0001 C CNN
F 3 "~" H 750 7250 50  0001 C CNN
	1    750  7250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 61DDBD8E
P 1000 7250
F 0 "C2" H 1092 7296 50  0000 L CNN
F 1 "10n" H 1092 7205 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D7.5mm_W2.5mm_P5.00mm" H 1000 7250 50  0001 C CNN
F 3 "~" H 1000 7250 50  0001 C CNN
	1    1000 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 5500 1350 5500
Wire Wire Line
	750  5400 1350 5400
Wire Wire Line
	1850 6400 1600 6400
Wire Wire Line
	1600 6400 1600 7650
Wire Wire Line
	1600 7650 1000 7650
Wire Wire Line
	1000 7650 1000 7350
Wire Wire Line
	750  7350 750  7650
Wire Wire Line
	750  7650 1000 7650
Connection ~ 1000 7650
$Comp
L Transistor_FET:IRF540N Q2
U 1 1 61F049DB
P 5750 4300
F 0 "Q2" H 5954 4346 50  0000 L CNN
F 1 "IRF540N" H 5954 4255 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 6000 4225 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 5750 4300 50  0001 L CNN
	1    5750 4300
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q4
U 1 1 61F09017
P 7900 4300
F 0 "Q4" H 8104 4346 50  0000 L CNN
F 1 "IRF540N" H 8104 4255 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8150 4225 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 7900 4300 50  0001 L CNN
	1    7900 4300
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q3
U 1 1 61F14BCA
P 7900 1900
F 0 "Q3" H 8104 1946 50  0000 L CNN
F 1 "IRF540N" H 8104 1855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8150 1825 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 7900 1900 50  0001 L CNN
	1    7900 1900
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:IRF540N Q1
U 1 1 61F1F01A
P 5750 1900
F 0 "Q1" H 5954 1946 50  0000 L CNN
F 1 "IRF540N" H 5954 1855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 6000 1825 50  0001 L CIN
F 3 "http://www.irf.com/product-info/datasheets/data/irf540n.pdf" H 5750 1900 50  0001 L CNN
	1    5750 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 3300 5850 3300
Connection ~ 7650 3300
Connection ~ 5850 3300
Wire Wire Line
	6350 950  6800 950 
Wire Wire Line
	6350 950  6350 1000
Wire Wire Line
	7650 900  5850 900 
Connection ~ 7650 900 
Connection ~ 5850 900 
Wire Wire Line
	5500 3300 4600 3300
Wire Wire Line
	4600 3300 4600 900 
Wire Wire Line
	4600 900  5500 900 
Connection ~ 5500 3300
Connection ~ 5500 900 
Wire Wire Line
	2100 2950 1300 2950
Connection ~ 2100 2950
Connection ~ 1150 2950
Wire Wire Line
	1150 2950 1150 3050
Wire Wire Line
	3250 1550 2700 1550
Wire Wire Line
	2700 1550 2700 2950
Connection ~ 3250 1550
Connection ~ 2700 2950
Wire Wire Line
	2700 2950 3250 2950
Wire Wire Line
	3250 2600 3250 2950
Connection ~ 4650 5400
Wire Wire Line
	4650 5400 4900 5400
Wire Wire Line
	4200 7000 3850 7000
Wire Wire Line
	3350 6600 3850 6600
Connection ~ 3850 6600
Wire Wire Line
	3850 6600 3850 7000
Wire Wire Line
	4200 7200 3450 7200
Wire Wire Line
	2900 7100 4200 7100
Wire Wire Line
	1000 5500 1000 7000
Wire Wire Line
	750  5400 750  6900
Wire Wire Line
	2600 7100 2200 7100
Wire Wire Line
	2200 7100 2200 6900
Wire Wire Line
	2200 6900 750  6900
Connection ~ 750  6900
Wire Wire Line
	750  6900 750  7150
Wire Wire Line
	3150 7200 2050 7200
Wire Wire Line
	2050 7200 2050 7000
Wire Wire Line
	2050 7000 1000 7000
Connection ~ 1000 7000
Wire Wire Line
	1000 7000 1000 7150
Wire Wire Line
	4200 6900 3000 6900
Wire Wire Line
	3000 6900 3000 6400
Connection ~ 3000 6400
Wire Wire Line
	3000 6400 2150 6400
Wire Wire Line
	4650 5400 4650 6100
Connection ~ 4650 6100
Wire Wire Line
	4650 6100 3550 6100
Wire Wire Line
	3850 6000 4850 6000
Wire Wire Line
	3850 5150 3850 6000
Wire Wire Line
	4850 5900 3750 5900
Text Label 2950 3600 0    50   ~ 0
+3.3v
Wire Wire Line
	2950 3600 2950 3750
Connection ~ 2950 3750
Wire Wire Line
	2950 3750 2400 3750
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 616C580D
P 1300 2850
F 0 "H1" H 1250 3050 50  0000 L CNN
F 1 "MountingHole_Pad" H 500 3150 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 1300 2850 50  0001 C CNN
F 3 "~" H 1300 2850 50  0001 C CNN
	1    1300 2850
	1    0    0    -1  
$EndComp
Connection ~ 1300 2950
Wire Wire Line
	1300 2950 1150 2950
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 616C7708
P 1450 2850
F 0 "H2" H 1400 3050 50  0000 L CNN
F 1 "MountingHole_Pad" H 500 3050 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 1450 2850 50  0001 C CNN
F 3 "~" H 1450 2850 50  0001 C CNN
	1    1450 2850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 616D3044
P 1600 2850
F 0 "H3" H 1550 3050 50  0000 L CNN
F 1 "MountingHole_Pad" H 500 2950 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 1600 2850 50  0001 C CNN
F 3 "~" H 1600 2850 50  0001 C CNN
	1    1600 2850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 616DEA4F
P 1750 2850
F 0 "H4" H 1700 3050 50  0000 L CNN
F 1 "MountingHole_Pad" H 500 2850 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 1750 2850 50  0001 C CNN
F 3 "~" H 1750 2850 50  0001 C CNN
	1    1750 2850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
