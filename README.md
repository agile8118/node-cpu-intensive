# Node.js Native Addons With C++

This project demonstrates performing a CPU-intensive operation—generating prime numbers—in Node.js using four different approaches. These methods are featured in the Multi-Threading section of the [Understanding Node.js: Core Concepts](https://www.udemy.com/course/understanding-nodejs-core-concepts/?referralCode=0BC21AC4DD6958AE6A95) course.

## Approaches

1. **Single Thread in C++**  
   We start by performing the operation using a single thread in C++.

2. **Multi-Threaded in C++**  
   Next, we execute it using multiple threads in C++.

3. **Single Thread in JavaScript**  
   Then, we run it in JavaScript with a single thread.

4. **Multi-Threaded in JavaScript**  
   Finally, we execute it in JavaScript using multiple threads.

## Test Results

Here’s a sample test with a `count` of 400 and `startingNumber` of 100,000,000,000,000:

```
------------------------------------------------
Starting Test 1...
Time Taken - C++ 1 Thread: 1128.6435419999998ms
------------------------------------------------

------------------------------------------------
Starting Test 2...
Time Taken - C++ 8 Threads: 153.3856249999999ms
------------------------------------------------

------------------------------------------------
Starting Test 3...
Time Taken - JS 1 Thread: 58657.762666ms
------------------------------------------------

------------------------------------------------
Starting Test 4...
Time Taken - JS 8 Threads: 8303.325124999996ms
------------------------------------------------
```

**Conclusion:**  
We made our app not 2 times, not 10 times, but 300 times faster! The operation went from taking 60 seconds to complete, down to only 0.2 seconds. This shows the importance of the responsibility of a software engineer, that bad design choices could end up costing a business a lot of money, and waste a lot of customers' time.

## Related Repositories

Here are 3 more related repositories that might interest you:

- [This project without GMP](https://github.com/agile8118/node-cpu-intensive-no-gmp)
- [This addon used in an HTTP web server to generate the numbers](https://github.com/agile8118/heavy-server-native)
- [The C++ part as a standalone project without Node.js](https://github.com/agile8118/prime-generator-cpp)

## How to Run the App

1. Build GMP: `./gmp.sh`
2. Install dependencies: `npm install`
3. Configure node-gyp: `npm run config`
4. Compile and run: `npm start`

## Notes

Ensure your machine is set up for C++ development. While the `gmp.sh` script handles GMP, you also need tools like CMake and Make installed. On Linux, you can install these tools with:

```
sudo apt install m4 cmake autoconf automake libtool build-essential
```

### Unable to Build & Link GMP?

On Linux, the `./gmp.sh` script might not properly set up the library with node-gyp. If that happens, install GMP manually:

```
sudo apt install libgmp-dev
```

Alternatively, you can check out the [node-cpu-intensive-no-gmp](https://github.com/agile8118/node-cpu-intensive-no-gmp) repository, which is the same project but without the GMP library. However, note that you won’t be able to use arbitrarily large numbers (larger than `unsigned long long`) for C++ tests.
