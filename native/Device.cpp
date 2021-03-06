#include "Device.h"
#include "Reverb.h"

namespace fx_rack {
  FilterDevice::FilterDevice(
    double cutoff,
    int taps,
    bool hp
  ): cutoff(cutoff), taps(taps), hp(hp) {}

  int FilterDevice::getType() const {
    return 0;
  }

  ReverbDevice::ReverbDevice(
    double delay, 
    double decayFactor, 
    int mix
  ): delay(delay), decayFactor(decayFactor), mix(mix) {}

  int ReverbDevice::getType() const {
    return 1;
  }

  DistortionDevice::DistortionDevice(
    double gain, 
    int16_t threshold
  ): gain(gain), threshold(threshold) {}

  int DistortionDevice::getType() const {
    return 2;
  }
}
