#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <string>
using namespace std;

#define random(x) (rand() % x)

void gcd(int *a, int *b){
	int newa = *a, newb = *b;
	if (newa < newb){
		int tmp = newa;
		newa = newb;
		newb = tmp;
	}
	while (newb > 0){
		int tmp = newa % newb;
		newa = newb;
		newb = tmp;
	}
	*a /= newa;
	*b /= newa;
}

string getDecimal(int a, int b){
	stringstream sstr;
	string ans;

	gcd(&a, &b);
	cout << a << " " << b << endl;

	int integral = a / b;
	a %= b;
	if (a == 0){
		sstr << integral;
		sstr >> ans;
		return ans;
	}
	vector<int> residue(b, -1);
	string decimal;
	int start_loop = -1, deci_idx = 0;
	while (a != 0){
		if (residue[a] != -1){
			start_loop = residue[a];
			break;
		}
		decimal += '0' + a * 10 / b;
		residue[a] = deci_idx;
		deci_idx++;
		a = a * 10 % b;
	}
	sstr << integral << '.';
	if (start_loop == -1){
		sstr << decimal;
		sstr >> ans;
	}
	else{
		sstr >> ans;
		for (unsigned int i = 0; i < decimal.size(); i++){
			if (start_loop == i)
				ans += '(';
			ans += decimal[i];
		}
		ans += ')';
	}

	return ans;
}


int main(){
	srand(unsigned(time(NULL)));
	
	int a = random(100), b = random(100);
	//int a = 48, b = 36;

	cout << a << " " << b << endl;
	cout << getDecimal(a, b);

	return 0;
}