/**
 * @file 	WaveSource.h
 *
 * @date 	11.3.2013
 * @author 	Zdenek Travnicek <travnicek@iim.cz>
 * @copyright GNU Public License 3.0
 *
 * This file defines source filter for Wave files
 */

#ifndef WAVESOURCE_H_
#define WAVESOURCE_H_

#include "WaveFile.h"
#include "AudioFilter.h"
#include <string>
#include <atomic>

namespace iimavlib {
class EXPORT WaveSource: public AudioFilter {
public:
	WaveSource(WaveFile& file, std::atomic<bool>& active);
	virtual ~WaveSource();

private:
	virtual error_type_t do_process(audio_buffer_t& buffer);
	virtual audio_params_t do_get_params() const;
	WaveFile& file_;
	std::atomic<bool>& active;
};

}



#endif /* WAVESOURCE_H_ */
