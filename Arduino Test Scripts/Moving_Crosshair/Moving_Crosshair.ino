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
    SPI.setDataMode(SPI_MODE0);
    SPI.setFrequency(8000000);

    Clear();
}

const int cols = 64;
const int rows = 32;

byte image [rows][cols][3] = {0};


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


  byte r = positive_modulo(x, rows);
  byte c = positive_modulo(y, cols);

  byte rp = constrain(r+1, 0, rows - 1);
  byte rm = constrain(r-1, 0, rows - 1);
  byte cp = constrain(c+1, 0, cols - 1);
  byte cm = constrain(c-1, 0, cols - 1);
  
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
  for (byte r = 0; r < rows; r++)
    for (byte c = 0; c < cols; c++)
    {
      image[r][c][0] =0;
      image[r][c][1] = c << 0;
      image[r][c][2] = r << 0;
    }
}


void sendImage()
{
  digitalWrite(RstPin, HIGH);
  digitalWrite(CsPin, LOW);
    
  for (byte r = 0; r < rows >> 1; r++)
    for (byte c = 0; c < cols; c++)
      for (byte rOff = 0; rOff <= 1; rOff++)
        for (byte rbg =0; rbg < 3; rbg++)
        {
          SPI.write(image[r+rOff*(rows / 2)][c][rbg]);
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


