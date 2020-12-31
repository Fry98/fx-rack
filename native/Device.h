#include "Filter.h"
#include "Reverb.h"

namespace fx_rack {
  class IDevice {
    public:
      virtual int getType() const = 0;
  };

  class FilterDevice : public IDevice {
    public:
      FilterDevice(double cutoff, int taps, bool hp);
      int getType() const;
      double cutoff;
      int taps;
      bool hp;
  };

  class ReverbDevice : public IDevice {
    public:
      ReverbDevice(double delay, double decayFactor, int mix);
      int getType() const;
      double delay;
      double decayFactor;
      int mix;
  };

  class DistortionDevice : public IDevice {
    public:
      DistortionDevice(int16_t gain, double threshold);
      int getType() const;
      int16_t gain;
      double threshold;
  };
}
