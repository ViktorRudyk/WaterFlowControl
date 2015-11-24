
    // reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
    // Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
    // http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com

    volatile int NbTopsFan; //measuring the rising edges of the signal
    int Calc;                               
    int hallsensor = 2;    //The pin location of the sensor
    boolean buzzer_flag = false; // status buzzera dlya pereryvchatogo zvuchannya
    boolean warming_flag = false; // POWERLED blinking during set-up time
    
    unsigned long time; // chas roboty bloku, BUZZER ne pracyu
    long previousMillis = 0;        // will store last time BUZZER was updated
    long interval = 120000;           // interval at which to buzz (milliseconds)
    
    int Alarm_Flag = LOW;
    
    void rpm ()     //This is the function that the interupt calls
    {
      NbTopsFan++;  //This function measures the rising and falling edge of the  hall effect sensors signal
    }
    // The setup() method runs once, when the sketch starts
    void setup() //
    {
      pinMode(13, OUTPUT); // **Arduino LED** initialize digital pin 13 as an output.
      pinMode(12, OUTPUT); // **SIGNAL**  initialize digital pin 12 as an output. 
      pinMode(11, OUTPUT);//test
      pinMode(3, OUTPUT); // **RED1 LED** initialize digital pin 3 as an output.
      pinMode(4, OUTPUT); // **YELLOW1 LED** initialize digital pin 4 as an output.
      pinMode(5, OUTPUT); // **GREEN1 LED** initialize digital pin 5 as an output.
      pinMode(6, OUTPUT); // **YELLOW2 LED** initialize digital pin 6 as an output.
      pinMode(7, OUTPUT); // **RED2 LED** initialize digital pin 7 as an output.
      
      pinMode(8, OUTPUT); // **GREEN2 POWER LED** initialize digital pin 8 as an output.
      
      pinMode(9, OUTPUT); // **RF - transmitter OUTPUT** initialize digital pin 9 as an output.
      
      pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
      
      Serial.begin(9600); //This is the setup function where the serial port is initialised,
      attachInterrupt(0, rpm, RISING); //and the interrupt is attached
    }
    // the loop() method runs over and over again,
    // as long as the Arduino has power
    void loop ()   
    {
      
      // start POWER_LED 
      //digitalWrite(8, HIGH);
      
      
      
      NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
      sei();      //Enables interrupts
      delay (1000);   //Wait 1 second
      cli();      //Disable interrupts
      Calc = (NbTopsFan / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate  in L/hour
      
      Serial.print (NbTopsFan, DEC); //Prints the number of impulses
      Serial.print (" imp/min  "); //Prints "imp/hour"  
      
      Serial.print (Calc, DEC); //Prints the number calculated above
      Serial.print (" L/min\r\n"); //Prints "L/hour" and returns a  new line
      
      Serial.print("Time: ");
      time = millis();
      //выводим время с момента старта программы
      Serial.println(time);
      
      
      if (time > 45000){
      Alarm_Flag = HIGH;
      digitalWrite(8, HIGH);
      }else{
      if (warming_flag){
        digitalWrite(8, LOW);
        warming_flag = !buzzer_flag;  // RED2 LED ON, BUZZER ON, RF cannal ON, ALL BLINKING others - LOW
        }else{
        digitalWrite(8, HIGH);
        warming_flag = !buzzer_flag;
        }
      }
 
      if(time - previousMillis > interval) {
      previousMillis = time;  
      digitalWrite(12, HIGH);
      delay (600);
      digitalWrite(12, LOW);
      }
      
      if (NbTopsFan >= 18 ){
      digitalWrite(3, LOW);
      digitalWrite(4, LOW); // RED2 LED ON, BUZZER ON, others - LOW
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
            
        if (buzzer_flag){
        digitalWrite(7, LOW);
        digitalWrite(12, LOW);
        digitalWrite(9, LOW);
        buzzer_flag = !buzzer_flag;  // RED2 LED ON, BUZZER ON, RF cannal ON, ALL BLINKING others - LOW
        }else{
        digitalWrite(7, HIGH);
        digitalWrite(12, Alarm_Flag);
        digitalWrite(9, Alarm_Flag);
        buzzer_flag = !buzzer_flag;
        } 
      
      }else if (NbTopsFan < 18 && NbTopsFan >= 14){
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);   // YELLOW2 LED ON, others - LOW
      digitalWrite(7, LOW);
      digitalWrite(12, LOW);
      digitalWrite(6, HIGH);
      }else if (NbTopsFan < 14 && NbTopsFan >= 10){
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(6, LOW);  // GREEN1 LED ON, others - LOW
      digitalWrite(7, LOW);
      digitalWrite(12, LOW);
      digitalWrite(5, HIGH);
      }else if (NbTopsFan < 10 && NbTopsFan >= 6){
      digitalWrite(3, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);   // YELLOW1 LED ON, others - LOW
      digitalWrite(7, LOW);
      digitalWrite(12, LOW);
      digitalWrite(4, HIGH);
      }else if (NbTopsFan < 6){
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);  //RED1 LED ON, BUZZER ON, others - LOW
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      
        if (buzzer_flag){
        digitalWrite(3, LOW);
        digitalWrite(12, LOW);
        digitalWrite(9, LOW);
        buzzer_flag = !buzzer_flag; // RED1 LED ON, BUZZER ON, RF cannal ON, ALL BLINKING; others - LOW
        }else{
        digitalWrite(3, HIGH);
        digitalWrite(12, Alarm_Flag);
        digitalWrite(9, Alarm_Flag);
        buzzer_flag = !buzzer_flag;
        } 
      }
      
    }
