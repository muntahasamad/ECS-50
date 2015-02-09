#include "MyFloat.h"
// Uncomment PRINT_DEBUG to print debug messages
//#define PRINT_DEBUG 1

MyFloat::MyFloat(){
  sign = 0;
  exponent = 0;
  mantissa = 0;
}

MyFloat::MyFloat(float f){
  unpackFloat(f);
}

MyFloat::MyFloat(const MyFloat & rhs){
	sign = rhs.sign;
	exponent = rhs.exponent;
	mantissa = rhs.mantissa;
}

ostream& operator<<(std::ostream &strm, const MyFloat &f){
	strm << f.packFloat();
	return strm;

}

MyFloat MyFloat::operator+(const MyFloat& rhs) const{
	MyFloat result;
	MyFloat lhs = *this;
	MyFloat f1, f2; // assign the float with the bigger exponent to f1 and the smaller exponent to f2
	unsigned int exponent_diff;
#ifdef PRINT_DEBUG
	cout << "------------------------------------------------" << endl;
	cout << "MyFloat operator+" << endl;
	cout << "lhs:" << endl;
	lhs.printFloat();
	cout << "rhs:" << endl;
	rhs.printFloat();
#endif
	if (rhs.exponent >= lhs.exponent){
		f1 = rhs;
		f2 = lhs;
	}
	else {  
		f1 = lhs;
		f2 = rhs;
	}
	exponent_diff = f1.exponent - f2.exponent;
	result.exponent = f1.exponent;
	f1.mantissa = f1.mantissa | (0x1 << 23);	
	f2.mantissa = f2.mantissa | (0x1 << 23);	
#ifdef PRINT_DEBUG
	cout << "Before shift:" << endl;
	cout << "exponent diff: " << exponent_diff << endl;
	cout << "Mantissa1: " << std::hex << f1.mantissa << endl;
	cout << "Mantissa2: " << std::hex << f2.mantissa << endl;
#endif
	if (exponent_diff < 8){
		f1.mantissa = f1.mantissa << exponent_diff;
	}
	else{
		f2.mantissa = f2.mantissa >> exponent_diff;
	}
#ifdef PRINT_DEBUG
	cout << "After shift:" << endl;
	cout << "Mantissa1: " << std::hex << f1.mantissa << endl;
	cout << "Mantissa2: " << std::hex << f2.mantissa << endl;
#endif
	if (f1.sign & f2.sign){
		result.mantissa = f1.mantissa + f2.mantissa;
		result.sign = 1;
	}
	else if (f1.sign){
		if (f1.mantissa > f2.mantissa){
			result.mantissa = f1.mantissa - f2.mantissa;
			result.sign = 1;
		}
		else if (f1.mantissa == f2.mantissa){
			result.mantissa = 0x0;
			result.sign = 0;
		}
		else{
			result.mantissa = f2.mantissa - f1.mantissa;
			result.sign = 0;
		}
	}
	else if (f2.sign){
		if (f2.mantissa > f1.mantissa){
			result.mantissa = f2.mantissa - f1.mantissa;
			result.sign = 1;
		}
		else if (f1.mantissa == f2.mantissa){
			result.mantissa = 0x0;
			result.sign = 0;
		}
		else{
			result.mantissa = f1.mantissa - f2.mantissa;
			result.sign = 0;
		}
	}
	else{
		result.mantissa = f1.mantissa + f2.mantissa;
		result.sign = 0;
	}
		
	if (exponent_diff < 8){
		result.mantissa = result.mantissa >> exponent_diff;
	}
	if (result.mantissa == 0){
		result.exponent =0;
	}
	else if ((0x1 << 24) & result.mantissa){
		result.mantissa = result.mantissa >> 1;
		result.exponent += 1;
	}
	else{
		while (!((0x1 << 23) & result.mantissa)){
			result.mantissa = result.mantissa << 1;
			result.exponent -= 1;
		}
	}
	result.mantissa = result.mantissa & 0xff7fffff;
	
#ifdef PRINT_DEBUG
	cout << "result.mantissa: " << std::hex << result.mantissa << endl;
	cout << "result: " << endl;
	result.printFloat();
	cout << "Leaving MyFloat operator+" << endl;
	cout << "------------------------------------------------" << endl;
#endif
	return result;	
}


MyFloat MyFloat::operator-(const MyFloat& rhs) const{
	MyFloat result;
	MyFloat lhs = *this;
	MyFloat f1, f2; // assign the float with the bigger exponent to f1 and the smaller exponent to f2
	unsigned int exponent_diff;
#ifdef PRINT_DEBUG
	cout << "------------------------------------------------" << endl;
	cout << "MyFloat operator-" << endl;
	cout << "lhs:" << endl;
	lhs.printFloat();
	cout << "rhs:" << endl;
	rhs.printFloat();
#endif
	if (rhs.exponent >= lhs.exponent){
		f1 = rhs;
		f1.sign = rhs.sign? 0 : 1;
		f2 = lhs;
	}
	else {  
		f1 = lhs;
		f2 = rhs;
		f2.sign = rhs.sign? 0 : 1;
	}
	exponent_diff = f1.exponent - f2.exponent;
	result.exponent = f1.exponent;
	f1.mantissa = f1.mantissa | (0x1 << 23);	
	f2.mantissa = f2.mantissa | (0x1 << 23);	
#ifdef PRINT_DEBUG
	cout << "Before shift:" << endl;
	cout << "exponent diff: " << exponent_diff << endl;
	cout << "Mantissa1: " << std::hex << f1.mantissa << endl;
	cout << "Mantissa2: " << std::hex << f2.mantissa << endl;
#endif
	if (exponent_diff <= 8){
		f1.mantissa = f1.mantissa << exponent_diff;
	}
	else{
		f2.mantissa = f2.mantissa >> exponent_diff;
	}
#ifdef PRINT_DEBUG
	cout << "After shift:" << endl;
	cout << "Mantissa1: " << std::hex << f1.mantissa << endl;
	cout << "Mantissa2: " << std::hex << f2.mantissa << endl;
#endif
	if (f1.sign & f2.sign){
		 if (f1.mantissa == f2.mantissa){
			result.mantissa = 0x0;
			result.sign = 0;
		}
		else {
			result.mantissa = f1.mantissa + f2.mantissa;
			result.sign = 1;
		}
	}
	else if (f1.sign){
		if (f1.mantissa > f2.mantissa){
			result.mantissa = f1.mantissa - f2.mantissa;
			result.sign = 1;
		}
		else{
			result.mantissa = f2.mantissa - f1.mantissa;
			result.sign = 0;
		}
	}
	else if (f2.sign){
		if (f2.mantissa > f1.mantissa){
			result.mantissa = f2.mantissa - f1.mantissa;
			result.sign = 1;
		}
		else{
			result.mantissa = f1.mantissa - f2.mantissa;
			result.sign = 0;
		}
	}
	else{
		if (f1.mantissa == f2.mantissa){
			result.mantissa = 0x0;
			result.sign = 0;
		}
		else{
			result.mantissa = f1.mantissa + f2.mantissa;
			result.sign = 0;
		}
	}
		
	if (exponent_diff <= 8){
		result.mantissa = result.mantissa >> exponent_diff;
	}
	if (result.mantissa == 0){
		result.exponent =0;
	}
	else if ((0x1 << 24) & result.mantissa){
		result.mantissa = result.mantissa >> 1;
		result.exponent += 1;
	}
	else{
		while (!((0x1 << 23) & result.mantissa)){
			result.mantissa = result.mantissa << 1;
			result.exponent -= 1;
		}
	}
	result.mantissa = result.mantissa & 0xff7fffff;
	
#ifdef PRINT_DEBUG
	cout << "result.mantissa: " << std::hex << result.mantissa << endl;
	cout << "result: " << endl;
	result.printFloat();
	cout << "Leaving MyFloat operator-" << endl;
	cout << "------------------------------------------------" << endl;
#endif
	return result;	
}

void MyFloat::unpackFloat(float f) {
	__asm__(
	"movl %%edx, %%eax;"
	"andl $0x007fffff, %%eax;"
	"movl %%edx, %%ebx;"
	"andl $0x7f800000, %%ebx;"
	"shrl $23, %%ebx;"
	"movl %%edx, %%ecx;"
	"shrl $31, %%ecx;"
	:"=a"(mantissa), "=b"(exponent), "=c"(sign)
	:"d"(f)
	:"cc"
	);
	
}//unpackFloat

void MyFloat::printFloat() const {
	cout << "MyFloat" << endl;
	cout << "Mantissa: " << std::hex << mantissa << endl;
	cout << "Exponent: " << exponent << endl;
	cout << "Sign: " << sign << endl;
}

float MyFloat::packFloat() const{
	float f;
	__asm__(
	"xorl %%eax, %%eax;"
	"orl %%ebx, %%eax;"
	"shll $23, %%ecx;"
	"orl %%ecx, %%eax;"
	"shll $31, %%edx;"
	"orl %%edx, %%eax;"
	:"=a"(f)
	:"b"(mantissa), "c"(exponent), "d"(sign)
	:"cc"
	);
	return f;
}//packFloat

bool MyFloat::carryWouldHappen(unsigned int a, unsigned int b){
	bool carry;
	__asm__(
	"movl $1, %%ecx;"
	"addl %%eax, %%ebx;"
	"jc carry;"
	"movl $0, %%ecx;"
	"carry:;"
	:"=c"(carry)
	:"a"(a), "b"(b)
	:"cc"
	);
	
	return carry;
}//carryWouldHappen
/*
int main(){
	float a, b;
	MyFloat f1, f2;
	cout << "Please enter LHS: ";
	cin >> a;
	f1 = MyFloat(a);
	cout << "Please enter RHS: ";
	cin >> b;
	f2 = MyFloat(b);
	MyFloat sum;
	sum = f1 + f2;
	cout << "Sum is: " << sum << endl; 
	MyFloat difference;
	difference = f1 - f2;
	cout << "Difference is: " << difference << endl; 
	return 0;
}
*/
