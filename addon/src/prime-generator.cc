#include <iostream>
#include <climits>
#include <iomanip>
#include <stdbool.h>
#include <thread>
#include <mutex>
#include <algorithm>
#include "prime-generator.h"

// Function to check if a number is prime
bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Function to check if a number larger than long long is prime
bool isPrime(mpz_t n) {
    if (mpz_cmp_ui(n, 1) <= 0) return false;
    if (mpz_cmp_ui(n, 3) <= 0) return true;

    mpz_t mod_result, i, limit;
    mpz_inits(mod_result, i, limit, NULL);

    // Check if n is divisible by 2 or 3
    mpz_mod_ui(mod_result, n, 2);
    if (mpz_cmp_ui(mod_result, 0) == 0) {
        mpz_clears(mod_result, i, limit, NULL);
        return false;
    }
    mpz_mod_ui(mod_result, n, 3);
    if (mpz_cmp_ui(mod_result, 0) == 0) {
        mpz_clears(mod_result, i, limit, NULL);
        return false;
    }

    mpz_set_ui(i, 5);
    mpz_sqrt(limit, n); // limit = sqrt(n)
    mpz_add_ui(limit, limit, 1); // limit = sqrt(n) + 1

    while (mpz_cmp(i, limit) <= 0) {
        mpz_mod(mod_result, n, i);
        if (mpz_cmp_ui(mod_result, 0) == 0) {
            mpz_clears(mod_result, i, limit, NULL);
            return false;
        }

        mpz_add_ui(mod_result, i, 2); // mod_result = i + 2
        mpz_mod(mod_result, n, mod_result);
        if (mpz_cmp_ui(mod_result, 0) == 0) {
            mpz_clears(mod_result, i, limit, NULL);
            return false;
        }

        mpz_add_ui(i, i, 6); // i += 6
    }

    mpz_clears(mod_result, i, limit, NULL);
    return true;
}

// Function to find the next prime number after a given number
long long nextPrime(long long n) {
    if (n <= 1) return 2;
    long long prime = n;
    bool found = false;

    while (!found) {
        prime++;
        if (isPrime(prime)) {
            found = true;
        }
    }
    return prime;
}

void nextPrime(mpz_t result, const mpz_t n) {
    if (mpz_cmp_ui(n, 1) <= 0) {
        mpz_set_ui(result, 2);
        return;
    }

    mpz_set(result, n);

    bool found = false;
    while (!found) {
        mpz_add_ui(result, result, 1); // result++
        if (isPrime(result)) {
            found = true;
        }
    }
}

// Function to format a number with commas every three digits
std::string formatNumberWithCommas(long long number) {
    std::string num_str = std::to_string(number);
    int insert_position = num_str.length() - 3;
    while (insert_position > 0) {
        num_str.insert(insert_position, ",");
        insert_position -= 3;
    }
    return num_str;
}

std::string formatNumberWithCommas(const mpz_t number) {
    // Convert the mpz_t number to a string
    char* num_str_c = mpz_get_str(NULL, 10, number);
    std::string num_str(num_str_c);

    // Free the memory allocated by mpz_get_str
    free(num_str_c);

    int insert_position = num_str.length() - 3;
    while (insert_position > 0) {
        num_str.insert(insert_position, ",");
        insert_position -= 3;
    }
    return num_str;
}

// Function to generate primes
std::vector<std::string> generatePrimes(int count, long long startingNumber, bool format, bool log) {
    std::vector<std::string> primes;

    long long prime = nextPrime(startingNumber);
    primes.push_back(format ? formatNumberWithCommas(prime) : std::to_string(prime));

    for (int i = 1; i < count; i++) {
        prime = nextPrime(prime);
        std::string prime_str = format ? formatNumberWithCommas(prime) : std::to_string(prime);
        if (log) std::cout << prime_str << std::endl;
        primes.push_back(prime_str);
    }

    return primes;
}

std::vector<std::string> generatePrimes(int count, const mpz_t startingNumber, bool format, bool log) {
    std::vector<std::string> primes;
    mpz_t prime, next;
    mpz_init_set(prime, startingNumber);
    mpz_init(next);

    nextPrime(next, prime);
    primes.push_back(format ? formatNumberWithCommas(next) : mpz_get_str(NULL, 10, next));

    for (int i = 1; i < count; i++) {
        nextPrime(next, next);
        std::string prime_str = format ? formatNumberWithCommas(next) : mpz_get_str(NULL, 10, next);
        if (log) std::cout << prime_str << std::endl;
        primes.push_back(prime_str);
    }

    mpz_clears(prime, next, NULL);
    return primes;
}

std::vector<std::string> generatePrimes(int count, const std::string startingNumber, bool format, bool log) {
    mpz_t gmp_start;
    mpz_init_set_str(gmp_start, startingNumber.c_str(), 10);

    mpz_t max_long_long;
    mpz_init_set_str(max_long_long, std::to_string(LLONG_MAX).c_str(), 10);

    if (mpz_cmp(gmp_start, max_long_long) <= 0) {
        // std::cout << "Going with long long..." << std::endl;
        long long ll_start = mpz_get_ui(gmp_start);
        mpz_clear(gmp_start);
        return generatePrimes(count, ll_start, format, log);
    }
    else {
        // std::cout << "Going with GMP..." << std::endl;
        return generatePrimes(count, gmp_start, format, log);
    }
}

// Actual function to call when including the file
std::vector<std::string> generatePrimesThreads(int threadCount, int primeCount, const std::string startValue, bool format, bool log) {
    std::vector<std::thread> threads;
    std::vector<std::string> result;
    std::mutex resultMutex;

    auto calc = [&](int count, const std::string& start, bool format, bool log) {
        // Generate primes
        auto primes = generatePrimes(count, start, format, log);
        // Lock the mutex before modifying the shared result vector
        {
            std::lock_guard<std::mutex> lock(resultMutex);
            result.insert(result.end(), primes.begin(), primes.end());
        }
        };

    for (int i = 0; i < threadCount; ++i) {
        mpz_t gmpValue, temp;
        mpz_init(gmpValue);
        mpz_init(temp);

        // Convert startValue to GMP
        mpz_set_str(gmpValue, startValue.c_str(), 10);

        // Add i to GMP value
        mpz_add_ui(gmpValue, gmpValue, i);

        // Now that we have the startValue in GMP, add 3000 to it
        mpz_set_ui(temp, i * 3000);
        mpz_mul_ui(temp, temp, i);
        mpz_add(gmpValue, gmpValue, temp);

        // Convert the startValue back to string before passing it to the thread
        char* newStartValue = mpz_get_str(nullptr, 10, gmpValue);
        std::string updatedStartValue(newStartValue);
        free(newStartValue);

        // Start the worker thread
        threads.push_back(std::thread(calc, primeCount / threadCount, updatedStartValue, format, log));

        mpz_clear(gmpValue);
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }


    // Sort the results
    std::sort(result.begin(), result.end());

    return result;
}