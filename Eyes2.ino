#include <FTOLED.h>
#include <progmem_compat.h>


/*
 * A very simple sketch showing all the possible drawing operations you can do
 */
#include <SPI.h>
#include <SD.h>
#include <eyeData.h>

const byte pin_csL = 4;
const byte pin_csR = 3;

const byte pin_dc = 9;
const byte pin_reset = 10;



OLED oled(pin_csL, pin_csR, pin_dc, pin_reset);

uint16_t img2[64][256];
int t;
void setup()
{

  memcpy(img2, img, sizeof(img)); // Copy const image to RAM
  Serial.begin(9600);
  Serial.println("Hello");
  oled.begin();


}

void loop()
{
  int x;


 
  
    for(x=5; x<60; x=x + 2) {
     t++;
  drawEye(x, t);
  
  
}
for (x = 60; x > 5; x = x - 2) {
  t++;
  drawEye(x, t);
 
  
}

Serial.println(t);




}


void drawEye(int Iris, int Rotat) {

  uint8_t  xL, y, aL, aR, xR;
  uint16_t pixel;
  uint32_t d, scale;
  uint16_t pixelsL[128], pixelsR[128];
  scale = Iris * 8;               // Read pot n A0

  scale = 256 + (scale * scale) / 256;
 Rotat = Rotat % 360;
  // Set up raw write to full screen:

  // Can now just issue raw 16-bit pixel values...

  // Currently this does math on and writes to every pixel,
  // even the black pixels in tShe corner that are never used.
  // Could perhaps do further optimization to skip those,
  // either using several setAddrWindows as needed, or a
  // tighter loop of zero-writes on those pixels...
  // not stressing over this right now, already stupid fast.
  oled.setTransaction(-1);



  for (y = 0; y < 128; y++) {          // For each row...
    for (xL = 0; xL < 128; xL++) {  // For each column...
      xR = abs(128 - xL);
      d = (dist[y][xL] * scale) / 1024; // Fetch polar distance, do scale
      // Serial.println(d);
      //  delay(100);
      if ((d > 0) && (d < 64)) {       // Within iris area?
        aL  = angle[y][xL] - Rotat;           // Fetch polar angle value for pixel
        aR  = angle[y][xR] + Rotat;           // Fetch polar angle value for pixel
        //Serial.println(a);
        pixelsL[xL] = img2[d][aL];         // Fetch 16-bit pixel value from map
        pixelsR[xR] = img2[d][aR];
      } else { // Out-of-bounds pixel -- show black
        pixelsL[xL] = 0;
        pixelsR[xR] = 0;
      }
    }
    oled.fillLine(y, pixelsL, pixelsR);
  }
  oled.setTransaction(0);


}

