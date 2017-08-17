#include <iostream>
#include <bitset>
int main()
{
    std::bitset<5> bitRegister(1);
    std::cout << bitRegister << std::endl;
	int a=1;
	while(a){
    //get the value that will be put in the highest position after the shift
    bool valToShiftIn = (bitRegister[0] ^ bitRegister[1]);

    bitRegister >>= 1; //go ahead and shift the whole register over to the right

    //mock a 1 being shifted in using bitwise-OR
    // 1. take the result of the XOR
    // 2. shift it up 4 to get it in the proper place
    // 3. bitwise-OR it with the already shifted register to put the value in the proper position
    bitRegister |= ( (valToShiftIn) << 4);
    std::cout << bitRegister << std::endl;
	std::cin>>a;}
	return 0;
}
