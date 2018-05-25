@echo on
init.bat && arm-linux-androideabi-g++.exe  main.cpp -llog -fPIE -pie
