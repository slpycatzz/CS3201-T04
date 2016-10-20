@echo off
REM Running the SIMPLE_Invalids batch
start /wait ..\Release\AutoTester.exe SystemTesting\iter1-follows-source.txt SystemTesting\iter1-follows-queries.txt SystemTesting\output\iter1-follows.xml
start /wait ..\Release\AutoTester.exe SystemTesting\iter1-source.txt SystemTesting\iter1-queries.txt SystemTesting\output\iter1.xml
start /wait ..\Release\AutoTester.exe SystemTesting\1-Source.txt SystemTesting\1-Queries.txt SystemTesting\output\out1.xml
start /wait ..\Release\AutoTester.exe SystemTesting\2-Source.txt SystemTesting\2-Queries.txt SystemTesting\output\out2.xml
start /wait ..\Release\AutoTester.exe SystemTesting\3-Source.txt SystemTesting\3-Queries.txt SystemTesting\output\out3.xml
start /wait ..\Release\AutoTester.exe SystemTesting\4-Source.txt SystemTesting\4-Queries.txt SystemTesting\output\out4.xml
start /wait ..\Release\AutoTester.exe SystemTesting\5-Source.txt SystemTesting\5-Queries.txt SystemTesting\output\out5.xml
start /wait ..\Release\AutoTester.exe SystemTesting\6-Source.txt SystemTesting\6-Queries.txt SystemTesting\output\out6.xml
start /wait ..\Release\AutoTester.exe SystemTesting\7-Source.txt SystemTesting\7-Queries.txt SystemTesting\output\out7.xml
exit