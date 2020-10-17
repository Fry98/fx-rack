#include <node.h>
#include <iostream>
#include "iimavlib.h"
#include "duaration.h"
#include "iimavlib/WaveFile.h"
// #include <uv.h>
// #include <atomic>

#define print(x) std::cout << x << std::endl;

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

  void load(const FunctionCallbackInfo<Value>& args) {
    print("load");
    Isolate* isolate = args.GetIsolate();
    if (args.Length() != 1 || !args[0]->IsString()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Invalid arguments").ToLocalChecked()
      ));
      return;
    }

    String::Utf8Value filename(isolate, args[0]);
    Duration duration;
    WaveFile file(*filename, duration);
    Local<Context> context = isolate->GetCurrentContext();
    Local<Object> obj = Object::New(isolate);

    obj->Set(
      context,
      String::NewFromUtf8(isolate, "rate").ToLocalChecked(),
      Number::New(isolate, duration.rate)
    );

    obj->Set(
      context,
      String::NewFromUtf8(isolate, "length").ToLocalChecked(),
      Number::New(isolate, duration.length)
    );

    args.GetReturnValue().Set(obj);
  }

  void initialize(Local<Object> exports) {
    print("init");
    NODE_SET_METHOD(exports, "load", load);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, initialize);
}
