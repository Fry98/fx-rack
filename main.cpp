#include <iostream>
#include "iimavlib.h"
#include "iimavlib/WaveSource.h"

using namespace iimavlib;

int main() {
  auto device_id = PlatformDevice::default_device();

  auto chain = filter_chain<WaveSource>("sample.wav")
    .add<PlatformSink>(device_id)
    .sink();
  chain->run();
}
