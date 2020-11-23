// Homework1_VRU
// Convert this program to C++

#include <iostream>
#include <vector>
using namespace std;



const int array_size = 40;

// Function: sum contents of int vector
inline void sum(int &p, const vector<int>& d)
{
  p = 0;
  for(int i = 0; i < d.size(); i++)
  {
    p = p + d[i];
  }
}


// Function: main
int main()
{
  int i;
  int accum = 0;
  vector<int> data;
  
  for(i = 0; i < array_size; ++i)
  {
    data.push_back(i);
  }
  sum(accum, data);
  cout << "sum is " << accum << endl;
  return 0;

}
