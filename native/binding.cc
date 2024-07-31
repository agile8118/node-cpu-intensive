// #include <napi.h>
// #include "prime-generator.h"

// class GeneratePrimesWrapper : public Napi::AsyncWorker {
// public:
//   GeneratePrimesWrapper(Napi::Function& callback, int count, std::string startingNumber, bool format, bool log)
//     : Napi::AsyncWorker(callback), count(count), startingNumber(startingNumber), format(format), log(log) {}

//   // This function will be executed on another thread
//   void Execute() override {
//     // Here we should check the thread options coming from node.js, if one call generatePrimes, if more than one call generatePrimesThreads
//     primes = generatePrimesThreads(10, count, startingNumber, format, log);
//   }

//   void OnOK() override {
//     Napi::Env env = Env();
//     Napi::Array jsPrimes = Napi::Array::New(env, primes.size());

//     for (size_t i = 0; i < primes.size(); ++i) {
//       jsPrimes[i] = Napi::String::New(env, primes[i]);
//     }

//     Callback().Call({ env.Null(), jsPrimes });
//   }

// private:
//   int count;
//   std::string startingNumber;
//   bool format;
//   bool log;
//   std::vector<std::string> primes;
// };

// Napi::Value add(const Napi::CallbackInfo& info) {
//   Napi::Env env = info.Env();

//   if (info.Length() < 2) {
//     Napi::TypeError::New(env, "Expected two arguments").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   if (!info[0].IsNumber() || !info[1].IsNumber()) {
//     Napi::TypeError::New(env, "Expected both arguments to be numbers").ThrowAsJavaScriptException();
//     return env.Null();
//   }

//   int arg1 = info[0].As<Napi::Number>().Int32Value();
//   int arg2 = info[1].As<Napi::Number>().Int32Value();
//   int sum = arg1 + arg2;

//   return Napi::Number::New(env, sum);
// }


// Napi::Object Init(Napi::Env env, Napi::Object exports) {
//   // First export
//   exports.Set(Napi::String::New(env, "generatePrimes"),
//     Napi::Function::New(env, [](const Napi::CallbackInfo& info) -> Napi::Value {
//       Napi::Env env = info.Env();

//       int count = info[0].As<Napi::Number>().Int32Value();
//       std::string startingNumber = info[1].As<Napi::String>();
//       bool format = info[2].As<Napi::Boolean>();
//       bool log = info[3].As<Napi::Boolean>();
//       Napi::Function callback = info[4].As<Napi::Function>();

//       GeneratePrimesWrapper* generatePrimes = new GeneratePrimesWrapper(callback, count, startingNumber, format, log);
//       generatePrimes->Queue();
//       return env.Undefined();
//       }));

//   // Second export
//   exports.Set(Napi::String::New(env, "add"),
//     Napi::Function::New(env, add));

//   return exports;
// }

// NODE_API_MODULE("heavy-lifter", Init)

#include <napi.h>
#include "prime-generator.h"

class GeneratePrimesWrapper : public Napi::AsyncWorker {
public:
  GeneratePrimesWrapper(Napi::Function& callback, int count, std::string startingNumber, bool format, bool log, int threads)
    : Napi::AsyncWorker(callback), count(count), startingNumber(startingNumber), format(format), log(log), threads(threads) {}

  void Execute() override {
    if (threads > 1) {
      primes = generatePrimesThreads(threads, count, startingNumber, format, log);
    }
    else {
      primes = generatePrimes(count, startingNumber, format, log);
    }
  }

  void OnOK() override {
    Napi::Env env = Env();
    Napi::Array jsPrimes = Napi::Array::New(env, primes.size());

    for (size_t i = 0; i < primes.size(); ++i) {
      jsPrimes[i] = Napi::String::New(env, primes[i]);
    }

    Callback().Call({ env.Null(), jsPrimes });
  }

private:
  int count;
  std::string startingNumber;
  bool format;
  bool log;
  int threads;
  std::vector<std::string> primes;
};

Napi::Value GeneratePrimesBind(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 3 || info.Length() > 4) {
    Napi::TypeError::New(env, "Expected three or four arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsString() || !info[info.Length() - 1].IsFunction()) {
    Napi::TypeError::New(env, "Invalid arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  int count = info[0].As<Napi::Number>().Int32Value();
  std::string startingNumber = info[1].As<Napi::String>();
  Napi::Function callback = info[info.Length() - 1].As<Napi::Function>();

  bool format = false;
  bool log = false;
  int threads = 1;

  if (info.Length() == 4 && info[2].IsObject()) {
    Napi::Object options = info[2].As<Napi::Object>();
    format = options.Has("format") ? options.Get("format").As<Napi::Boolean>() : false;
    log = options.Has("log") ? options.Get("log").As<Napi::Boolean>() : false;
    threads = options.Has("threads") ? options.Get("threads").As<Napi::Number>().Int32Value() : 1;
  }

  GeneratePrimesWrapper* generatePrimes = new GeneratePrimesWrapper(callback, count, startingNumber, format, log, threads);
  generatePrimes->Queue();
  return env.Undefined();
}

// Just a simple function that adds two numbers, used for testing
Napi::Value Add(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected two arguments").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsNumber() || !info[1].IsNumber()) {
    Napi::TypeError::New(env, "Expected both arguments to be numbers").ThrowAsJavaScriptException();
    return env.Null();
  }

  int arg1 = info[0].As<Napi::Number>().Int32Value();
  int arg2 = info[1].As<Napi::Number>().Int32Value();
  int sum = arg1 + arg2;

  return Napi::Number::New(env, sum);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  // First export
  exports.Set(Napi::String::New(env, "generatePrimes"),
    Napi::Function::New(env, GeneratePrimesBind));

  // Second export (for testing)
  exports.Set(Napi::String::New(env, "add"),
    Napi::Function::New(env, Add));

  return exports;
}

NODE_API_MODULE("heavy-lifter", Init)