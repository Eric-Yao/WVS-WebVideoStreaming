# WVS: Web Video Streaming


## ServerCore
The core engine is developed from scratch by using C++ language. It is used to setup a web-based real-time video streaming server. It can be applied to different applications such as web-based cloud gaming, web-based mobile deep learning.

#### Project-1 

[Celtic-plus CONVINcE (Consumption OptimizatioN in VIdeo NEtworks)](https://convince.wp.tem-tsp.eu/home/)

The WVS engine is original developed from scratch within the Celtic-plus project CONVINcE. The first version of demo was developed by integrating with OpentStack, and was shown in the CONVINcE Plenary meeting on 18 January 2017, Lund, Sweden.

#### Project-2 

[EASE (Embedded Applications Software Engineering)](http://ease.cs.lth.se/)

The original WVS engine was further developed within the EASE project. The aim is to support the lightweight mobile to efficiently conduct the deep learning by offloading the part of complicated computing to the server.

## WebClient
The web-client is developed from scratch by using php language. It is used to connect with the ServerCore, and thus real-time streaming the video/image transmitted from the ServerCore side.

## MobileDL
The MobileDL is developed by using Java language and integrating with the [DL4j](https://deeplearning4j.org/android) library. The MobileDL is used to do the deep learning process at the mobile side. Such process is expected to be efficiently conducted with reference to the trade-off among energy consumption, computing accuracy and time.

