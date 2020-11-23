#include "Filter.h"

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
}
