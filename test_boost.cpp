// test_boost.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <boost/optional.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_int.hpp> 
#include <cassert>
#include <cassert>

using namespace std;
using namespace boost::multiprecision;
using namespace boost::random;

vector <int> first_primes_list = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
                     31, 37, 41, 43, 47, 53, 59, 61, 67,
                     71, 73, 79, 83, 89, 97, 101, 103,
                     107, 109, 113, 127, 131, 137, 139,
                     149, 151, 157, 163, 167, 173, 179,
                     181, 191, 193, 197, 199, 211, 223,
                     227, 229, 233, 239, 241, 251, 257,
                     263, 269, 271, 277, 281, 283, 293,
                     307, 311, 313, 317, 331, 337, 347, 349 };


//Выбор случайного главного кандидата
cpp_int rand_large(int n) {
    cpp_int x = pow(cpp_int(2), n);
    random_device gen;
    uniform_int_distribution<cpp_int> ui(1, x);
    cpp_int y = ui(gen);
    return y;
}

// возведение числа x в степень y по модулю p
cpp_int power(cpp_int x, cpp_int y, cpp_int p)
{
    cpp_int res = 1;      // Initialize result
    x = x % p;  // Update x if it is more than or
                // equal to p
    while (y > 0)
    {
        // If y is odd, multiply x with result
        if (y & 1)
            res = (res * x) % p;

        // y must be even now
        y = y >> 1; // y = y/2
        x = (x * x) % p;
    }
    return res;
}

//Деление с первыми простыми числами (тест на простоту низкого уровня) 
cpp_int getLowLevelPrime(int n) {
    while (true) {
        cpp_int prime_candidate = rand_large(n);
        for (int i = 0; i < first_primes_list.size(); i++)
        {
            int divisor = first_primes_list[i];
            if (prime_candidate % divisor == 0 && divisor * divisor <= prime_candidate) {
                break;
            }
            else {
                return prime_candidate;
            }
        }
    }
}


bool trialComposite(cpp_int round_tester, cpp_int evenComponent, cpp_int miller_rabin_candidate, int maxDivisionsByTwo) {
    if (power(round_tester, evenComponent,
        miller_rabin_candidate) == 1)
        return false;
    for (int i = 0; i < maxDivisionsByTwo; i++)
    {
        if (power(round_tester, pow(cpp_int(2), i) * evenComponent,
            miller_rabin_candidate) == miller_rabin_candidate - 1)
            return false;
    }
    return true;
}

//Тест на простоту Рабина Миллера (тест на простоту высокого уровня)  
bool isMillerRabinPassed(cpp_int miller_rabin_candidate) {
    int maxDivisionsByTwo = 0;
    cpp_int evenComponent = miller_rabin_candidate - 1;
    while (evenComponent % 2 == 0)
    {
        evenComponent >>= 1;
        maxDivisionsByTwo += 1;
    }
    if (!(pow(cpp_int(2), maxDivisionsByTwo) * evenComponent == miller_rabin_candidate - 1)) {
        cout << endl << "Что-то не так" << endl;
    }

    int numberOfRabinTrials = 20;
    for (int i = 0; i < numberOfRabinTrials; i++)
    {
        random_device gen;
        uniform_int_distribution<cpp_int> ui(2, miller_rabin_candidate);
        cpp_int round_tester = ui(gen);
        if (trialComposite(round_tester, evenComponent, miller_rabin_candidate, maxDivisionsByTwo)) {
            return false;
        }
    }
    return true;
}

int main()
{
    setlocale(LC_ALL, "rus");
    int n;
    cout << "Введите количество битов: ";
    cin >> n;
    bool flag_prime = true;
    while (flag_prime) {
        cpp_int prime_candidate = getLowLevelPrime(n);
        if (!isMillerRabinPassed(prime_candidate)) {
            continue;
        }
        else {
            cout << "Случайное простое число: " << prime_candidate << endl;
            flag_prime = false;
        }
    }
    return 0;
}