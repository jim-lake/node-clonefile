#include "napi.h"
#include <stdio.h>
#include <sys/attr.h>
#include <sys/clonefile.h>

using namespace Napi;

class CloneWorker : public AsyncWorker {
public:
  CloneWorker(const Function &callback, const std::string &src,
              const std::string &dest, const int flags)
      : AsyncWorker(callback, "clonefile"), _src(src), _dest(dest),
        _flags(flags) {}

  ~CloneWorker() {}
  void Execute() override {
    const auto ret =
        clonefile(this->_src.c_str(), this->_dest.c_str(), this->_flags);
    if (ret == -1) {
      this->_errno = errno;
      const char *str_err = strerror(this->_errno);
      const std::string err = str_err == NULL ? "Unknown error" : str_err;
      SetError(err);
    }
  }
  void OnError(const Napi::Error &error) override {
    HandleScope scope(Env());
    error.Set("errno", Number::New(Env(), this->_errno));
    AsyncWorker::OnError(error);
  }

private:
  std::string _src;
  std::string _dest;
  int _flags;
  int _errno;
};

Value Clonefile(const Napi::CallbackInfo &info) {
  const Napi::Env env = info.Env();
  Value ret = env.Null();

  if (info.Length() != 4) {
    ret = String::New(env, "Expected 4 arguments");
  } else if (!info[0].IsString()) {
    ret = String::New(env, "Expected string arg 0");
  } else if (!info[1].IsString()) {
    ret = String::New(env, "Expected string arg 1");
  } else if (!info[2].IsNumber()) {
    ret = String::New(env, "Expected number arg 2");
  } else if (!info[3].IsFunction()) {
    ret = String::New(env, "Expected function arg 3");
  } else {
    const auto src = info[0].As<String>();
    const auto dest = info[1].As<String>();
    const auto flags = info[2].As<Number>().Int32Value();
    const auto cb = info[3].As<Function>();
    const auto worker = new CloneWorker(cb, src, dest, flags);
    worker->Queue();
  }
  return ret;
}
Object Init(Napi::Env env, Object exports) {
  exports.Set("clonefile", Function::New(env, Clonefile));
  return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
