#include <node.h>
#include <nan.h>

class Internal {
};

class B {
  public:
    explicit B(Internal internal):internal(internal){}
};

class A {
  public:
    A() : internal() {};
    B foo() { return B(internal); }
  private:
    Internal internal;
};

class BWrapper : public Nan::ObjectWrap {
  public:
    B a_;
    explicit BWrapper(B b) : b_(b) {}
    ~BWrapper() {}
};


class AWrapper : public Nan::ObjectWrap {
  public:
  A a_;
  explicit AWrapper(A a) : a_(a) {}
  ~AWrapper() {}

  static void register_class(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("A").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "foo", foo());

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("A").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
  }

 private:

  static NAN_METHOD(New) {
    if (info.IsConstructCall()) {
      A a;
      AWrapper *obj = new AWrapper(a);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    } else {
      //TODO: Not sure what this is doing and/or why
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(foo) {
    AWrapper* obj = Nan::ObjectWrap::Unwrap<AWrapper>(info.Holder());
    B b = obj->foo();
    BWrapper * result = new BWrapper(b);
    // Something to get a B object to javascript
    //...
    info.GetReturnValue().Set(result->Wrap());
  }

};


NAN_MODULE_INIT(InitModule) {
  AWrapper::register_class(target);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitModule);
