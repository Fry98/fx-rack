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
  using v8::Function;
  using v8::Context;
  using v8::Persistent;
  using iimavlib::WaveFile;

  WaveFile* current_file = nullptr;
  Persistent<Function> cursor_cb;

  void load(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    String::Utf8Value filename(isolate, args[0]);
    Duration duration;

    try {
      WaveFile* new_file = new WaveFile(*filename, duration);
      if (current_file != nullptr) delete current_file;
      current_file = new_file;
    } catch (...) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Error loading audio file").ToLocalChecked()
      ));
      return;
    }

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

  void on_cursor_move(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Function> cb = Local<Function>::Cast(args[0]);
    cursor_cb.Reset(isolate, cb);
  }

  void get_cursor(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Value> argv[] = {
      Number::New(isolate, 420)
    };

    Local<Function>::New(isolate, cursor_cb)->Call(
      isolate->GetCurrentContext(),
      Null(isolate), 1, argv
    );
    cursor_cb.Reset();
  }

  void initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "load", load);
    NODE_SET_METHOD(exports, "onCursorMove", on_cursor_move);
    NODE_SET_METHOD(exports, "getCursor", get_cursor);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, initialize);
}
