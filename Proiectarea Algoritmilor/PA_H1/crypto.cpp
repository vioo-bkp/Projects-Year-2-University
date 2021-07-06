#include<bits/stdc++.h>

using namespace std;

int main() {
	ifstream in("crypto.in");
    ofstream out("crypto.out");
    // Declaring vector of pairs
    vector<pair<int, int> > vect;
    int N, B;
    int total_cheltuieli = 0;
    in >> N >> B;

    // Initializing 1st and 2nd element of
    // pairs with array values
    int Pi[N], Ui[N];
    vect.reserve(N);

    for (int i = 0; i < N; ++i) {
        in >> Pi[i] >> Ui[i];
        vect.emplace_back(Pi[i], Ui[i]);
    }

    // Entering values in vector of pairs
    // Using simple sort() function to sort
    sort(vect.begin(), vect.end());

    for (int j = 0; j <= B; ++j) {
        for (int i = 0; i < N; ++i) {
            if (vect[0].first <= vect[1].first) {
                total_cheltuieli += vect[0].second;
                if (B >= total_cheltuieli) {
                    vect[0].first++;
                } else {
                    out << vect[0].first << endl;
                }
            }
           sort(vect.begin(), vect.end());
        }
    }

    in.close();
    out.close();
    return 0;
}
