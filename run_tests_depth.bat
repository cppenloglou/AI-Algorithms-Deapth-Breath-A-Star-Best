@echo off

REM Set the path to your C program
set PROGRAM=registerIcs22116.exe

REM Set the number of tests for each method
set NUM_TESTS=10

REM Loop for each method
for %%M in ("deapth") do (
  REM Loop for each test
  for /l %%i in (4, 5, 100) do (
    for /l %%t in (2, 11, 100) do (

        REM Run the program with the specified method, random numbers as strings, and write the output to a file
        call %PROGRAM% %%M %%i %%t "solutionToExcel.txt"

        REM Optionally, you can print the return code of the program
        echo Test %%M_%%i: Return Code - !ERRORLEVEL!
    ) 
  )
)
