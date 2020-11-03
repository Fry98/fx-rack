#include <node.h>
#include <uv.h>
#include "iimavlib.h"
#include "iimavlib/WaveFile.h"
#include "iimavlib/WaveSource.h"
#include <atomic>

namespace fx_rack {
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Context;
  using iimavlib::WaveFile;
  using iimavlib::WaveSource;
  using iimavlib::PlatformDevice;
  using iimavlib::PlatformSink;
  using iimavlib::filter_chain;

  std::atomic<bool> active(false);
  WaveFile* current_file = nullptr;

  struct Work {
    uv_work_t request;
  };

  void play_worker(uv_work_t *req) {
    auto device_id = PlatformDevice::default_device();
    auto chain = filter_chain<WaveSource>(*current_file, active)
      .add<PlatformSink>(device_id)
      .sink();

    chain->run();
  }

  void play_worker_cb(uv_work_t *req, int status) {
    delete req->data;
  }

  void load(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    String::Utf8Value filename(isolate, args[0]);

    if (args.Length() != 1 || !args[0]->IsString()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Invalid arguments").ToLocalChecked()
      ));
      return;
    }

    try {
      WaveFile* new_file = new WaveFile(*filename);
      if (current_file != nullptr) delete current_file;
      current_file = new_file;
    } catch (...) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Error loading audio file").ToLocalChecked()
      ));
    }
  }

  void play(const FunctionCallbackInfo<Value>& args) {
    if (active) return;

    Isolate* isolate = args.GetIsolate();
    if (current_file == nullptr) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "No audio file loaded").ToLocalChecked()
      ));
      return;
    }

    active = true;
    Work* work = new Work();
    work->request.data = work;
    uv_queue_work(uv_default_loop(), &work->request, play_worker, play_worker_cb);
  }

  void stop(const FunctionCallbackInfo<Value>& args) {
    active = false;
  }

  void on_finished(const FunctionCallbackInfo<Value>& args) {

  }

  void initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "load", load);
    NODE_SET_METHOD(exports, "play", play);
    NODE_SET_METHOD(exports, "stop", stop);
    NODE_SET_METHOD(exports, "onFinished", on_finished);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, initialize);
}
