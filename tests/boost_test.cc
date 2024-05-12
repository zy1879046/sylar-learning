/**测试代码**/
#include<iostream>
#include<boost/any.hpp>
#include<map>
#include<string>
using namespace std;
int main(int argc, char const *argv[])
{
    map<string, int> testmap1;
    map<string, map<string, int> > testmap2;
    testmap1["test"] = 2;
    testmap2["test2"] = testmap1;
    boost::any testany = testmap2;
    cout << testmap2["test2"]["test"];
    return 0;
}
