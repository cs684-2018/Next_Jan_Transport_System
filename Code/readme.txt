2018 - CS684  Group X : Project Next Jan Transport System (NJTS)
================================================================= 
 
Group Info: 
------------ 
+   **Abhijit Divekar (163079036)** 
+   **AjayKumar Maurya (16307R009)**
+   **Punit Jain (17307R016)**
 
 
Extension Of 
------------ 
 
No, this project is not an extension of any previous project.  
 
Project Description 
====================

In a Metropolitan city like Mumbai, majority of population depends on public transport i.e. Local Trains and Buses. 
Out of the two, buses are the one that reaches each and every corner of the city. 
But, buses are very unpredictable and in case if there is any delay/cancellation, people are not informed, 
which leads to formation of large queues at the Bus stop and people are then forced to take other expensive means of transport. 
**Next Jan Transport System (NJTS)** uses **GPS** and **EspLoRa32 network** to locate the exact position of buses and Google API's to calculate the **Estimated Time of Arrival(ETA)**.
The Information regarding location of buses with their respective **Bus IDs** and **timestamp** is stored at **AWS-DynamoDB Database**.
This data from DynamoDB can be accessed by the webpage in which location of bus-stops(currently hard-coded) can be used to calculate ETA using Google Maps API. 
This implementation will help the public to efficiently manage their time and money, and also it will improve the standards of the service providers which would increase their number of users.
 
Technologies Used 
------------------- 
 
+   Embedded C 
+   Esp32 (Wifi+LoRa -433 MHz)
+   NEO-6M GPS Module
+   AWS-IOT
+   JavaScript
+   HTML    
 
 
Installation Instructions 
=========================

+  [Software Installation Video](https://youtu.be/M7_jYx-QBrk)

Software Links
--------------- 
+   [Arduino](https://www.arduino.cc/en/Guide/Windows)
+   [ESP32 Libraries](https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series)
+   [Additional Esp32 Libraries for AWS support](https://github.com/ExploreEmbedded/Hornbill-Examples/tree/master/arduino-esp32/AWS_IOT)

A Chrome/Firefox browser is required to run .html files of project. No Additional Software is Required for that.
   
Demonstration Video 
=========================  

+  [Demonstration Video](https://www.youtube.com/watch?v=KuoGJK82j-I)

References 
=========== 
  
+ [Registering Thing with AWS-IOT and MQTT pub-sub](https://www.exploreembedded.com/wiki/Secure_IOT_with_AWS_and_Hornbill_ESP32)
+ [AWS-IOT Tutorials](https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html)
+ [AWS-DynamoDB Tutorials](https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/GettingStarted.html)
+ [Google Maps API](https://developers.google.com/maps/)
