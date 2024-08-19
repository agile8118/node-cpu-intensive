#ifndef PRIME_GENERATOR_H
#define PRIME_GENERATOR_H

#include <vector>
#include <string>
#include <gmp.h>

// Function to check if a number is prime
bool isPrime(long long n);

// Function to check if a number larger than long long is prime
bool isPrime(mpz_t n);

// Function to find the next prime number after a given number
long long nextPrime(long long n);

// Function to find the next prime number after a given mpz_t number
void nextPrime(mpz_t result, const mpz_t n);

// Function to format a number with commas every three digits
std::string formatNumberWithCommas(long long number);

// Function to format an mpz_t number with commas every three digits
std::string formatNumberWithCommas(const mpz_t number);

// Function to generate a vector of prime numbers as strings
std::vector<std::string> generatePrimes(int count, long long startingNumber, bool format, bool log);

// Function to generate a vector of prime numbers as strings for mpz_t starting number
std::vector<std::string> generatePrimes(int count, const mpz_t startingNumber, bool format, bool log);

std::vector<std::string> generatePrimes(int count, const std::string startingNumber, bool format, bool log);

// Actual function to call when including the file
std::vector<std::string> generatePrimesThreads(int threadCount, int count, const std::string startingNumber, bool format, bool log);

#endif // PRIME_GENERATOR_H