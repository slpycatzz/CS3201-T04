@echo off

set runDir=..\Release\
set testDir=..\tests\SystemTesting\
set error="<failed>"

rem set target test directory
if not [%1]==[] set testDir=%1

start /wait %runDir%AutoTester.exe %testDir%iter1-follows-source.txt %testDir%iter1-follows-queries.txt %testDir%output\iter1-follows.xml
start /wait %runDir%AutoTester.exe %testDir%iter1-source.txt %testDir%iter1-queries.txt %testDir%output\iter1.xml
start /wait %runDir%AutoTester.exe %testDir%1-Source.txt %testDir%1-Queries.txt %testDir%output\out1.xml
start /wait %runDir%AutoTester.exe %testDir%2-Source.txt %testDir%2-Queries.txt %testDir%output\out2.xml
start /wait %runDir%AutoTester.exe %testDir%3-Source.txt %testDir%3-Queries.txt %testDir%output\out3.xml
start /wait %runDir%AutoTester.exe %testDir%4-Source.txt %testDir%4-Queries.txt %testDir%output\out4.xml
start /wait %runDir%AutoTester.exe %testDir%5-Source.txt %testDir%5-Queries.txt %testDir%output\out5.xml
start /wait %runDir%AutoTester.exe %testDir%6-Source.txt %testDir%6-Queries.txt %testDir%output\out6.xml
start /wait %runDir%AutoTester.exe %testDir%7-Source.txt %testDir%7-Queries.txt %testDir%output\out7.xml
start /wait %runDir%AutoTester.exe %testDir%8-Source.txt %testDir%8-Queries.txt %testDir%output\out8.xml
start /wait %runDir%AutoTester.exe %testDir%9-Source.txt %testDir%9-Queries.txt %testDir%output\out9.xml
start /wait %runDir%AutoTester.exe %testDir%10-Source.txt %testDir%10-Queries.txt %testDir%output\out10.xml
start /wait %runDir%AutoTester.exe %testDir%11-Source.txt %testDir%11-Queries.txt %testDir%output\out11.xml

setlocal enabledelayedexpansion

rem check if there is any error occurred.
for %%f in (%testDir%output\*.xml) do (
    rem output filename that have failed test cases.
    >nul findstr /C:%error% %%f && (
        echo Queries failed in %%~nf.xml
    ) || (
        echo All queries passed for %%~nf.xml
    )
)

echo All test cases completed^^!^^!
pause
