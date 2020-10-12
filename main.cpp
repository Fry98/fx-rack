#include <node.h>
#include "iimavlib.h"
#include "iimavlib/WaveSource.h"

namespace fx_rack {
  using iimavlib::PlatformDevice;
  using iimavlib::WaveSource;
  using iimavlib::PlatformSink;
  using iimavlib::filter_chain;
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void play(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    if (args.Length() != 1 || !args[0]->IsString()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Invalid arguments").ToLocalChecked()
      ));
      return;
    }

    String::Utf8Value str(isolate, args[0]);
    auto device_id = PlatformDevice::default_device();
    auto chain = filter_chain<WaveSource>(*str)
      .add<PlatformSink>(device_id)
      .sink();

    chain->run();
  }

  void initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "play", play);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, initialize);
}
