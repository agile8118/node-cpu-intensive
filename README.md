This project is about doing a CPU-intensive operation in Node.js using 4 different ways.

First, we’ll perform the operation using a single thread in C++.
Next, we’ll execute it using multiple threads in C++.
Then, we’ll run it in JavaScript with a single thread.
Finally, we’ll execute it in JavaScript using multiple threads.

##### How to run the app:

1. First build GMP
2. Configure GYP: `node-gyp configure`
3. Compile the native code: `node-gyp build`
4. Finally run `npm start`

##### Steps to build GMP:

- cd GMP
- ./configure --enable-cxx --disable-shared
- make
- make check
