#include <node.h>
#include <nan.h>

class Internal {
};

class B {
  public:
    Internal internal;
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
    B b_;
    explicit BWrapper(B b) : b_(b) {}
    ~BWrapper() {}

  static void init_class() {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("B").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
  }

  static NAN_METHOD(New) {
	  if (!info.IsConstructCall()) {
      return Nan::ThrowError("File() must be called as a constructor");
    }

    if (info.Length() != 1 || ! info[0]->IsExternal()) {
        return Nan::ThrowError("File() can only be called internally");
    }

    B* b = static_cast<B*>(info[0].As<v8::External>()->Value());
    BWrapper *obj = new BWrapper(*b);
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

  static v8::Local<v8::Object> NewInstance(B* b) {
    Nan::EscapableHandleScope scope;

    const unsigned argc = 1;
    v8::Local<v8::Value> argv[argc] = { Nan::New<v8::External>(b) };
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor());
    v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

    return scope.Escape(instance);
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }
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

    Nan::SetPrototypeMethod(tpl, "foo", foo);

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
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
  }

  static NAN_METHOD(foo) {
    AWrapper* obj = Nan::ObjectWrap::Unwrap<AWrapper>(info.Holder());
    B b = obj->a_.foo();
    info.GetReturnValue().Set(BWrapper::NewInstance(&b));
  }

  static inline Nan::Persistent<v8::Function> & constructor() {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

};


NAN_MODULE_INIT(InitModule) {
  BWrapper::init_class();
  AWrapper::register_class(target);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitModule);
