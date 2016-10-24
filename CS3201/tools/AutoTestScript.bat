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
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Calls.txt %testDir%clauseTesting\output\Calls.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\CallsStar.txt %testDir%clauseTesting\output\CallsStar.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Custom.txt %testDir%clauseTesting\output\Custom.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Follows.txt %testDir%clauseTesting\output\Follows.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\FollowsStar.txt %testDir%clauseTesting\output\FollowsStar.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Modifies.txt %testDir%clauseTesting\output\Modifies.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Next.txt %testDir%clauseTesting\output\Next.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\NextStar.txt %testDir%clauseTesting\output\NextStar.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Parent.txt %testDir%clauseTesting\output\Parent.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\ParentStar.txt %testDir%clauseTesting\output\ParentStar.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Pattern.txt %testDir%clauseTesting\output\Pattern.xml
start /wait %runDir%AutoTester.exe %testDir%clauseTesting\SourceOne.txt %testDir%clauseTesting\Uses.txt %testDir%clauseTesting\output\Uses.xml

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

for %%f in (%testDir%clauseTesting\output\*.xml) do (
    rem output filename that have failed test cases.
    >nul findstr /C:%error% %%f && (
        echo Queries failed in %%~nf.xml
    ) || (
        echo All queries passed for %%~nf.xml
    )
)

echo All test cases completed^^!^^!
pause
