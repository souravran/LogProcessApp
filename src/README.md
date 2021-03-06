Process Log Data Implementation
================================
:Author: Sourav Senapati
:Date:   March 13th, 2016

-----------------

### Task

The task is to read the given data.log file, process the data and write the results into a xml file. The data.log file has comma separated values and format of the log data is not changed. The sample log data is given below:
```sh
sel:6,ccd1ca23­e502­4ad6­9617­269b7d40cc4c,Banner­-71,6.449
sel:6,b1324a53­6915­47bf­8ee6­46e9ace57fc9,Banner­-71,22.221
cnt:1,ccd1ca23­e502­4ad6­9617­269b7d40cc4c
cnt:114,ccd1ca23­e502­4ad6­9617­269b7d40cc4c
cnt:114,b1324a53­6915­47bf­8ee6­46e9ace57fc9
```
Above log data is from an application that handles banner ads in cellphones. Given a banner with ID "Banner-71". Every row has an event ID (eg.- sel:16, cnt:1, so on), then a key value. Task is to output the revenue generated by each banner, if that banner has a display event. The information about display event of any banner is given by the row ID "1" (eg.- cnt:1). As in the abaove case Banner-71 has  events with different key values but one event is displayed with price 6.449, because this particular banner has event ID 1. Hence the output is shown below:

    <Banner id="Banner­-71"  revenues="6.449">
        <Events>
            <Event id="1">1</event>
            <Event id="114">2</event>
            <Event id="6">2</event>
         </Events>
    </Banner>
 

### Compile 

This is a CMake project. CMake takes a CMakeList file, and outputs to a platform-specific build format. If in Linux system, create a build directory, preferably parallel to source directory (src) and go to the buld directory. To achieve that, type the following command:

```sh
$ mkdir build_x86-64
$ cd build_x86-64
```
Then run cmake and point to the source directory. Type the followiing command:
```sh
$ cmake ../src
```
And finally run make in the build directory to generate the executables, the executables are found under application folder. Type with the following command:
```sh
$ make
```
ECLIPSE EDITOR : If Eclipse is your favorite editor, then a small script is being provided for making an Eclipse build project. Run the following command in the source folder while providing the arguments:
```sh
$ ./mk-eclipse-build-project.sh SRC_DIR BUILD_DIR BUILD_TYPE
```
### Run

To run the application, go to the build directory and supply the data log file, type the follwing:
```sh
$ ./application/LogProcessorApp data.log
```
The results are written into the output.xml file.

### Document generation

Support for Doxygen document generation system has been supplied with the project. A CMake file is
provided that generates required config file in the build directory. After successfully building the project, go to the build directory and type:
```sh
$ doxygen Doxyfile
```
This generates Doxygen docuementation inside doxygen folder for the current project. The document generation is both in HTML and Latex. In case the doxygen doc is not required, please comment out the follwing line in 
CMakeLists.txt file unders source folder:

include(cmake/doxygen.cmake)

