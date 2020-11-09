#include <napi.h>
#include <atomic>
#include "duration.h"
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

  std::atomic<bool> active(false);
  std::atomic<size_t> cursor(0);
  Duration duration;
  WaveFile* current_file = nullptr;
  ThreadSafeFunction tsfn;

  void play_worker() {
    auto device_id = PlatformDevice::default_device();
    auto chain = filter_chain<WaveSource>(*current_file, active)
      .add<PlatformSink>(device_id)
      .sink();

    chain->run();
    active = false;
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
      cursor = 0;
      WaveFile* new_file = new WaveFile(std::string(filepath), &cursor, duration);
      if (current_file != nullptr) delete current_file;
      current_file = new_file;

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

  Object initialize(Env env, Object exports) {
    NAPI_FUNCTION("load", load);
    NAPI_FUNCTION("play", play);
    NAPI_FUNCTION("stop", stop);
    NAPI_FUNCTION("reset", reset);
    NAPI_FUNCTION("onCursorMove", on_cursor_move);
    return exports;
  }

  NODE_API_MODULE(NODE_GYP_MODULE_NAME, initialize)
}
