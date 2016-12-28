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
    int A[] = {0,3,7,9,12,34,56,78};
    cout << BinarySearch(A, sizeof(A)/sizeof(decltype(A[0])), 78) << endl;
    return 0;
}
