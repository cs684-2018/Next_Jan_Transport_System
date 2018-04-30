import serial 
import numpy  
import matplotlib.pyplot as plt 
from drawnow import drawnow
import time
X_data= []
Y_data=[]
#serData.close()
serData = serial.Serial('COM65', 115200,timeout=1) 
horizontalData=[0,0]
verticalData=[0,0]
plt.ion() 

#plt.title('Joystick Reading')     
#plt.grid(True)
#plt.ylabel('Vertical ADC')
#plt.xlabel('Horizontal ADC')                           
 
def makeFig(): 
   plt.ylim(0,4096) 
   plt.xlim(0,4096)
   plt.plot(horizontalData[-1],verticalData[-1], 'ro-', )      
   
 
while True: 
    while (serData.inWaiting()==0): 
        pass #do nothing
    serString = serData.readline()
    serString = serString.decode("utf-8")
    print(type(serString))
    serString = serString.split(',')
    
#    print(serString[0])
#    print(int(serString[0][4]+serString[0][3]+serString[0][2]+serString[0][1]))
#    print(serString[1])
#    print(int(serString[1][4]+serString[1][3]+serString[1][2]+serString[1][1]))
    try:
        verticalData.append(int(serString[0][4]+serString[0][3]+serString[0][2]+serString[0][1]))
        horizontalData.append(4096 - int(serString[1][4]+serString[1][3]+serString[1][2]+serString[1][1]))
        drawnow.drawnow(makeFig)
        plt.pause(.0001)
        serData.flushInput()
    except IndexError: 
        serData.flushInput()
        pass
   # if ((horizontalData[-1]-horizontalData[-2] > 5) or (verticalData[-1]-verticalData[-2] > 5)):
    
#    print(horizontalData[-1])
#    print(verticalData[-1])
#    else:
#        pass
    
    #drawnow.drawnow(makeFig)
                                            
    
    
    