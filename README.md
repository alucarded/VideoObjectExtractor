# VideoObjectExtractor
==============
Automatic marking of silhouettes of moving objects in video sequences

Pre-requisites:
--------------
- OpenCV library

On Linux (tested on Ubuntu 14.04):
--------------
1. In main project directory, in command line type:
 - mkdir build
 - cd build
 - cmake ..
 - make

2. Execute prepare_camera.sh to configure camera with v4l2-ctl tool

3. Make sure you have camera connected and then run the binary file.

On Windows:
--------------
1. Prepare project/workspace with CMake for Windows http://www.cmake.org/download/
2. Compile

In action:
--------------
https://drive.google.com/file/d/0B54bH06GzxIHS1dMMF9RLUVlbU0/view?usp=sharing

Related paper:
--------------
Posłuszny T., Putz B. (2016) An Improved Extraction Process of Moving Objects’ Silhouettes in Video Sequences. In: Jabłoński R., Brezina T. (eds) Advanced Mechatronics Solutions. Advances in Intelligent Systems and Computing, vol 393. Springer, Cham

https://link.springer.com/chapter/10.1007/978-3-319-23923-1_9
