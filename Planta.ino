#include <Servo.h>
#include <math.h>

#define SP(x) Serial.print(x)
#define SPL(x) Serial.println(x)
#define AR(x) analogRead(x)
#define AW(x, y) analogWrite(x, y)
#define DR(x) digitalRead(x)
#define DW(x, y) digitalWrite(x, y)

const int trigPin = 6;
const int echoPin = 7;

long duration;
int distance;

Servo planta;
int senzor_umiditate = A1;
int senzor_lumina = 2;
int senzor_caldura = 0;
int potentiometru = 3;
int buton = 11;
int jumper = 10;
int state_button = 0;
int rosu = 3, verde = 4, albastru = 5;


// umid temperatura lumina umid&temp umid&lumina temp&lumina umid&temp&lumina

int valoare_umiditate, valoare_lumina, valoare_caldura, valoare_potentiometru, valoare_buton, valoare_jumper;

int threshold_umiditate = 0;
int threshold_lumina = 0;
int threshold_temperatura = 0;

float convert_temperature(int U) {
  return (1 / ((1/298.15) + (log((1024/(float)U) - 1)/4000)) - 273.15);
}

float convert_light(int U) {
  return 1024.0 * U / 5.0 - U;
}

void set_threshold()
{
  if (state_button == 0) threshold_umiditate = map(valoare_potentiometru, 0, 1000, 40, 100); ///testata val umid
  else if (state_button == 1) threshold_temperatura = map(valoare_potentiometru, 0, 1000, 40, 100); ///testata val temperatura
  else if (state_button == 2) threshold_lumina = map(valoare_potentiometru, 0, 1000, 40, 700); ///testata val lumina
}

void button_pressed(int limita)
{
  state_button++;
  while (state_button == limita)
  {
      DW(verde, HIGH);
      DW(rosu, HIGH);
      DW(albastru, HIGH);
      delay(100);
      DW(verde, LOW);
      DW(rosu, LOW);
      DW(albastru, LOW);
      delay(100);
    valoare_jumper = DR(jumper);
    if (valoare_jumper == 0)
    {
      state_button = 0;
    }
    DW(verde, LOW);
    DW(rosu, LOW);
    DW(albastru, LOW);
  }
}

void recoltare_date()
{
  int om = 1;
  distanta();

  SP("distanta:");
  SPL(distance);

  valoare_umiditate = AR(senzor_umiditate);
  SP("valoare_umiditate=");
  SP(valoare_umiditate);
  SP("\t");


  valoare_lumina = AR(senzor_lumina);
  SP("valoare_lumina=");
  SP(convert_light(valoare_lumina));
  SP("/");
  SP(valoare_lumina);
  SP("\t");

  valoare_caldura = AR(senzor_caldura);
  SP("valoare_caldura=");
  SP(convert_temperature(valoare_caldura));
  SP("/");
  SP(valoare_caldura);
  SP("\t");

  valoare_potentiometru = AR(potentiometru);
  SP("valoare_potentiometru=");
  SP(valoare_potentiometru);
  SP("\t");

  valoare_buton = DR(buton);
  SP("valoare_buton=");
  SP(valoare_buton);
  SP("\t");


  valoare_jumper = DR(jumper);
  SP("valoare_jumper=");
  SPL(valoare_jumper);
}

void umid()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_umiditate < threshold_umiditate)
  {
    planta.write (180);
    DW(verde, LOW);
    delay(100);
  }
  else {
    planta.write (0);
    DW(verde, HIGH);
    delay(100);
  }
}

void temp()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_caldura < threshold_temperatura)
  {
    planta.write (180);
    DW(rosu, LOW);
    delay(100);
  }
  else {
    planta.write (0);
    DW(rosu, HIGH);
    delay(100);
  }
}

void lumina()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_lumina < threshold_lumina)
  {
    planta.write (180);
    DW(albastru, LOW);
    delay(100);
  }
  else {
    planta.write (0);
    DW(albastru, HIGH);
    delay(100);
  }
}

void umid_temp()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_umiditate < threshold_umiditate && valoare_caldura < threshold_temperatura)
  { planta.write (180);
    DW(verde, LOW);
    DW(rosu, LOW);
    delay(100);
  }

  else {
    planta.write (0);
    if (valoare_umiditate > threshold_umiditate)
    { DW(verde, HIGH);
      delay(1000);
      DW(verde, LOW);
    }
    if (valoare_caldura > threshold_temperatura)
    { DW(rosu, HIGH);
      delay(1000);
      DW(rosu, LOW);
    }
    delay(100);
  }
}

void umid_lumina()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_umiditate < threshold_umiditate && valoare_lumina < threshold_lumina)
  {
    planta.write (180);
    DW(verde, LOW);
    DW(albastru, LOW);
    delay(100);
  }
  else {
    planta.write (0);
    if (valoare_umiditate > threshold_umiditate)
    { DW(verde, HIGH);
      delay(1000);
      DW(verde, LOW);
    }
    if (valoare_lumina > threshold_lumina)
    { DW(albastru, HIGH);
      delay(1000);
      DW(albastru, LOW);
    }
    delay(100);
  }
}

void temp_lumina()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_caldura < threshold_temperatura && valoare_lumina < threshold_lumina)
  {
    planta.write (180);
    DW(rosu, LOW);
    DW(albastru, LOW);
    delay(100);
  }
  else {
    planta.write (0);
    if (valoare_caldura > threshold_temperatura)
    { DW(rosu, HIGH);
      delay(1000);
      DW(rosu, LOW);
    }
    if (valoare_lumina > threshold_lumina)
    { DW(albastru, HIGH);
      delay(1000);
      DW(albastru, LOW);
    }
    delay(100);
  }
}

void umid_temp_lumina()
{
  DW(verde, LOW);
  DW(rosu, LOW);
  DW(albastru, LOW);
  if (valoare_umiditate < threshold_umiditate && valoare_caldura < threshold_temperatura && valoare_lumina < threshold_lumina)
  {
    planta.write (180);
    DW(verde, LOW);
    DW(rosu, LOW);
    DW(albastru, LOW);
    delay(100);
  }
  else {
    planta.write (0);
    if (valoare_umiditate > threshold_umiditate)
    { DW(verde, HIGH);
      delay(1000);
      DW(verde, LOW);
      delay(50);
    }
    if (valoare_caldura > threshold_temperatura)
    { DW(rosu, HIGH);
      delay(1000);
      DW(rosu, LOW);
      delay(50);
    }
    if (valoare_lumina > threshold_lumina)
    { DW(albastru, HIGH);
      delay(1000);
      DW(albastru, LOW);
      delay(50); 
    }
    delay(100);
  }
}

void distanta()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;
}

void status_led() {
  if (state_button == 0) {
    DW(verde, HIGH);
    delay(250);
    DW(verde, LOW);
    delay(250);
  }
  else if(state_button == 1) {
    DW(rosu, HIGH);
    delay(250);
    DW(rosu, LOW);
    delay(250);
  }
  else if(state_button == 2) {
    DW(albastru, HIGH);
    delay(250);
    DW(albastru, LOW);
    delay(250);
  }
}

void setup()
{
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  planta.attach(9);
  planta.write(20);
  delay(100);
  Serial.begin(9600);

  threshold_umiditate = AR(senzor_umiditate)+80;
  threshold_temperatura = AR(senzor_caldura)+10;
  threshold_lumina = AR(senzor_lumina)+20;
}

void loop()
{

  recoltare_date();

  if(valoare_jumper == 1) {
    DW(verde, LOW);
    DW(rosu, LOW);
    DW(albastru, LOW);
    status_led();
  }

  if (valoare_jumper == 1 && valoare_buton == 1)
  {
    set_threshold();
    button_pressed(3);
  }
  else if (valoare_jumper == 0 )
  {
    if (valoare_buton == 1)
      button_pressed(7);

    switch (state_button)
    {
      case 0:
        {
          umid();
          break;
        }

      case 1:
        {
          temp();
          break;
        }

      case 2:
        {
          lumina();
          break;
        }

      case 3:
        {
          umid_temp();
          break;
        }

      case 4:
        {
          umid_lumina();
          break;
        }

      case 5:
        {
          temp_lumina();
          break;
        }

      case 6:
        {
          umid_temp_lumina();
          break;
        }

    }
  }


  SP("th_um=");
  SP(threshold_umiditate);
  SP("\t");

  SP("th_ca=");
  SP(threshold_temperatura);
  SP("\t");

  SP("th_lu=");
  SPL(threshold_lumina);

  if (distance < 7)
  {
    //  om = 0;
  }

  SP("state_button=");
  SPL(state_button);

  delay(100);
}



