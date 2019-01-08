#include "combine.h"

Combine::Combine(int _n, int _k): n(_n),k(_k) {};

int Combine::init(void)
{
    counter = 0;
    if (k > n) return GEN_ERRO;
    if (k == 0) return GEN_VOID;
    for(int j = 0; j < k; j++)
        index.push_back(j);
    counter++;
    return GEN_NEXT;
}

int Combine::next(void)
{
    for (int i=k-1; i>=0; --i)
        if (index[i] < n-k+i) {
            ++index[i];
            for (int j=i+1; j<k; ++j)
                index[j] = index[j-1]+1;
            counter++;
            return GEN_NEXT;
        }
    return GEN_STOP;
}
