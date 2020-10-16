#include <node.h>
#include <uv.h>
#include <atomic>
#include <iostream>
#include "iimavlib.h"
#include "iimavlib/WaveSource.h"

namespace fx_rack {
  using iimavlib::PlatformDevice;
  using iimavlib::PlatformSink;
  using iimavlib::filter_chain;
  using iimavlib::WaveSource;
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
    std::string filename;
  };

  std::atomic<bool> active(false);

  void play_worker(uv_work_t* req) {
    Work* work = static_cast<Work*>(req->data);
    auto device_id = PlatformDevice::default_device();
    auto chain = filter_chain<WaveSource>(work->filename, active)
      .add<PlatformSink>(device_id)
      .sink();

    chain->run();
    active = false;
  }

  void play_worker_cb(uv_work_t *req,int status) {
    delete req->data;
  }

  void play(const FunctionCallbackInfo<Value>& args) {
    if (active) return;
    active = true;

    Isolate* isolate = args.GetIsolate();
    String::Utf8Value filename(isolate, args[0]);

    Work* work = new Work();
    work->request.data = work;
    work->filename = std::string(*filename);
    uv_queue_work(uv_default_loop(), &work->request, &play_worker, &play_worker_cb);
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
