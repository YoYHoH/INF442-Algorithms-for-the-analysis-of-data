#include <iostream>

int main()
{
  int n;
  std::cout << "Enter a number: ";
  std::cin >> n;
  std::cout << "I am going to build an array of "
	    << n
	    << " elements"
	    << std::endl;
  
  int array[n];
  
  for (int i = 0; i < 2*n; i++) {
    array[i] = i*i;
    std::cout << "Wrote "
	      << array[i]
	      << " into cell "
	      << i
	      << " of the array"
	      << std::endl;
  }
  
  return 0;
}