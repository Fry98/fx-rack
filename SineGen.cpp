#include <iostream>
#include "SineGen.h"
#include "iimavlib.h"

namespace sine_gen {
  using iimavlib::audio_params_t;
  using iimavlib::pAudioFilter;
  using iimavlib::convert_rate_to_int;

  const double pi2 = 8 * std::atan(1.0);
  const double max_val = std::numeric_limits<int16_t>::max();

  SineGen::SineGen(
    const double freq,
    bool& active
  ): AudioFilter(pAudioFilter()), freq(freq), active(active) {}

  error_type_t SineGen::do_process(audio_buffer_t& buffer) {
    if (!active) return error_type_t::buffer_empty;
    const audio_params_t& params = buffer.params;
    const double step = 1.0 / convert_rate_to_int(params.rate);
    for (auto& sample : buffer.data) {
      sample = static_cast<int16_t>(max_val * std::sin(time * freq * pi2));
      time = time + step;
    }
    return error_type_t::ok;
  }
}
