#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <numeric>

bool flag_obr = false;
long long res_global_key;

using namespace std;

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
        cout << endl << " ОЧЕНЬ ПЛОХО, ОБРАТНОГО НЕТ " << endl;
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

vector <vector < vector <long long> > > res_x;

void all_multiplies(long long in_K, vector <long long> s, long long P_mod) {
    long long s_size = s.size();
    set <pair <long long, long long>> start;
    set < set <pair <long long, long long> > > el_set;
    for (int i = 0; i < s_size; i++)
    {
        pair <long long, long long> x = make_pair(s[i], i);
        set <pair <long long, long long> > dop_set;
        dop_set.insert(x);
        start.insert(x);
        el_set.insert(dop_set);
    }
    long long count = 0;
    while (count != s_size - 1)
    {
        set < set <pair <long long, long long> > > help_set;
        for (pair <long long, long long> d : start)
        {
            for (set < pair <long long, long long> > d_set : el_set)
            {
                set < pair <long long, long long> > new_d_set = d_set;
                new_d_set.insert(d);
                help_set.insert(new_d_set);

            }
        }
        for (set < pair <long long, long long> > d_set : help_set)
        {
            el_set.insert(d_set);
        }
        count++;
    }
    vector < vector <long long> > res_multi(s_size + 1);
    //for (set < pair <int, int> > d : el_set)
    //{
    //    cout << d.size() << " ";
    //    for (pair <int, int> d_set : d)
    //    {
    //        cout << "( " << d_set.first << " , " << d_set.second << " ),";
    //    }
    //    cout << endl;
    //}
    for (set < pair <long long, long long> > d : el_set)
    {
        int mult = 1;
        for (pair <long long, long long> d_set : d)
        {
            mult *= d_set.first;
        }
        if (mult != 0) {
            res_multi[s_size - d.size()].push_back(mod_otr(mult, P_mod));
        }
    }

    long long ed = 1;
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

int nod(long long x, long long y) {
    return (x ? nod(y % x, x) : y);
}

void secret_recovery(long long K, vector <pair <long long, long long>>& dots, long long P_mod) {

    vector <pair <long long, long long>> rec_dots = dots;

    cout << endl << "Восстановление ключа будет происходить с помощью следующих точек: " << endl;
    for (int i = 0; i < K; i++)
    {
        cout << "Точка №" << i + 1 << ": ";
        cout << "( " << rec_dots[i].first << "," << rec_dots[i].second << " )" << endl;
    }

    //восстанавливать буду с помощью интерполяционного полинома Лагранжа

    /*
    * (x_1, y_1), (x_2, y_2), (x_3, y_3), ... (содержатся в rec_dots)
    *
    *             k
    *     P(x) = SUM (p_j(x)) mod p
    *            j=1
    *
    *
    *                      k      x - x_m
    *     p_j(x) = y_j *  MULT ------------- mod p
    *                     m=1    x_j - x_m
    *                     m!=j
    *
    *
    */

    //vector <vector <pair <int, int>>> P_x(K); // весь P(x)
    //vector <int> p;
    vector < pair < vector <pair <long long, long long> >, long long> > P_x(K);  // first числитель: x - first (x_m)
                                                               // first знаменатель: second (x_j - x_m)
                                                              // second y_j
    for (int j = 0; j < K; j++)
    {
        long long res_Pj;
        vector <pair <long long, long long> > p_j_first;
        for (int m = 0; m < K; m++)
        {
            pair <long long, long long> pj_pair_dop;
            if (m != j) {
                pj_pair_dop = make_pair(rec_dots[m].first, mod_otr(rec_dots[j].first - rec_dots[m].first, P_mod));
                p_j_first.push_back(pj_pair_dop);
            }
        }
        pair < vector <pair <long long, long long> >, long long> pj_pair;
        pj_pair = make_pair(p_j_first, rec_dots[j].second);
        //rec_dots[i].second * res_Pj;
        P_x[j] = pj_pair;
    }
    //for (int i = 0; i < K; i++)
    //{
    //    P_x[i].second = P_x[i].second / ;
    //}
    //vector <vector <int> > degrees(K);
    //for (int i = 0; i < K; i++)
    //{
    //    for (int x = 0; x < P_x.size(); x++)
    //    {
    //        for (int r = 0; r < P_x[x].first.size(); r++)
    //        {
    //            degrees[i].push_back(P_x[x].first[r].first);
    //        }
    //            
    //    }

    //}
    vector < vector <vector <long long> > > all_p;
    vector < pair <long long, long long> > all_const(K);
    res_x.resize(K);
    for (int i = 0; i < K; i++)
    {
        long long znam_mult = 1;
        for (int j = 0; j < P_x[i].first.size(); j++)
        {
            znam_mult *= P_x[i].first[j].second;
        }
        pair <long long, long long> v_const = make_pair(mod_otr(P_x[i].second, P_mod), mod_otr(znam_mult, P_mod));
        all_const[i] = v_const;
        vector <long long> x_ms;
        for (int j = 0; j < P_x[i].first.size(); j++)
        {
            int dd = mod_otr(P_x[i].first[j].first * -1, P_mod);
            if (dd != 0) {
                x_ms.push_back(dd); // если что чекнуть
            }
        }
        all_multiplies(i, x_ms, P_mod);

    }
    //Сейчас имеем константы v_const и сомножители res_x
    for (int i = 0; i < K; i++)
    {
        for (int j = 0; j < res_x[i].size(); j++)
        {
            int sum_in_res_x = 0;
            for (int h = 0; h < res_x[i][j].size(); h++)
            {
                sum_in_res_x += res_x[i][j][h];
            }
            res_x[i][j].clear();
            res_x[i][j].push_back(mod_otr(sum_in_res_x, P_mod));
        }
    }
    //ВЫВЕДЕМ КОНСТАНТЫ СПЕРЕДИ
    for (int i = 0; i < all_const.size(); i++)
    {
        cout << "Верхняя константа для точки №" << i + 1 << ": ";
        cout << all_const[i].first << endl;
        cout << "Нижняя константа для точки №" << i + 1 << ": ";
        cout << all_const[i].second << endl;
    }
    //умножим верхнюю часть константы на сомножители
    for (int i = 0; i < K; i++)
    {
        long long up_const = all_const[i].first;
        long long down_const = all_const[i].second;
        for (int j = 0; j < res_x[i].size(); j++)
        {
            res_x[i][j][0] *= up_const;
            res_x[i][j][0] = mod_otr(res_x[i][j][0], P_mod);
            res_x[i][j][0] *= euclid(mod_otr(down_const, P_mod), P_mod); // обратный эл. над конечным полем
            res_x[i][j][0] = mod_otr(res_x[i][j][0], P_mod);
        }
    }
    vector <long long> res_p_x(K);
    for (int j = 0; j < K; j++) // по всем степеням
    {
        int res_sum = 0;
        for (int j2 = 0; j2 < K; j2++)
        {
            res_sum += res_x[j2][j][0];
        }
        res_p_x[j] = mod_otr(res_sum, P_mod);
    }
    cout << endl << "Исходная полиномиальная функция: " << endl;

    print_f_x(res_p_x, K, P_mod);
    //ИЩЕМ НОД ДЛЯ ЭТИХ ЧИСЕЛ

    //int nd = res_p_x[0];
    //for (int i = 1; i < K; i++) nd = nod((nd < res_p_x[i] ? nd : res_p_x[i]), (nd < res_p_x[i] ? res_p_x[i] : nd));
    //cout << endl << "NOD = " << nd << endl;
    //if (nd != 1) {
    //    for (int i = 0; i < res_p_x.size(); i++)
    //    {
    //        res_p_x[i] /= nd;
    //    }
    //}


    // восстановим по точке 0
    long long res_key = res_p_x[0];

    //for (int i = 1; i < res_p_x.size(); i++)
    //{
    //    int this_e = res_p_x[i];
    //    for (int j = 0; j < i; j++)
    //    {
    //        this_e *= 0;
    //    }
    //}
    res_global_key = res_key;
    cout << endl << "Итоговый ключ: P(0) = " << res_key << endl;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    long long S, N, K, P_mod;
    cout << "Введите количество участников K: " << endl;
    cin >> K;
    cout << "Введите модуль P: " << endl;
    cin >> P_mod;
    vector < pair < long long, long long > > dots_for_recovery;
    cout << "Введите " << K << " точек (так: x y): " << endl;
    for (int i = 0; i < K; i++) {
        long long vvod_R1;
        long long vvod_R2;
        cout << "Точка №" << i + 1 << " :";
        cin >> vvod_R1 >> vvod_R2;
        dots_for_recovery.push_back(make_pair(vvod_R1, vvod_R2));
        cout << endl;
    }
    secret_recovery(K, dots_for_recovery, P_mod);
    return 0;
}
