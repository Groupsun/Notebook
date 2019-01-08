```C++
#include <iostream>
#include <vector>
using namespace std;
void Showthenumber(vector <int>:: const_iterator begin , vector< int>::const_iterator end)
{
                 while (begin != end)
                {
                                cout << * begin << " " ;
                                ++ begin;
                }
}
int main()
{
                 vector<int > random;
                 int temp;
                 while (cin >> temp)
                {
                                random.push_back(temp);
                }
                 vector<int >::const_iterator begin = random.begin();
                 vector<int >::const_iterator end = random.end();
                Showthenumber(begin, end);
                 return 0;
}
```
当要向函数体中传递vector时，最好使用迭代器。