# AutoPump-publication-repository
Code for "Autonomous underwater pumping system"

Code from the Publication:
Mucciarone, David A., Hans B. DeJong, and Robert B. Dunbar. "Autonomous underwater pumping system." HardwareX (2020): e00154.
(https://www.sciencedirect.com/science/article/pii/S2468067220300638)

The code was written by H. DeJong and D. Mucciarone. There are 3 Arduino programs needed to run the AutoPump.

1. Program "time_set_manually" is used to set the correct time on the real-time clock. 

2. Program "current_time" is used after the "time_set_manually" to verify the real-time clock has the correct time. It can also be used after the deployment to check for any clock drift. The time and date will display in the Arduino serial reader.

3. Program "AutoPump_main_RTC" is the main program for the AutoPump. The real-time clock must have the correct time and date before using this program. 

