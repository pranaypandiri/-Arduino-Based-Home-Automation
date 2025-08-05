#include <SoftwareSerial.h>
#include <dht.h>
dht DHT;

#define ldr 8
#define dht A0
#define soil 10

#define gas A1
#define alarm 12

#define sensorPower 7
#define sensorPin A2

#define echoPin 6
#define trigPin 5

#define relay1 3
#define relay2 4

int val = 0;
int  light;
int temp;
int humidity;
int distance;
int smoke;
int moisture;
int level;
int moisture_percentage;

float duration;
char data;

int flag1=0;
int flag2=0;

float arr[5];

void setup() {
  Serial.begin(9600);
  
  pinMode(ldr,INPUT);
  pinMode(dht,INPUT);

  pinMode(gas,INPUT);

  pinMode(alarm,OUTPUT);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);

  
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  pinMode(soil,INPUT);
  pinMode(sensorPower, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  light=digitalRead(ldr);
  distance=calculate_distance();
//  Serial.println(distance);
  //BLUETOOTH CODE
  if(Serial.available()>0){
    data=Serial.read();
    //   Serial.println(data); 
      if(data=='1'){
          flag1=1;
          digitalWrite(relay1,LOW);   
      }
      else if(data=='2'){
          flag1=0;
          digitalWrite(relay1,HIGH);
      }
      else if(data=='3'){
          flag2=1;
          digitalWrite(relay2,LOW);  
      
      }
      else if(data=='4'){ 
          flag2=0;
          digitalWrite(relay2,HIGH);
      }   
      else if(data=='0'){
          exit(0);  
      } 
      else if(data=='5'){
          digitalWrite(relay1,HIGH);
          digitalWrite(relay2,HIGH);      
      }

  }

  // ldr
  if (light == LOW) { // 0 means light 1 means dark
    if(flag1==0){
      digitalWrite(relay1, HIGH);
    }
  } 
  else {  
    digitalWrite(relay1, LOW);
  }

  // ultrasonic sensor
  if(distance<10){    
    digitalWrite(relay2, LOW);

  }
  else{
      if(flag2!=1){
        digitalWrite(relay2, HIGH);
      }
  }



  //DHT temp
  int chk = DHT.read11(dht);
  arr[0]=DHT.temperature;
  arr[1]=DHT.humidity;




//  Serial.print("Temperature is ");
//  Serial.print(arr[0]);
//  Serial.println(" C");
//  Serial.print("Humidity is " );
//  Serial.print(arr[1]);
//  Serial.println(" %");


  // mq6
  smoke=analogRead(gas);
  arr[2]=smoke;
  if(smoke>350){
    digitalWrite(alarm,HIGH);
  }
  else{
    digitalWrite(alarm,LOW);
  }

    // soil moisture
  moisture=analogRead(soil);
  moisture_percentage = (((moisture/1023.00)*100));
  arr[3]=moisture_percentage;


  
  // water level
  level = readSensor();
  //  Serial.println(level);
  arr[4]=level;
  if(level<190){
    digitalWrite(alarm,LOW);
  }
  else{
    digitalWrite(alarm,HIGH);    
  }
 

//
  for(int i=0;i<5;i++){
    Serial.println(arr[i]);
  
  }
  

  delay(1500);

}



int calculate_distance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  
  return distance;
}

int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // wait 10 milliseconds
  val = analogRead(sensorPin);    // Read the analog value form sensor
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // send current reading
}