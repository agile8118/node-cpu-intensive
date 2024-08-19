const { performance } = require("perf_hooks");
const { Worker } = require("worker_threads");
const path = require("path");
const util = require("util");
const { generatePrimes } = require("bindings")("heavy-lifter");
const generatePrimesJS = require("../lib/prime-generator"); // JavaScript version

const generatePrimesCPP = util.promisify(generatePrimes); // C++ version

const count = 400; // total number of primes numbers to generate
const startingNumber = "100000000000000"; // starting number to start generating primes from
const threads = 8;

// Convert the starting number to a BigInt if it's larger than Number.MAX_SAFE_INTEGER
let startingNumberJS = BigInt(startingNumber);
if (startingNumberJS < Number.MAX_SAFE_INTEGER) {
  startingNumberJS = Number(startingNumber);
}

(async () => {
  // ------------------ TEST 1 ------------------------
  // Generating the primes with C++ in 1 thread
  // --------------------------------------------------
  console.log("Starting Test 1...");
  const start1 = performance.now();
  const primes1 = await generatePrimesCPP(count, startingNumber, {
    format: true,
    log: false,
  });
  console.log(`Time Taken - C++ 1 Thread: ${performance.now() - start1}ms\n\n`);

  // ------------------ TEST 2 ------------------------
  // Generating the primes with C++ in multiple threads
  // --------------------------------------------------
  console.log("Starting Test 2...");
  const start2 = performance.now();
  const primes2 = await generatePrimesCPP(count, startingNumber, {
    format: true,
    threads,
  });
  console.log(
    `Time Taken - C++ ${threads} Threads: ${performance.now() - start2}ms\n\n`
  );

  // ------------------ TEST 3 ------------------------
  // Generating the primes with JavaScript in 1 thread
  // --------------------------------------------------
  console.log("Starting Test 3...");
  const start3 = performance.now();
  const primes3 = generatePrimesJS(count, startingNumberJS, {
    format: true,
  });
  console.log(`Time Taken - JS 1 Thread: ${performance.now() - start3}ms\n\n`);

  // ------------------ TEST 4 ------------------------
  // Generating the primes with JavaScript in 8 thread
  // --------------------------------------------------
  console.log("Starting Test 4...");
  let primes4 = [];
  let completed = 0;
  const start4 = performance.now();

  for (let i = 0; i < threads; i++) {
    const worker = new Worker(path.resolve(__dirname, "worker.js"), {
      workerData: {
        count: count / threads,
        start:
          typeof startingNumberJS === "bigint"
            ? startingNumberJS + BigInt(i * 3000)
            : startingNumberJS + i * 3000,
      },
    });

    const threadId = worker.threadId;

    worker.on("message", (primes) => {
      primes4 = primes4.concat(primes);
    });

    worker.on("error", (err) => {
      console.error(err);
    });

    worker.on("exit", (code) => {
      completed++;

      if (completed === threads) {
        console.log(
          `Time Taken - JS ${threads} Threads: ${
            performance.now() - start4
          }ms\n\n`
        );
      }

      if (code !== 0) {
        console.error(`Worker ${threadId} exited with code ${code}`);
      }
    });
  }
})();
