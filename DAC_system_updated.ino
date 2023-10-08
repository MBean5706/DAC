/* 
This program is designed for the DAC system. It uses the SCD30 sensor to detect temp,
and CO2. The values registered by the sensor will determine when the 
valves will turn on or off (open or closed).

Created by: Marion Bean
*/

//--Variables and databases--------------------------------------------------------
#include <Adafruit_SCD30.h>

Adafruit_SCD30 scd30;

double tempRead = 0.0,
       humidityRead = 0.0,
       co2Read = 0.0;

int valvePin1 = 8;
int valvePin2 = 9;

int timeDelay = 2000;

int pressurePin1 = A0;
int pressureRead1 = 0;
int pressureValue1 = 0;

int pressurePin2 = A1;
int pressureRead2 = 0;
int pressureValue2 = 0;

double pressureMin = 102.4;  //0 psi = 0.5v
double pressureMax = 921.6;  //150 psi = 4.5v
double pressurePSImax = 150.0;

//---SETUP--------------------------------------------------------------------------
void setup() 
{
  Serial.begin(115200);

  //this tests if the analog input works
  //pinMode(pressurePin, INPUT_PULLUP);  
  
  //Initailzes the valve pins as output.
  pinMode(valvePin1, OUTPUT);   
  pinMode(valvePin2, OUTPUT);

  //Starts the SCD30 sensor.
  if (!scd30.begin()) 
  {
    Serial.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  Serial.println("SCD30 Found!");

  Serial.print("Measurement Interval: ");
  Serial.print(scd30.getMeasurementInterval());
  Serial.println(" seconds");

  if (scd30.dataReady()) 
  {
    Serial.println("Data available!");
  } else {
    Serial.println("Data not available!");
  }
}


/*--------------MAIN----------------------------------------------*/
void loop() 
{
  Serial.println("");
  if (!scd30.read()) 
  {
    Serial.println("Error reading sensor data");
    return;
  } else 
  {
  displaySCD();  //display all the values from the SCD30 sensor
  Serial.println("");
  }

  //Control of the valves.
  if((co2Read >= 400) && (co2Read <=2000))
  {
    openValve1();
    closeValve2();
  }else if((co2Read <= 400) || (co2Read >=2000))
  {
    closeValve1();
    openValve2();
  }else {
    //do nothing
  }
 
  displayPressure(); //display the pressure sensors values

  delay(timeDelay);
}


/*---------Functions----------------------------*/
//Display the temp. in celsius, the humidity percentage, the detected CO2 in ppm.
void displaySCD()
{
    tempRead = scd30.temperature;
    humidityRead = scd30.relative_humidity;
    co2Read = scd30.CO2;
    
    Serial.print("Temperature:  ");      
    Serial.print(tempRead);
    Serial.println(" degrees C");

    Serial.print("Relative Humidity:  ");
    Serial.print(humidityRead);
    Serial.println(" %");

    Serial.print("CO2:  ");
    Serial.print(co2Read, 3);
    Serial.println(" ppm");
}

//Functions for opening and closing valves
void openValve1()
{
  digitalWrite(valvePin1, LOW);  //opposite, so LOW is open and vice versa
  Serial.println("Valve 1 open!");
}

void closeValve1()
{
  digitalWrite(valvePin1, HIGH);
  Serial.println("Valve 1 closed!");
}

void openValve2()
{
  digitalWrite(valvePin2, LOW);
  Serial.println("Valve 2 open!");
}

void closeValve2()
{
  digitalWrite(valvePin2, HIGH);
  Serial.println("Valve 2 closed!");
}

//Get and display the pressure sensor values
void displayPressure()
{
  //Get and display pressure sensor values 
  pressureRead1 = analogRead(pressurePin1);
  pressureValue1 = map(pressureRead1, pressureMin, pressureMax, 0, pressurePSImax);

  pressureRead2 = analogRead(pressurePin2);
  pressureValue2 = map(pressureRead2, pressureMin, pressureMax, 0, pressurePSImax);
 

  Serial.print("Pressure Sensor 1:  ");
  Serial.print(pressureValue1);
  Serial.println("psi");


  Serial.print("Pressure Sensor 2:  ");
  Serial.print(pressureValue2);
  Serial.println("psi");
  Serial.println("");
}