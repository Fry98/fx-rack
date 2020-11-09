/**
 * @file 	WaveFile.cpp
 *
 * @date 	19.1.2013
 * @author 	Zdenek Travnicek <travnicek@iim.cz>
 * @copyright GNU Public License 3.0
 *
 */

#include "iimavlib/WaveFile.h"
#include "iimavlib/Utils.h"
#include <stdexcept>
namespace iimavlib {

WaveFile::WaveFile(const std::string& filename, audio_params_t params)
:params_(params),mono_source_(false), cursor(nullptr)
{
	file_.open(filename,std::ios::binary | std::ios::out | std::ios::trunc);
	if (!file_.is_open()) throw std::runtime_error("Failed to open the output file");
	header_ = wav_header_t(number_of_channels,
								convert_rate_to_int(params_.rate),
								16);//params_.sample_size()*8);
	update(0);
}

WaveFile::WaveFile(const std::string& filename, std::atomic<size_t>* cursor, Duration& duration):mono_source_(false), cursor(cursor)
{
	file_.open(filename,std::ios::binary | std::ios::in);
	if (!file_.is_open()) throw std::runtime_error("Failed to open the input file");
	file_.read(reinterpret_cast<char*>(&header_),sizeof(wav_header_t));
	if (file_.gcount()!=sizeof(wav_header_t))
		throw std::runtime_error("Failed to read wav header");
	params_.rate = convert_int_to_rate(header_.rate);
	duration.rate = header_.rate;
	if (header_.bps != 16) {
		throw std::runtime_error("Only 16bit depths supported");
	}
	if (header_.channels != 2 ) {
		if (header_.channels == 1) {
			mono_source_ = true;
		} else {
			throw std::runtime_error("Only mono or stereo samples are supported");
		}
	}

	if (mono_source_) {
		int16_t sample;
		while (file_.read(reinterpret_cast<char*>(&sample), sizeof(int16_t))) {
			mono_data.push_back(sample);
		}
		duration.length = mono_data.size();
	} else {
		audio_sample_t sample;
		while (file_.read(reinterpret_cast<char*>(&sample), sizeof(audio_sample_t))) {
			stereo_data.push_back(sample);
		}
		duration.length = stereo_data.size();
	}
}

void WaveFile::update(size_t new_data_size)
{
	header_.add_size(static_cast<uint32_t>(new_data_size));
	file_.seekp(0,std::ios::beg);
	file_.write(reinterpret_cast<char*>(&header_),sizeof(header_));
	file_.seekp(0,std::ios::end);
}

audio_params_t WaveFile::get_params() const
{
	return params_;
}

error_type_t WaveFile::store_data(const std::vector<audio_sample_t>& data, size_t sample_count)
{
	if (!sample_count) sample_count = data.size();
	const size_t data_size = sample_count*params_.sample_size();
	if (data_size) {
		update(data_size);
		file_.write(reinterpret_cast<const char*>(&data[0]),data_size);
	}
	return error_type_t::ok;
}

error_type_t WaveFile::read_data(std::vector<audio_sample_t>& data, size_t& sample_count)
{
	size_t max_samples = data.size();
	size_t invalid = 0;
	size_t cursor_cpy = cursor->load();

	if (sample_count > max_samples) sample_count = max_samples;
	if (!mono_source_) {
		size_t data_size = stereo_data.size();
		for (size_t i = 0; i < sample_count; i++) {
			size_t idx = cursor_cpy + i;
			if (idx < data_size) {
				data[i] = stereo_data[idx];
			} else {
				data[i] = 0;
				invalid++;
			}
		}
	} else {
		size_t data_size = mono_data.size();
		for (size_t i = 0; i < sample_count; i++) {
			size_t idx = cursor_cpy + i;
			if (idx < data_size) {
				data[i].left = mono_data[idx];
			} else {
				data[i].left = 0;
				invalid++;
			}
		}
	}

	tsfn->BlockingCall((void*) nullptr, [cursor_cpy](Napi::Env env, Napi::Function jscb, void* value) {
		jscb.Call({ Napi::Number::New(env, cursor_cpy) });
	});

	sample_count = sample_count - invalid;
	cursor->store(cursor_cpy + sample_count);
	return error_type_t::ok;
}

void WaveFile::set_cursor_cb(Napi::ThreadSafeFunction* tsfn) {
	this->tsfn = tsfn;
}
}

