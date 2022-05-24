#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <numeric>

bool flag_obr = false;
long long res_global_key;

using namespace std;

bool prime(long long n) {
    for (long long i = 2; i <= sqrt(n); i++)
        if (n % i == 0)
            return false;
    return true;
}

void print_f_x(vector <long long> f_elems, long long K, long long P_mod) {
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

int mod_otr(long long x, long long P_mod) {
    if (x < 0) {
        while (x < P_mod)
        {
            x += P_mod;
        }
    }
    return x % P_mod;
}

int euclid(long long x, long long P_mod) {
    long long A = P_mod;
    long long B = x;
    bool flag = false;
    vector < vector <long long> > euclid_vecs;
    while (!flag) {
        long long del;
        if (A % B != 0) {
            del = A / B;
        }
        else {
            del = 0;
            flag = true;
        }
        euclid_vecs.push_back({ A, B, A % B, del });
        long long dop_A = A;
        A = B;
        B = dop_A % B;
    }
    if (euclid_vecs[euclid_vecs.size() - 1][1] != 1) {

        cout << endl << " ОЧЕНЬ ПЛОХО, ОБРАТНОГО НЕТ ДЛЯ P = " << P_mod << endl;
        flag_obr = true;
        return 0;
    }
    else {
        long long x_i = 0;
        long long y_i = 1;
        for (int i = 0; i < euclid_vecs.size() - 1; i++)
        {
            long long x_i_next = y_i;
            long long y_i_next = x_i - y_i * euclid_vecs[euclid_vecs.size() - i - 2][3];
            x_i = x_i_next;
            y_i = y_i_next;
        }
        return mod_otr(y_i, P_mod);
    }
}

void find_n_dots(vector <long long> f_elems, long long K, long long N, vector <pair <long long, long long>>& dots, long long P_mod) {
    for (int w = 1; w < N + 1; w++)
    {
        vector <long long> f_elems_dop(K);
        for (int i = 0; i < f_elems.size(); i++)
        {
            long long dop = w;
            if (i == 0) {
                f_elems_dop[i] = f_elems[i];
            }
            else if (i == 1) {
                f_elems_dop[i] = f_elems[i] * w;
            }
            else {
                for (int j = 0; j < i - 1; j++)
                {
                    dop *= w;
                }
                f_elems_dop[i] = f_elems[i] * dop;
            }
        }

        long long res = 0;
        for (int i = 0; i < K; i++)
        {
            res += f_elems_dop[i];
        }
        dots[w - 1] = make_pair(w, mod_otr(res, P_mod));
    }
}

void find_n_dots_rand(vector <long long> f_elems, long long K, long long N, vector <pair <long long, long long>>& dots_2, long long P_mod) {
    vector <bool> used(N + 100, false);
    for (int w = 0; w < N; w++)
    {
        long long rand_x = rand() % (N + 10) + 1;
        while (used[rand_x] || rand_x == 0 || rand_x >= P_mod) {
            rand_x = rand() % (N + 10) + 1;
        };
        used[rand_x] = true;
        vector <long long> f_elems_dop(K);
        for (int i = 0; i < K; i++)
        {
            long long dop = rand_x;
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

        long long res = 0;
        for (int i = 0; i < K; i++)
        {
            res += f_elems_dop[i];
        }
        dots_2[w] = make_pair(rand_x, mod_otr(res, P_mod));
    }
}

vector <vector < vector <long long> > > res_x;
vector <long long> x_for_check;

void shamir(long long S, long long N, long long K, long long P_mod) {
    //                                    составление секрета

    vector <long long> f_elems(K);
    f_elems[0] = S;
    for (int i = 1; i < K; i++)

    {
        //int random_variable = std::rand();
        f_elems[i] = rand() % 1000 + 1;
        mod_otr(f_elems[i], P_mod);
    }

        cout << "Полученное уравнение прямой: y = ";
        print_f_x(f_elems, K, P_mod);


    //x по номеру ключа
    vector <pair <long long, long long>> dots(N);
    find_n_dots(f_elems, K, N, dots, P_mod);

    // ОБЫЧНЫЕ

    //cout << endl << "Точки dots: " << endl;
    //for (int i = 0; i < N; i++)
    //{
    //    cout << "Точка №" << i + 1 << ": ";
    //    cout << "( " << dots[i].first << "," << dots[i].second << " )" << endl;
    //}

    //рандом x (+10)

    vector <pair <long long, long long>> dots_2(N);
    find_n_dots_rand(f_elems, K, N, dots_2, P_mod);

        cout << endl << "Точки dots_2: " << endl;
        for (int i = 0; i < N; i++)
        {
            cout << "Точка №" << i + 1 << ": ";
            cout << "( " << dots_2[i].first << "," << dots_2[i].second << " )" << endl;
        }

    for (int i = 0; i < N; i++)
    {
        x_for_check.push_back(dots_2[i].first);
    }

        cout << "Модуль: P(0) = " << P_mod << endl;

}

void generate_and_check_P_mod(long long S, long long N, long long K, long long P_mod) {
    while (!prime(P_mod)) {
        P_mod++;
    }
    shamir(S, N, K, P_mod);
    cout << "Модуль: P(0) = " << P_mod << endl;
}


int main()
{
    setlocale(LC_ALL, "Rus");
    long long S, N, K;
    cout << "Введите секрет: " << endl;
    cin >> S;
    cout << "Введите общее количество участников N: " << endl;
    cin >> N;
    cout << "Введите количество участников K < " << N << ", которое нужно для восстановления ключа : " << endl;
    cin >> K;
    //cout << "Введите модуль p > " << max(S, N) << ": " << endl;
    //cin >> P_mod;
    long long P_mod = rand() % RAND_MAX + max(S, N);
    generate_and_check_P_mod(S, N, K, P_mod);
    //shamir(S, N, K, P_mod);
    return 0;
}