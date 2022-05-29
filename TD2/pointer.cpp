#include <iostream>
int main()
{
  int x = 10;
  int *ptr = &x;
  int *ptr1 = ptr;

  std::cout << "x = " << x << std::endl;
  std::cout << "&x = " << &x << std::endl;
  std::cout << "ptr =" << ptr << std::endl;
  std::cout << "*ptr =" << *ptr << std::endl;
  std::cout << "ptr1 =" << ptr1 << std::endl;
  std::cout << "*ptr1 =" << *ptr1 << std::endl;
  std::cout << "&*ptr1 =" << &*ptr1 << std::endl;

  return 0;
}