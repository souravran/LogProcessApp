Process Log Data Implementation
================================
:Author: Sourav Senapati
:Date:   March 13th, 2016

-----------------

### Task

The task is to read the given data.log fine and process the data and write the result into an xml file. The sample log data is given below:

sel:6,ccd1ca23­e502­4ad6­9617­269b7d40cc4c,Banner­-71,6.449
sel:6,b1324a53­6915­47bf­8ee6­46e9ace57fc9,Banner­-71,22.221
cnt:1,ccd1ca23­e502­4ad6­9617­269b7d40cc4c
cnt:114,ccd1ca23­e502­4ad6­9617­269b7d40cc4c
cnt:114,b1324a53­6915­47bf­8ee6­46e9ace57fc9

So above data is about a Banner with ID "Banner-71". Every row has a row ID, then a key value. Task is to output the cost each banner, if that banner is displayed. The information about if the banner is displayed is given by the row ID is "1" (eg- cnt:1). As in the abaove case Banner-71 has two different key values but one is displayed with price 6.449, because this particual banner has row ID 1. Hence the output is shown below:

    <Banner id="Banner­-71"  revenues="6.449">
        <Events>
            <Event id="1">1</event>
            <Event id="114">2</event>
            <Event id="6">2</event>
         </Events>
    </Banner>
 

### Compile 

This is a CMake project. CMake takes a CMakeList file, and outputs it to a platform-specific build format. If in Linux system, create a build directory, preferably parallel to source directory and go to the buld directory. Type the following command:

```sh
$ mkdir build_x86-64
$ cd build_x86-64
```
Then run cmake and point to the source (src) directory. Type the followiing command:
```sh
$ cmake ../src
```
And finally run make in the build directory to generate the executables with the following command:
```sh
$ make
```
The executables are found under application folder.
If Eclipse is your favorite editor, then a small script is being provided for making an Eclipse build project. Run the following command in the source folder:
```sh
$ ./mk-eclipse-build-project.sh
```
### Run

To run the application, go to the build directory and supply the data file, type the follwing:
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
