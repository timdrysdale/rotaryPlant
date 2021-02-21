#ifndef ROTARYPLANT_ROTARYPLANT_H_
#define ROTARYPLANT_ROTARYPLANT_H_
#endif

//Calculates the position and velocity given the
// current count and timestamp from a rotary encoder
//
// ppr: pulses per revolution from your encoder
// 
// lpf: low pass filter coefficient. Set to 1 to
//      have unfiltered velocites. A typical filter
//      value is lpf = 0.1
//
// timeToSeconds: set to 1e-6 if times are in microseconds
//
// Once the plant is instantiated, prime it with the current
// time and encoder count using initialise().
// call sample() in the interrupt routine with the
// latest encoder count and time in e.g. microseconds.
// Then the position and velocity obtained at that time
// can be retrieved any time until the next call to sample()
// by using the getters getPosition(), and getVelocity().

class RotaryPlant{

  long raw0, raw1;
  float p0, p1;
  long t0, t1;
  float v0, v1; //raw velocity 
  float fv0, fv1; //filtered velocity
  float a; //filter coefficient
  float convertTimeToSeconds;
  long samples = 0;
  bool velocityValid = false;
  
  int _ppr; //pulses per revolution expected (depends on if pins are interrupts)
  int pMax, pMin;
public:
  RotaryPlant(int ppr, float lpf, float timeToSeconds);
  long wrap(long raw); // internal function exposed for testing
  float fractionalPosition(long raw); // internal function exposed for testing
  void initialise(long position, long time);
  void sample(long position, long time); // call this on an interrupt
  float getPosition(void); // call when last sampled position is required
  float getVelocity(void); // call when last sampled velocity is required
};

