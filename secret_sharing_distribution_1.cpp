#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <numeric>
#include <boost/optional.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random.hpp>

using namespace boost::multiprecision;
using namespace std;

bool flag_obr = false;
cpp_int res_global_key;

void print_f_x(vector <cpp_int> f_elems, cpp_int K, cpp_int P_mod) {
    for (int i = 0; i < K; i++)
    {
        if (i == 0)
        {
            cout << f_elems[i] << " + ";
        }
        else if (i == 1 && K == 2) {
            cout << f_elems[i] << "x";
        }
        else if (i == 1) {
            cout << f_elems[i] << "x + ";
        }
        else if (i == K - 1) {
            cout << f_elems[i] << "x^" << i;
        }
        else {
            cout << f_elems[i] << "x^" << i << " + ";
        }
    }
    cout << " mod " << P_mod << endl;
}

cpp_int mod_otr(cpp_int x, cpp_int P_mod) {
    if (x < 0) {
        while (x < P_mod)
        {
            x += P_mod;
        }
    }
    return x % P_mod;
}

cpp_int rand_large(cpp_int n) {
    cpp_int x = n - 1;
    random_device gen;
    uniform_int_distribution<cpp_int> ui(1, x);
    cpp_int y = ui(gen);
    return y;
}

void find_n_dots_rand(vector <cpp_int> f_elems, int K, int N, vector <pair <cpp_int, cpp_int>>& dots_2, cpp_int P_mod) {
    vector <cpp_int> all_ch(N, 0);
    for (int w = 0; w < N; w++)
    {
        cpp_int rand_x = rand_large(P_mod);
        while (!check_unique(rand_x, all_ch, N) || rand_x == 0 || rand_x >= P_mod) {
            cpp_int rand_x = rand_large(P_mod);
        };
        all_ch.push_back(rand_x);
        vector <cpp_int> f_elems_dop(K);
        for (int i = 0; i < K; i++)
        {
            cpp_int dop = rand_x;
            if (i == 0) {
                f_elems_dop[i] = f_elems[i];
            }
            else if (i == 1) {
                f_elems_dop[i] = f_elems[i] * rand_x;
            }
            else {
                for (int j = 0; j < i - 1; j++)
                {
                    dop *= rand_x;
                }
                f_elems_dop[i] = f_elems[i] * dop;
            }
        }

        cpp_int res = 0;
        for (int i = 0; i < K; i++)
        {
            res += f_elems_dop[i];
        }
        dots_2[w] = make_pair(rand_x, mod_otr(res, P_mod));
    }
}

vector <vector < vector <cpp_int> > > res_x;
vector <cpp_int> x_for_check;

void shamir(cpp_int S, int N, int K, cpp_int P_mod) {

    vector <cpp_int> f_elems(K);
    f_elems[0] = S;
    for (int i = 1; i < K; i++)

    {
        f_elems[i] = rand() % 1000 + 1;
        mod_otr(f_elems[i], P_mod);
    }

    cout << "Полученная полиномиальная функция: ";
    print_f_x(f_elems, K, P_mod);

    vector <pair <cpp_int, cpp_int>> dots_2(N);
    find_n_dots_rand(f_elems, K, N, dots_2, P_mod);

    cout << endl << "Точки: " << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "Точка №" << i + 1 << ": ";
        cout << "( " << dots_2[i].first << "," << dots_2[i].second << " )" << endl;
    }

    for (int i = 0; i < N; i++)
    {
        x_for_check.push_back(dots_2[i].first);
    }

}

int main()
{

    setlocale(LC_ALL, "Rus");
    int N, K;
    cpp_int S, P_mod;
    cout << "Введите секрет: " << endl;
    cin >> S;
    cout << "Введите общее количество участников N: " << endl;
    cin >> N;
    cout << "Введите количество участников K < " << N << ", которое нужно для восстановления ключа : " << endl;
    cin >> K;
    cout << "Введите модуль p > " << max(S, cpp_int(N)) << ": " << endl;
    cin >> P_mod;
    shamir(S, N, K, P_mod);
    cout << "Модуль: P(0) = " << P_mod << endl;
    return 0;
}
