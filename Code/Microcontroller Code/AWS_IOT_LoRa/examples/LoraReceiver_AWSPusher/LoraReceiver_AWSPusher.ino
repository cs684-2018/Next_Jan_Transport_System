/*
 * Team Id: Next Jan Transport System 
 * Author List: Abhijit Divekar, AjayKumar Maurya, Punit Jain.
 * Filename: LoraReceiver_AWSPusher.ino
 * Theme: CS 684 Project 
 * Functions: setup(), loop(). 
 * This file receives GPS Data over LoRa, modifies it into json and publishes it over MQTT. 
 */


#include <AWS_IOT.h>
#include <WiFi.h>

AWS_IOT hornbill;

char WIFI_SSID[]="xxxxxxxxxxxxxxxxxxxx";
char WIFI_PASSWORD[]="xxxxxxxxxx";
char HOST_ADDRESS[]="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // In AWS-IOT core Goto settings->Endpoints . Copy the block given below endpoint and paste it here. 
char CLIENT_ID[]= "xxxxxxx";
char TOPIC_NAME[]= "xxxxxxxxxxx";

char newpacket_flag=0;
char LoRa_packet[255]="";
int LoRa_packet_size = 0;
char json_packet_array[200];
String json_packet="";

int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0;
char payload[255];
char rcvdPayload[255];
//LORA_init_start

#include <SPI.h>
#include <LoRa.h>
// Pin definetion of WIFI LoRa 32
// HelTec AutoMation 2017 support@heltec.cn 
#define SCK     5    // GPIO5  -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    27   // GPIO27 -- SX127x's MOSI
#define SS      18   // GPIO18 -- SX127x's CS
#define RST     14   // GPIO14 -- SX127x's RESET
#define DI0     26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    433E6  //you can set band here directly,e.g. 868E6,915E6
#define PABOOST true
// 
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}

//LORA_init_end

/*
*
* Function Name: <Function Name>
* Input: <Inputs (or Parameters) list with description if any>
* Output: <Return value with description if any>
* Logic: <Description of the function performed and the logic usedin the function>
* Example Call:<Example of how to call this function>
*
*/


void AWS_IOT_publish()
{
        sprintf(payload,"Hello from hornbill ESP32 : %d",msgCount++);
//        for (int count =0 ; count < LoRa_packet_size +1 ; count++)
//        {
//          payload[count] = LoRa_packet[count];
//        }
          if(hornbill.publish(TOPIC_NAME,payload) == 0)
        {        
            Serial.print("Publish Message:");
            Serial.println(payload);
        }
        else
        {
            Serial.println("Publish failed");
        }
}
void onReceive(int packetSize)
{
  // received a packet
  Serial.print("Received packet '");
  // read packet
  LoRa_packet_size = packetSize;
  for (int i = 0; i < packetSize; i++)
  {
    LoRa_packet[i] = (char)LoRa.read();
    Serial.print(LoRa_packet[i]);
  }
  newpacket_flag=1;
  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
  //AWS_IOT_publish();
}
void LORA_setup()
{
  while (!Serial);

  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND,PABOOST)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}
void publish_packet()
{
      //sprintf(payload,json_packet_array);
      if(hornbill.publish(TOPIC_NAME,json_packet_array) == 0)
      {        
          Serial.print("Publish Message:");
          Serial.println(json_packet_array);
      }
      else
      {
          Serial.println("Publish failed");
      }
}

void format_json()
{
  int count =0;
  int comma_indices[7];

  char time_value[15]="";
  char lat_value[15]="";
  char lon_value[15]="";
  char vel_value[15]="";
  char date_value[15]="";
  char busID_value[5]="";
  char devID_value[5]="";

  String time_json="";
  String lat_json="";
  String lon_json="";
  String vel_json="";
  String date_json="";
  String busID_json="";
  String devID_json="";

  for (int k=0; k<LoRa_packet_size+1 ; k++)
  {
    if(LoRa_packet[k]==',')
    {
      comma_indices[count] = k;
      count++;
      //Serial.print(k);
    }
  }
  
  for (int a= 0; a<comma_indices[0] ; a++)
  {
    time_value[a] = char(LoRa_packet[a]);
  }
  //Serial.println(time_value);

  for (int a=10; a<comma_indices[1] ; a++)
  {
    lat_value[a-10] = char(LoRa_packet[a]);
  }
  //Serial.println(lat_value);
  for (int a =21; a<comma_indices[2] ; a++)
  {
    lon_value[a-21] = char(LoRa_packet[a]);
  }
  //Serial.println(lon_value);
  for (int a =33; a<comma_indices[3] ; a++)
  {
    vel_value[a-33] = LoRa_packet[a];
  }
  //Serial.println(vel_value);
  for (int a =39; a<comma_indices[4] ; a++)
  {
    date_value[a-39] = LoRa_packet[a];
  }
  //Serial.println(date_value);
  for (int a =46; a<49 ; a++)
  {
    busID_value[a-46] = char(LoRa_packet[a]);
  }
  devID_value[0] = '0';
  devID_value[1] = '0';
  devID_value[2] = '0'; 
  devID_value[3] = '1';
  
  //Serial.println(busID_value);
  time_json = " \"Time\" : \"" + String(time_value) + "\" ,";
  Serial.println(time_json);

  lat_json = " \"Latitude\" : \"" + String(lat_value) + "\" ," ;
  Serial.println(lat_json);

  lon_json = " \"Longitude\" : \"" + String(lon_value) + "\" ," ;
  Serial.println(lon_json);

  vel_json = " \"Velocity\" : \"" + String(vel_value) + "\" ," ;
  Serial.println(vel_json);

  date_json = " \"Date\" : \"" + String(date_value) + "\" }" ;
  Serial.println(date_json);

  busID_json = "{ \"BusID\" : \"" + String(busID_value) + "\" ," ;
  Serial.println(busID_json);

  devID_json = " \"DevID\" : \"" + String(devID_value) + "\" ," ;
  
  json_packet = busID_json + devID_json + time_json + lat_json + lon_json + vel_json + date_json;
  Serial.println(json_packet);

  //
  //json_packet = busID_json + "," + time_json + "," + date_json + "," + lat_json + "," + lon_json + ",";
  //json_packet.concat(json_packet,"\{ \"BusID\"\: \"" + busID_json + " \"    \}";
  //sprintf(json_packet, "\{ \"BusID\"\: \"" + busID_json + " \"    \}" );
  
}
void setup() {
    Serial.begin(115200);
    delay(200);
    
    LORA_setup();
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfull");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(200);

}

void loop() {

    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
    
    if(newpacket_flag == 1)
    {
      format_json();
      json_packet.toCharArray(json_packet_array,200);
      publish_packet();
      newpacket_flag = 0;
      Serial.println("Packet Published Successfully !!!");
    }
    
    
//        for (int count =0 ; count < LoRa_packet_size +1 ; count++)
//        {
//          payload[count] = LoRa_packet[count];
//        }
//        }

}