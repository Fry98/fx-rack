#include <node.h>
#include <uv.h>
#include <iostream>
#include "SineGen.h"
#include "iimavlib.h"

namespace fx_rack {
  using iimavlib::PlatformDevice;
  using iimavlib::PlatformSink;
  using iimavlib::filter_chain;
  using sine_gen::SineGen;
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  struct Work {
    uv_work_t request;
  };

  bool active = false;

  void play_sine(uv_work_t* req) {
    auto device_id = PlatformDevice::default_device();
    auto chain = filter_chain<SineGen>(440.0, active)
      .add<PlatformSink>(device_id)
      .sink();

    chain->run();
    delete req->data;
  }

  void play(const FunctionCallbackInfo<Value>& args) {
    if (active) return;
    active = true;

    Work* work = new Work();
    work->request.data = work;
    uv_queue_work(uv_default_loop(), &work->request, &play_sine, nullptr);
  }

  void stop(const FunctionCallbackInfo<Value>& args) {
    active = false;
  }

  void initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "play", play);
    NODE_SET_METHOD(exports, "stop", stop);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, initialize);
}
