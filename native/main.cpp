#include <napi.h>
#include <atomic>
#include <iostream>
#include "Duration.h"
#include "Device.h"
#include "Reverb.h"
#include "Distortion.h"
#include "Filter.h"
#include "iimavlib.h"
#include "iimavlib/WaveFile.h"
#include "iimavlib/WaveSource.h"

#define NAPI_FUNCTION(name, func) \
  exports.Set( \
    String::New(env, name), \
    Function::New(env, func) \
  );

namespace fx_rack {
  using iimavlib::WaveFile;
  using iimavlib::WaveSource;
  using iimavlib::PlatformDevice;
  using iimavlib::PlatformSink;
  using iimavlib::filter_chain;
  using Napi::CallbackInfo;
  using Napi::TypeError;
  using Napi::Object;
  using Napi::Function;
  using Napi::String;
  using Napi::Env;
  using Napi::Value;
  using Napi::ThreadSafeFunction;
  using Napi::Number;
  using Napi::Boolean;
  using Napi::Array;

  std::atomic<bool> active(false);
  std::atomic<size_t> cursor(0);
  Duration duration;
  WaveFile* current_file = nullptr;
  IDevice** devices = nullptr;
  size_t device_count = 0;
  ThreadSafeFunction tsfn;

  void play_worker() {
    auto device_id = PlatformDevice::default_device();
    auto chain = filter_chain<WaveSource>(*current_file, active);

    for (size_t i = 0; i < device_count; i++) {
      switch (devices[i]->getType()) {
        case 0: {
          auto dev = static_cast<FilterDevice*>(devices[i]);
          FilterType type = dev->hp ? HPF : LPF;
          chain = chain.add<Filter>(type, dev->taps, dev->cutoff);
          break;
        }
        case 1: {
          auto dev = static_cast<ReverbDevice*>(devices[i]);
          chain = chain.add<Reverb>(dev->delay, dev->decayFactor, dev->mix);
          break;
        }
      }
    }

    auto tail = chain
      .add<PlatformSink>(device_id)
      .sink();

    tail->run();
    active = false;
    for (size_t i = 0; i < device_count; i++) delete devices[i];
    delete[] devices;
  }

  Value load(const CallbackInfo& info) {
    active = false;
    Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsString()) {
      TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
      return env.Null();
    }

    String filepath = info[0].As<String>();
    try {
      WaveFile* new_file = new WaveFile(std::string(filepath), &cursor, duration);
      if (current_file != nullptr) delete current_file;
      current_file = new_file;
      cursor = 0;

      Object obj = Object::New(env);
      obj.Set("rate", duration.rate);
      obj.Set("length", duration.length);
      return obj;
    } catch (...) {
      TypeError::New(env, "Error loading audio file").ThrowAsJavaScriptException();
      return env.Null();
    }
  }

  void play(const CallbackInfo& info) {
    Env env = info.Env();
    if (active) return;

    if (current_file == nullptr) {
      TypeError::New(env, "No audio file loaded").ThrowAsJavaScriptException();
      return;
    }

    if (cursor == duration.length) cursor = 0;
    current_file->set_cursor_cb(&tsfn);
    active = true;

    Array filters = info[0].As<Array>();
    device_count = filters.Length();
    devices = new IDevice*[device_count];

    for (size_t i = 0; i < device_count; i++) {
      Value val = filters[i];
      Object obj = val.As<Object>();
      auto type = obj.Get("type").As<Number>().Int32Value();

      switch (type) {
        case 0: {
          double cutoff = obj.Get("cutoff").As<Number>().DoubleValue();
          int taps = obj.Get("slope").As<Number>().Int32Value();
          bool hp = obj.Get("hp").As<Boolean>().Value();
          devices[i] = new FilterDevice(cutoff, taps, hp);
          break;
        } case 1: {
          double delay = obj.Get("delay").As<Number>().DoubleValue();
          double decayFactor = obj.Get("decayFactor").As<Number>().DoubleValue();
          int mix = obj.Get("mix").As<Number>().Int32Value();
          devices[i] = new ReverbDevice(delay, decayFactor, mix);
          break;
        }
      }
    }

    std::thread(play_worker).detach();
  }

  void stop(const CallbackInfo& info) {
    active = false;
  }

  void on_cursor_move(const CallbackInfo& info) {
    Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsFunction()) {
      TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
      return;
    }

    tsfn = ThreadSafeFunction::New(
      env,
      info[0].As<Napi::Function>(),
      "TSFN", 0, 1
    );
  }

  void reset(const CallbackInfo& info) {
    active = false;
    cursor = 0;
  }

  void skip(const CallbackInfo& info) {
    Env env = info.Env();

    if (info.Length() != 1 || !info[0].IsNumber()) {
      TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
      return;
    }

    Number idx = info[0].As<Number>();
    size_t cursor_idx = (size_t) double(idx);
    cursor = cursor_idx;
  }

  Object initialize(Env env, Object exports) {
    NAPI_FUNCTION("load", load);
    NAPI_FUNCTION("play", play);
    NAPI_FUNCTION("stop", stop);
    NAPI_FUNCTION("reset", reset);
    NAPI_FUNCTION("skip", skip);
    NAPI_FUNCTION("onCursorMove", on_cursor_move);
    return exports;
  }

  NODE_API_MODULE(NODE_GYP_MODULE_NAME, initialize)
}
