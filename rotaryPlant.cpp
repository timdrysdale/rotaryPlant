#include "rotaryPlant.h"

RotaryPlant::RotaryPlant(int ppr, float lpf, float timeToSeconds) {
  _ppr = ppr;

  // low pass filter coefficient for velocity
  // typically
  if (lpf > 1) lpf = 1;
  if (lpf < 0) lpf = 0;
  a = lpf;

  // if times are in micros, then set timeToSeconds to be 1e-6
  convertTimeToSeconds = timeToSeconds;
  
  pMax = (ppr / 2);
  pMin = (ppr / 2) * -1;
}

long RotaryPlant::wrap(long raw) {

  if ( raw <= pMax && raw >= pMin) {
	return raw;
  }

  long residue = raw % (_ppr);
  
  if ( residue <= pMax && residue >= pMin) {
	return residue;
  }
  
  residue = residue % ( _ppr / 2);
  
  if (residue >= 0 ) return pMin + residue;
  return pMax + residue;
}

float RotaryPlant::fractionalPosition(long raw) {
  return (float) wrap(raw) / (float)_ppr;
}

void RotaryPlant::initialise(long position, long time) {
  p0 = fractionalPosition(position);
  v0 = 0;
  t0 = time;
  fv0 = 0;
}

void RotaryPlant::sample(long position, long time) {

  p1 = p0;
  t1 = t0;
  v1 = v0;
  fv1 = fv0;
  raw1 = raw0;
  samples++;
  if (samples > 2) {
	velocityValid = true;
  } else {
	v1 = 0;
	fv1 = 0;
  }
  raw0 = position;
  t0 = time;

  /******* position *******/
  p0 = fractionalPosition(position);  // Posiiton unit is fraction of a revolution

  /******* velocity ******/
  
  // use raw position to avoid mistakes due to wrapping
  float dp = ( (float)raw0 - (float)raw1 ) / (float)_ppr; // displacement unit is fraction of a revolution 
  
  // https://arduino.stackexchange.com/questions/33572/arduino-countdown-without-using-delay/33577#33577
  // no need to compensate for the clock overflow, because the subtraction overflows too and you get
  // right answer.

  if (t0 == t1) return; // resample at same time, so assume velocity same as before (+avoid nan from divide by zero!)
  
  float dt = ((float)t0 - (float)t1) * convertTimeToSeconds;

  v0 = dp / dt;
  
  //https://dsp.stackexchange.com/questions/60277/is-the-typical-implementation-of-low-pass-filter-in-c-code-actually-not-a-typica
  fv0 = ((1 - a) * fv1) + (a * (v0 + v1) / 2);
}

float RotaryPlant::getPosition(void) { // position in units of fraction of a revolution
  return p0; //expected range -0.5 <= pos < 0.5
}

float RotaryPlant::getVelocity(void) { // velocity in units of revolutions per second
	return velocityValid ? fv0 : 0;
}



