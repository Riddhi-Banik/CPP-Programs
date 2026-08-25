#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

vector<int> factor(int num) {
    vector<int> ans;
    int i = 2;
    do {
        if (num % i){
            i += 1;
        } else {
            ans.push_back(i);
            num /= i ;
        };
    } while (num > 1);

    return ans;
};

int main() {
    
    int target_num;

    cout << "Insert: ";cin >> target_num;cout << "\n";

    vector<int> answer = factor(target_num);
    cout << "[";
    for (int i : answer){
        cout << i << " ";
    } cout << "]\n";
    
    string _;
    cout << "Press Enter to Exit..."; cin.ignore();cin.get();
    return 0;
};

/* pseudo-code
 
def factor(num:int) -> list:
    ans = list()
    i = 2
    while True:
        if num == 1:
            break
        if (num % i):
            i += 1
        else:
            ans.append(i)
            num = num // i
    return ans
        
if __name__ == "__main__":
    target_num: int = int(input('Give a number: '))
    print(factor(target_num))

*/

