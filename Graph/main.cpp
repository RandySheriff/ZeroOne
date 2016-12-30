#include "Graph.h"
#include <iostream>
using namespace std;

template<class T>
int BinarySearch(T* addr, size_t sz, const T& key)
{
    if (nullptr == addr || sz == 0)
    {
        return -1;
    }

    auto st = addr;
    auto ed = addr + sz;

    while (st < ed)
    {
        auto md = st + ((ed - st) >> 1);

        if (key == *md)
        {
            return md - addr;
        }
        else if (key > *md)
        {
            st = md + 1;
        }
        else
        {
            ed = md;
        }
    }

    return -1;
}

int main()
{
    /*
    int A[][3] = {
                  {0,1,1},
                  {2,0,2},
                  {3,3,0}
                 };
    */

    int A[][4] = {
                  {-1, 1, 3, 7},
                  {-1,-1,-1, 1},
                  {-1,-1,-1, 5},
                  {-1,-1,-1,-1}
                 };

    std::function<bool(const int&)> HasEdge = [] (const int& value) { return value != 0;};

    auto graph = Graph<int>::CreateFromArray((void*)A, sizeof(A)/sizeof(A[0]), A[0][0], HasEdge);

    graph->PrintGraph(-1);

    std::function<bool(const int&, const int&)> IsSmaller = [] (const int& v1, const int& v2) { return v1 < v2;};

    std::cout << graph->FindShortestPathByAStarSearch(0, 3, -1, IsSmaller) << std::endl;

    /*
    double A[][3] = {{0.0,1.1,1.1},{2.2,0.0,2.2},{3.3,3.3,0.0}};

    std::function<bool(const double&)> HasEdge = [] (const int& value) { return value > 0.0;};

    auto graph = Graph<double>::CreateFromArray((void*)A, sizeof(A)/sizeof(A[0]), A[0][0], HasEdge);

    graph->PrintGraph(0.0);
    */

    delete graph;

    return 0;
}
