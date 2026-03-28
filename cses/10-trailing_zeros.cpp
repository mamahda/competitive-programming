#include <iostream>

int main(){
  int n;
  std::cin >> n;
  int count = 0;

  for (int i = 5; n / i >= 1; i *= 5){
    count += n / i;
    printf("%d\n", count);
  }

  std::cout << count << std::endl;
}