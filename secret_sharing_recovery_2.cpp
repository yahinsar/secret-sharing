#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <numeric>
#include <boost/optional.hpp>
#include <boost/multiprecision/cpp_int.hpp> 


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


cpp_int euclid(cpp_int x, cpp_int P_mod) {
    cpp_int A = P_mod;
    cpp_int B = x;
    bool flag = false;
    vector < vector <cpp_int> > euclid_vecs;
    while (!flag) {
        cpp_int del;
        if (A % B != 0) {
            del = A / B;
        }
        else {
            del = 0;
            flag = true;
        }
        euclid_vecs.push_back({ A, B, A % B, del });
        cpp_int dop_A = A;
        A = B;
        B = dop_A % B;
    }
    if (euclid_vecs[euclid_vecs.size() - 1][1] != 1) {
        cout << endl << "Введенный модуль не является простым числом " << endl;
        flag_obr = true;
        return 0;
    }
    else {
        cpp_int x_i = 0;
        cpp_int y_i = 1;
        for (int i = 0; i < euclid_vecs.size() - 1; i++)
        {
            cpp_int x_i_next = y_i;
            cpp_int y_i_next = x_i - y_i * euclid_vecs[euclid_vecs.size() - i - 2][3];
            x_i = x_i_next;
            y_i = y_i_next;
        }
        return mod_otr(y_i, P_mod);
    }
}

vector <vector < vector <cpp_int> > > res_x;

void all_multiplies(int in_K, vector <cpp_int> s, cpp_int P_mod) {
    int s_size = s.size();
    set <pair <cpp_int, cpp_int>> start;
    set < set <pair <cpp_int, cpp_int> > > el_set;
    for (int i = 0; i < s_size; i++)
    {
        pair <cpp_int, cpp_int> x = make_pair(s[i], i);
        set <pair <cpp_int, cpp_int> > dop_set;
        dop_set.insert(x);
        start.insert(x);
        el_set.insert(dop_set);
    }
    cpp_int count = 0;
    while (count != s_size - 1)
    {
        set < set <pair <cpp_int, cpp_int> > > help_set;
        for (pair <cpp_int, cpp_int> d : start)
        {
            for (set < pair <cpp_int, cpp_int> > d_set : el_set)
            {
                set < pair <cpp_int, cpp_int> > new_d_set = d_set;
                new_d_set.insert(d);
                help_set.insert(new_d_set);

            }
        }
        for (set < pair <cpp_int, cpp_int> > d_set : help_set)
        {
            el_set.insert(d_set);
        }
        count++;
    }
    vector < vector <cpp_int> > res_multi(s_size + 1);
    for (set < pair <cpp_int, cpp_int> > d : el_set)
    {
        cpp_int mult = 1;
        for (pair <cpp_int, cpp_int> d_set : d)
        {
            mult *= d_set.first;
        }
        if (mult != 0) {
            res_multi[s_size - d.size()].push_back(mod_otr(mult, P_mod));
        }
    }

    cpp_int ed = 1;
    res_multi[s_size].push_back(ed);
    cout << "для " << in_K + 1 << "-й точки: " << endl;
    for (int i = 0; i < res_multi.size(); i++)
    {
        cout << "x^" << i << " = ";
        for (int j = 0; j < res_multi[i].size(); j++)
        {
            cout << res_multi[i][j] << ", ";
        }
        cout << endl;
    }
    res_x[in_K] = res_multi;

}

void secret_recovery(int K, vector <pair <cpp_int, cpp_int>>& dots, cpp_int P_mod) {

    vector <pair <cpp_int, cpp_int>> rec_dots = dots;

    cout << endl << "Восстановление ключа будет происходить с помощью следующих точек: " << endl;
    for (int i = 0; i < K; i++)
    {
        cout << "Точка №" << i + 1 << ": ";
        cout << "( " << rec_dots[i].first << "," << rec_dots[i].second << " )" << endl;
    }

    vector < pair < vector <pair <cpp_int, cpp_int> >, cpp_int> > P_x(K);

    for (int j = 0; j < K; j++)
    {
        cpp_int res_Pj;
        vector <pair <cpp_int, cpp_int> > p_j_first;
        for (int m = 0; m < K; m++)
        {
            pair <cpp_int, cpp_int> pj_pair_dop;
            if (m != j) {
                pj_pair_dop = make_pair(rec_dots[m].first, mod_otr(rec_dots[j].first - rec_dots[m].first, P_mod));
                p_j_first.push_back(pj_pair_dop);
            }
        }
        pair < vector <pair <cpp_int, cpp_int> >, cpp_int> pj_pair;
        pj_pair = make_pair(p_j_first, rec_dots[j].second);
        P_x[j] = pj_pair;
    }

    vector < vector <vector <cpp_int> > > all_p;
    vector < pair <cpp_int, cpp_int> > all_const(K);
    res_x.resize(K);
    for (int i = 0; i < K; i++)
    {
        cpp_int znam_mult = 1;
        for (int j = 0; j < P_x[i].first.size(); j++)
        {
            znam_mult *= P_x[i].first[j].second;
        }
        pair <cpp_int, cpp_int> v_const = make_pair(mod_otr(P_x[i].second, P_mod), mod_otr(znam_mult, P_mod));
        all_const[i] = v_const;
        vector <cpp_int> x_ms;
        for (int j = 0; j < P_x[i].first.size(); j++)
        {
            cpp_int dd = mod_otr(P_x[i].first[j].first * -1, P_mod);
            if (dd != 0) {
                x_ms.push_back(dd);
            }
        }
        all_multiplies(i, x_ms, P_mod);

    }
    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < res_x[i].size(); j++)
        {
            cpp_int sum_in_res_x = 0;
            for (int h = 0; h < res_x[i][j].size(); h++)
            {
                sum_in_res_x += res_x[i][j][h];
            }
            res_x[i][j].clear();
            res_x[i][j].push_back(mod_otr(sum_in_res_x, P_mod));
        }
    }

    for (int i = 0; i < K; i++)
    {
        cpp_int up_const = all_const[i].first;
        cpp_int down_const = all_const[i].second;
        for (int j = 0; j < res_x[i].size(); j++)
        {
            res_x[i][j][0] *= up_const;
            res_x[i][j][0] = mod_otr(res_x[i][j][0], P_mod);
            res_x[i][j][0] *= euclid(mod_otr(down_const, P_mod), P_mod); 
            res_x[i][j][0] = mod_otr(res_x[i][j][0], P_mod);
        }
    }
    vector <cpp_int> res_p_x(K);
    for (int j = 0; j < K; j++) 
    {
        cpp_int res_sum = 0;
        for (int j2 = 0; j2 < K; j2++)
        {
            res_sum += res_x[j2][j][0];
        }
        res_p_x[j] = mod_otr(res_sum, P_mod);
    }
    cout << endl << "Исходная полиномиальная функция: " << endl;

    print_f_x(res_p_x, K, P_mod);

    cpp_int res_key = res_p_x[0];

    res_global_key = res_key;
    cout << endl << "Полученный секрет: P(0) = " << res_key << endl;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    int N, K;
    cpp_int S, P_mod;
    cout << "Введите количество участников K: " << endl;
    cin >> K;
    cout << "Введите модуль P: " << endl;
    cin >> P_mod;
    vector < pair < cpp_int, cpp_int > > dots_for_recovery;
    cout << "Введите " << K << " точек (так: x y): " << endl;
    for (int i = 0; i < K; i++) {
        cpp_int vvod_R1;
        cpp_int vvod_R2;
        cout << "Точка №" << i + 1 << " :";
        cin >> vvod_R1 >> vvod_R2;
        dots_for_recovery.push_back(make_pair(vvod_R1, vvod_R2));
        cout << endl;
    }
    secret_recovery(K, dots_for_recovery, P_mod);
    return 0;
}

