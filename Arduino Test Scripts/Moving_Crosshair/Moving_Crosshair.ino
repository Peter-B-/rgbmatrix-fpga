#include <SPI.h>

const int CsPin = 15;
const int RstPin = 16;

void setup() {
  pinMode(CsPin, OUTPUT);
  pinMode(RstPin, OUTPUT);
  digitalWrite(CsPin, HIGH);
  digitalWrite(RstPin, HIGH);

  randomSeed(analogRead(0));

  
    SPI.begin();
  //  SPI.setHwCs(true);
    SPI.setDataMode(SPI_MODE0);
    //SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.setFrequency(2000000);
    //SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));  
    //SPI.setFrequency(10000000);

    Clear();
}


byte image [16][32][3] = {0};


float x = 0;
float y = 0;
float dx = 1;
float dy = 1;

float dt = 0.1;

void loop() {
  setupBackground();

  dx = dx + (random(300)-150)*0.0005;
  dy = dy + (random(300)-150)*0.0005;

  if (dx > 0.1) dx = 0.1;
  if (dx < -0.1) dx = -0.1;
  if (dy > 0.1) dx = 0.1;
  if (dy < -0.1) dx = -0.1;
  
  x = x+dx*dt;
  y = y+dy*dt;


  byte r = positive_modulo(x, 16);
  byte c = positive_modulo(y, 32);

  byte rp = constrain(r+1, 0, 15);
  byte rm = constrain(r-1, 0, 15);
  byte cp = constrain(c+1, 0, 31);
  byte cm = constrain(c-1, 0, 31);
  
  image[rm][c][0] = 255;
  image[rm][c][1] = 255;
  image[rm][c][2] = 255;
  image[rp][c][0] = 255;
  image[rp][c][1] = 255;
  image[rp][c][2] = 255;
  
  image[r][cm][0] = 255;
  image[r][cm][1] = 255;
  image[r][cm][2] = 255;
  image[r][cp][0] = 255;
  image[r][cp][1] = 255;
  image[r][cp][2] = 255;
  
  image[r][c][0] = 255;
  image[r][c][1] = 255;
  image[r][c][2] = 255;


  
  sendImage();
}

inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

void setupBackground()
{
  for (byte r = 0; r < 16; r++)
    for (byte c = 0; c < 32; c++)
    {
      image[r][c][0] =0;
      image[r][c][1] = c << 1;
      image[r][c][2] = r << 2;
    }
}


void sendImage()
{
  digitalWrite(RstPin, HIGH);
  digitalWrite(CsPin, LOW);
    
  for (byte r = 0; r < 8; r++)
    for (byte c = 0; c < 32; c++)
      for (byte rOff = 0; rOff <= 1; rOff++)
        for (byte rbg =0; rbg < 3; rbg++)
        {
          SPI.write(image[r+rOff*8][c][rbg]);
        }

  digitalWrite(CsPin, HIGH);
  digitalWrite(RstPin, LOW);
}











void Clear()
{
  digitalWrite(RstPin, HIGH);
  digitalWrite(CsPin, LOW);

  for (uint16_t r = 0; r < 256; r++)
  {
    SPI.write(0);
    SPI.write(0);
    SPI.write(0);
    SPI.write(0);
    SPI.write(0);
    SPI.write(0);
  }

  digitalWrite(CsPin, HIGH);
  digitalWrite(RstPin, LOW);
}


