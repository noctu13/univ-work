#ifndef COMBINE_H
#define COMBINE_H

#define GEN_NEXT 0
#define GEN_STOP 1
#define GEN_VOID 2
#define GEN_ERRO 3

#include <vector>

class Combine
{
public:
    std::vector<int> index;
    int n, k, counter;
    Combine(int, int);
    int init(void);
    int next(void);
};

#endif // COMBINE_H

