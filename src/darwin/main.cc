#include "napi.h"
#include <stdio.h>
#include <sys/attr.h>
#include <sys/clonefile.h>
#include <unistd.h>

using namespace Napi;

const int CLONE_OVERWRITE = 0x10000;

static const char *_errorToCode(const int error) {
  switch (error) {
  case EPERM:
    return "EPERM";
  case ENOENT:
    return "ENOENT";
  case ESRCH:
    return "ESRCH";
  case EINTR:
    return "EINTR";
  case EIO:
    return "EIO";
  case ENXIO:
    return "ENXIO";
  case E2BIG:
    return "E2BIG";
  case ENOEXEC:
    return "ENOEXEC";
  case EBADF:
    return "EBADF";
  case ECHILD:
    return "ECHILD";
  case EAGAIN:
    return "EAGAIN";
  case ENOMEM:
    return "ENOMEM";
  case EACCES:
    return "EACCES";
  case EFAULT:
    return "EFAULT";
  case EBUSY:
    return "EBUSY";
  case EEXIST:
    return "EEXIST";
  case EXDEV:
    return "EXDEV";
  case ENODEV:
    return "ENODEV";
  case ENOTDIR:
    return "ENOTDIR";
  case EISDIR:
    return "EISDIR";
  case EINVAL:
    return "EINVAL";
  case ENFILE:
    return "ENFILE";
  case EMFILE:
    return "EMFILE";
  case ENOTTY:
    return "ENOTTY";
  case ETXTBSY:
    return "ETXTBSY";
  case EFBIG:
    return "EFBIG";
  case ENOSPC:
    return "ENOSPC";
  case ESPIPE:
    return "ESPIPE";
  case EROFS:
    return "EROFS";
  case EMLINK:
    return "EMLINK";
  case EPIPE:
    return "EPIPE";
  case EDOM:
    return "EDOM";
  case ERANGE:
    return "ERANGE";
  default:
    return "UNKNOWN";
  }
}

class CloneWorker : public AsyncWorker {
public:
  CloneWorker(const Function &callback, const std::string &src,
              const std::string &dest, const int flags)
      : AsyncWorker(callback, "clonefile"), _src(src), _dest(dest),
        _flags(flags), _errno(0) {}

  ~CloneWorker() {}
  void Execute() override {
    const auto overwrite = this->_flags & CLONE_OVERWRITE;
    const auto flags = this->_flags & ~CLONE_OVERWRITE;
    auto ret = clonefile(this->_src.c_str(), this->_dest.c_str(), flags);
    if (ret != 0) {
      this->_errno = errno;
      if (this->_errno == EEXIST && overwrite) {
        unlink(this->_dest.c_str());
        ret = clonefile(this->_src.c_str(), this->_dest.c_str(), flags);
        if (ret == 0) {
          this->_errno = 0;
        } else {
          this->_errno = errno;
        }
      }
      if (this->_errno != 0) {
        const char *str_err = strerror(this->_errno);
        const std::string err = str_err == NULL ? "Unknown error" : str_err;
        SetError(err);
      }
    }
  }
  void OnError(const Napi::Error &error) override {
    HandleScope scope(Env());
    error.Set("errno", Number::New(Env(), this->_errno));
    error.Set("code", String::New(Env(), _errorToCode(this->_errno)));
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
