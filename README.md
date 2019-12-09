# IoT-based-Health-System-for-Mountaineers-and-Soldiers
This project envisions on designing an IoT based dynamic health surveilling system which continuously monitors the health statistics of soldiers/mountaineers and notifying the base camp if changes are observed. GSM as a wireless technology is used for data transmission from the soldiers/mountaineers to the base units. The system also employs a high precision GPS unit for simultaneously tracking the coordinates.

To achieve high accuracy in our results, we rely on three vital signs precisely body temperature, pulse rate and pulse oximetry. Significant rise or drop in body temperature shows signs of unhealthy conditions and considering body temperature as our primary vital sign, we can achieve better results. The most commonly read vital sign, pulse can be used to detect a wide range of emergency conditions, such as cardiac arrest, pulmonary embolisms, and vasovagal syncope. In addition, pulse oximetry measures the level of oxygen in the blood which serves as an indicator of respiratory function and can aid in diagnostics of conditions such as hypoxia (low oxygen reaching the body’s tissues). As such, pulse oximetry is a valuable addition to a general health monitoring system. On the other hand, GPS module aid in supporting our predictions. Suppose if our system outputs Hyperthermia and the position coordinates are somewhere near high humid or desert combat areas then we will be very sure about our medical predictions.

## Method of Action
### At Soldier
![Image](https://github.com/AjithKumarVS/IoT-based-Health-System-for-Mountaineers-and-Soldiers/blob/master/C1.png?raw=true)
The biomedical sensors attached to the soldier body will fetch the health statistics such as body temperature,pulse rate and oxygen concentration in the blood. This is stored in the microcontroller memory which is further communicated to the base camp via GSM.   
  
### At Base Camp   
![Image](https://github.com/AjithKumarVS/IoT-based-Health-System-for-Mountaineers-and-Soldiers/blob/master/C2.png?raw=true)
This unit receives the information about the soldier and displays the value of each sensor as well as the GPS location. It also sends an alert message with the medical conditions.