@echo off
REM Running the test cases.
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\iter1-follows-source.txt ..\tests\SystemTesting\iter1-follows-queries.txt ..\tests\SystemTesting\output\iter1-follows.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\iter1-source.txt ..\tests\SystemTesting\iter1-queries.txt ..\tests\SystemTesting\output\iter1.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\1-Source.txt ..\tests\SystemTesting\1-Queries.txt ..\tests\SystemTesting\output\out1.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\2-Source.txt ..\tests\SystemTesting\2-Queries.txt ..\tests\SystemTesting\output\out2.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\3-Source.txt ..\tests\SystemTesting\3-Queries.txt ..\tests\SystemTesting\output\out3.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\4-Source.txt ..\tests\SystemTesting\4-Queries.txt ..\tests\SystemTesting\output\out4.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\5-Source.txt ..\tests\SystemTesting\5-Queries.txt ..\tests\SystemTesting\output\out5.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\6-Source.txt ..\tests\SystemTesting\6-Queries.txt ..\tests\SystemTesting\output\out6.xml
start /wait ..\Release\AutoTester.exe ..\tests\SystemTesting\7-Source.txt ..\tests\SystemTesting\7-Queries.txt ..\tests\SystemTesting\output\out7.xml
exit