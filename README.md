# CSE687 Project 2: Map Reduce
May 12, 2022

Authors: Todd Hricik, Lyndsay LaBarge

## Overview
The Visual Studio Solution contains five seperate projects. We are using Visual Studio 2022.

The MapReduce project contains the implementation of the primary command line application.

The UnitTests project contains our Google Unit tests.

The Interfaces project contains out map and reduce interfaces, IMap and IReduce. These are template abstract classes. Concrete classes must include a map and reduce class 
method respectively.

The MapLibrary project contains our DLL library with our concrete Map class implementation. It follows the factory method pattern and contains a single function,
createMapper that returns a pointer to a concrete Map class object. The createMapper function takes single parameter, the output directory the object writes its results to.

The ReduceLibrary project contains our DLL library with our concrete Reduce class implementation. It follows the factory method pattern and contains a single function,
createReducer that returns a pointer to a concrete Reduce class object. The createReducer function takes single parameter, the output directory the object writes its results to.

We used C++ 17 and the boost C++ library (version 1.79). We used the boost Filesystem as opposed to implementating our own file management class.

## Build
All projects use a properties sheet. To build the projects, you will need to change the values of the user macros BoostRoot and BoostLib in MapReduce > PropertySheet.props, UnitTests > PropertySheet.props, Interfaces > PropertySheet.props, MapLibrary > PropertySheet.props, and ReduceLibrary > PropertySheet.props. By default they are set to `C:\Program Files\boost\boost_ 1_79_0` and `C:\Program Files\boost\boost_1_79_0\stage\lib`. To build, use either the Debug or Release configuration for x64 platforms.

## Usage
```
MapReduce.exe <path of dir containging input files> <path of dir for intermediate files> <path of dir for output files <path of map DLL> <path of reduce DLL>
```
All command line requirements are required. 

The input directory must contain UTF-8 encoded text files. If the input directory contains a file that does not meet the requirements, that file will be skipped during processing.

If the intermediate directory or the output directory does not already exist, the application will create the directories.

## Expected Output
### Command
```
.\MapReduce.exe "C:\Users\llaba\source\repos\CSE687OOD_Project2\MapReduce\shakespeare" "C:\Users\llaba\source\repos\CSE687OOD_Project2\MapReduce\temp" "C:\Users\llaba\source\repos\CSE687OOD_Project2\MapReduce\output" "C:\Users\llaba\source\repos\CSE687OOD_Project2\x64\Release\MapLibrary.dll" "C:\Users\llaba\source\repos\CSE687OOD_Project2\x64\Release\ReduceLibrary.dll"
```
### Command Line Output 
```
[2022-05-11 17:02:14.630203] [0x00006730] [info]    targetDir member has been set in Workflow constructor.
[2022-05-11 17:02:14.631203] [0x00006730] [info]    Info in Workflow constructor: intermediateDir member has been set in Workflow constructor.
[2022-05-11 17:02:14.631704] [0x00006730] [info]    Info in Workflow constructor: Directory for intermediate files created.
[2022-05-11 17:02:14.632204] [0x00006730] [info]    Info in Workflow constructor: outDir member has been set in Workflow constructor.
[2022-05-11 17:02:14.632204] [0x00006730] [info]    Info in Workflow constructor: Directory for output files created.
[2022-05-11 17:02:14.633704] [0x00006730] [info]    Info in Workflow constructor: Map DLL located.
[2022-05-11 17:02:14.634704] [0x00006730] [info]    Info in Workflow constructor: Reduce DLL located.
[2022-05-11 17:02:14.695715] [0x00006730] [info]    Info in Workflow run(): Map process completed for All'sWellThatEndsWell.txt
[2022-05-11 17:02:14.736622] [0x00006730] [info]    Info in Workflow run(): Map process completed for AMidSummerNightsDream.txt
[2022-05-11 17:02:14.787130] [0x00006730] [info]    Info in Workflow run(): Map process completed for AsYouLIkeIte.txt
[2022-05-11 17:02:14.850641] [0x00006730] [info]    Info in Workflow run(): Map process completed for Cymbeline.txt
[2022-05-11 17:02:14.902150] [0x00006730] [info]    Info in Workflow run(): Map process completed for Love'sLabourLost.txt
[2022-05-11 17:02:14.953228] [0x00006730] [info]    Info in Workflow run(): Map process completed for MeasureForMeasure.txt
[2022-05-11 17:02:15.003239] [0x00006730] [info]    Info in Workflow run(): Map process completed for MuchAdoAboutNothing.txt
[2022-05-11 17:02:15.050247] [0x00006730] [info]    Info in Workflow run(): Map process completed for PericlesPrinceOfTyre.txt
[2022-05-11 17:02:15.099236] [0x00006730] [info]    Info in Workflow run(): Map process completed for TamingOfTheShrew.txt
[2022-05-11 17:02:15.135372] [0x00006730] [info]    Info in Workflow run(): Map process completed for TheComedyOfErrors.txt
[2022-05-11 17:02:15.184014] [0x00006730] [info]    Info in Workflow run(): Map process completed for TheMerchantOfVenice.txt
[2022-05-11 17:02:15.240524] [0x00006730] [info]    Info in Workflow run(): Map process completed for TheMerryWivesOfWindsor.txt
[2022-05-11 17:02:15.279531] [0x00006730] [info]    Info in Workflow run(): Map process completed for TheTempest.txt
[2022-05-11 17:02:15.327539] [0x00006730] [info]    Info in Workflow run(): Map process completed for TheTwelthNight.txt
[2022-05-11 17:02:15.371652] [0x00006730] [info]    Info in Workflow run(): Map process completed for TheTwoGentlemenOfVerona.txt
[2022-05-11 17:02:15.433160] [0x00006730] [info]    Info in Workflow run(): Map process completed for TroilusAndCressida .txt
[2022-05-11 17:02:15.490527] [0x00006730] [info]    Info in Workflow run(): Map process completed for Winter'sTale.txt
[2022-05-11 17:02:15.491027] [0x00006730] [info]    Running sort on All'sWellThatEndsWell.txt
[2022-05-11 17:02:15.503530] [0x00006730] [info]    Running sort on AMidSummerNightsDream.txt
[2022-05-11 17:02:15.512031] [0x00006730] [info]    Running sort on AsYouLIkeIte.txt
[2022-05-11 17:02:15.523533] [0x00006730] [info]    Running sort on Cymbeline.txt
[2022-05-11 17:02:15.538035] [0x00006730] [info]    Running sort on Love'sLabourLost.txt
[2022-05-11 17:02:15.549537] [0x00006730] [info]    Running sort on MeasureForMeasure.txt
[2022-05-11 17:02:15.561039] [0x00006730] [info]    Running sort on MuchAdoAboutNothing.txt
[2022-05-11 17:02:15.572542] [0x00006730] [info]    Running sort on PericlesPrinceOfTyre.txt
[2022-05-11 17:02:15.582043] [0x00006730] [info]    Running sort on TamingOfTheShrew.txt
[2022-05-11 17:02:15.593546] [0x00006730] [info]    Running sort on TheComedyOfErrors.txt
[2022-05-11 17:02:15.601547] [0x00006730] [info]    Running sort on TheMerchantOfVenice.txt
[2022-05-11 17:02:15.613048] [0x00006730] [info]    Running sort on TheMerryWivesOfWindsor.txt
[2022-05-11 17:02:15.625050] [0x00006730] [info]    Running sort on TheTempest.txt
[2022-05-11 17:02:15.633552] [0x00006730] [info]    Running sort on TheTwelthNight.txt
[2022-05-11 17:02:15.644554] [0x00006730] [info]    Running sort on TheTwoGentlemenOfVerona.txt
[2022-05-11 17:02:15.653556] [0x00006730] [info]    Running sort on TroilusAndCressida .txt
[2022-05-11 17:02:15.667058] [0x00006730] [info]    Running sort on Winter'sTale.txt
[2022-05-11 17:02:15.680560] [0x00006730] [info]    Running reduce operation...
[2022-05-11 17:02:20.073196] [0x00006730] [info]    Writing success file...
[2022-05-11 17:02:20.073697] [0x00006730] [info]    Removing intermediate files...
[2022-05-11 17:02:20.080198] [0x00006730] [info]    Map reduce process complete.
```

### File Output
Assuming the application completed with no errors, the results of the map reduce operation are located in the specified output directory. It will output 2 files.

`reduce.txt` - file containing the map reduce output

`SUCCESS` - file indicating the operation completed successfully


## Unit Test Results
The unit tests are defined in UnitTests\tests.cpp. There are unit tests for the Map, Sort, Reduce, and Workflow calsses. 

```
[==========] Running 12 tests from 4 test cases.
[----------] Global test environment set-up.
[----------] 3 tests from mapTest
[ RUN      ] mapTest.checkOutputPath
[       OK ] mapTest.checkOutputPath (1 ms)
[ RUN      ] mapTest.checkMapResult
[       OK ] mapTest.checkMapResult (1 ms)
[ RUN      ] mapTest.badOutputDirectory
[       OK ] mapTest.badOutputDirectory (0 ms)
[----------] 3 tests from mapTest (3 ms total)

[----------] 3 tests from sortTest
[ RUN      ] sortTest.checkSortOutput
[       OK ] sortTest.checkSortOutput (0 ms)
[ RUN      ] sortTest.badInputFilePath
[       OK ] sortTest.badInputFilePath (0 ms)
[ RUN      ] sortTest.badInputFileFormat
[       OK ] sortTest.badInputFileFormat (1 ms)
[----------] 3 tests from sortTest (2 ms total)

[----------] 4 tests from reduceTest
[ RUN      ] reduceTest.checkOutputDirectory
[       OK ] reduceTest.checkOutputDirectory (1 ms)
[ RUN      ] reduceTest.checkOutputPath
[       OK ] reduceTest.checkOutputPath (1 ms)
[ RUN      ] reduceTest.checkBadDirectory
[       OK ] reduceTest.checkBadDirectory (0 ms)
[ RUN      ] reduceTest.checkReduceOutput
[       OK ] reduceTest.checkReduceOutput (4 ms)
[----------] 4 tests from reduceTest (6 ms total)

[----------] 2 tests from WorkflowTest
[ RUN      ] WorkflowTest.testConstructor
[2022-05-11 17:04:29.398128] [0x00002ef8] [debug]   Debug in Workflow constructor: Entering constructor.
[2022-05-11 17:04:29.398628] [0x00002ef8] [info]    targetDir member has been set in Workflow constructor.
[2022-05-11 17:04:29.399129] [0x00002ef8] [info]    Info in Workflow constructor: intermediateDir member has been set in Workflow constructor.
[2022-05-11 17:04:29.401131] [0x00002ef8] [info]    Info in Workflow constructor: Directory for intermediate files created.
[2022-05-11 17:04:29.401629] [0x00002ef8] [info]    Info in Workflow constructor: outDir member has been set in Workflow constructor.
[2022-05-11 17:04:29.402129] [0x00002ef8] [info]    Info in Workflow constructor: Directory for output files created.
[2022-05-11 17:04:29.405130] [0x00002ef8] [info]    Info in Workflow constructor: Map DLL located.
[2022-05-11 17:04:29.407630] [0x00002ef8] [info]    Info in Workflow constructor: Reduce DLL located.
[       OK ] WorkflowTest.testConstructor (10 ms)
[ RUN      ] WorkflowTest.testRun
[2022-05-11 17:04:29.408631] [0x00002ef8] [debug]   Debug in Workflow constructor: Entering constructor.
[2022-05-11 17:04:29.409131] [0x00002ef8] [info]    targetDir member has been set in Workflow constructor.
[2022-05-11 17:04:29.409630] [0x00002ef8] [info]    Info in Workflow constructor: intermediateDir member has been set in Workflow constructor.
[2022-05-11 17:04:29.410131] [0x00002ef8] [info]    Info in Workflow constructor: Directory for intermediate files created.
[2022-05-11 17:04:29.410631] [0x00002ef8] [info]    Info in Workflow constructor: outDir member has been set in Workflow constructor.
[2022-05-11 17:04:29.411130] [0x00002ef8] [info]    Info in Workflow constructor: Directory for output files created.
[2022-05-11 17:04:29.414131] [0x00002ef8] [info]    Info in Workflow constructor: Map DLL located.
[2022-05-11 17:04:29.416631] [0x00002ef8] [info]    Info in Workflow constructor: Reduce DLL located.
[2022-05-11 17:04:29.456138] [0x00002ef8] [info]    Info in Workflow run(): Map process completed for All'sWellThatEndsWell.txt
[2022-05-11 17:04:29.457138] [0x00002ef8] [info]    Running sort on All'sWellThatEndsWell.txt
[2022-05-11 17:04:29.469140] [0x00002ef8] [info]    Running reduce operation...
[2022-05-11 17:04:30.575334] [0x00002ef8] [info]    Writing success file...
[2022-05-11 17:04:30.576334] [0x00002ef8] [info]    Removing intermediate files...
[2022-05-11 17:04:30.577333] [0x00002ef8] [info]    Map reduce process complete.
[       OK ] WorkflowTest.testRun (1171 ms)
[----------] 2 tests from WorkflowTest (1181 ms total)

[----------] Global test environment tear-down
[==========] 12 tests from 4 test cases ran. (1195 ms total)
[  PASSED  ] 12 tests.
```