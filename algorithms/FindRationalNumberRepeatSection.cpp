#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

//#define DEBUG
#define MAX_INT 1000
#define NUM_TRIAL 10


string findRepeatSection(int numerator, int denominator){
	string non_repeat_part;
	string repeat_part;
	string int_part;

#ifdef DEBUG
	cout << "numerator: " << numerator << " denominator: " << denominator << endl;
#endif

	int reminder, result;
	stringstream sstr;
	sstr << numerator / denominator;
	sstr >> int_part;
#ifdef DEBUG
	cout << "Interger part is " << int_part << endl;
#endif
	numerator %= denominator;

	vector<int> reminder_vec(MAX_INT);
	fill(reminder_vec.begin(), reminder_vec.end(), 0);
	int pos = 0;
	while (true){
		pos++;
		if (numerator == 0){
#ifdef DEBUG
			cout << "Not infinite" << endl;
#endif
			break;
		}
		if (reminder_vec[numerator] != 0){
#ifdef DEBUG
			cout << "Repeat Section End" << endl;
#endif
			break;
		}

		reminder_vec[numerator] = pos;
		numerator *= 10;

		sstr.clear();
		sstr << (numerator / denominator);
		string tmp;
		sstr >> tmp;

		non_repeat_part += tmp;
#ifdef DEBUG
		cout << "non_repeat_part is " << non_repeat_part << endl;
#endif

		numerator %= denominator;
	}

	string ans;
	if (numerator != 0){
		repeat_part.assign(non_repeat_part, reminder_vec[numerator] - 1, non_repeat_part.size() - reminder_vec[numerator] + 1);
		if (reminder_vec[numerator] > 1)
			non_repeat_part.assign(non_repeat_part, 0, reminder_vec[numerator] - 1);
		else
			non_repeat_part = "";
	}

	if (repeat_part.size() == 0)	
		repeat_part = "0";
	ans = int_part + "." + non_repeat_part + "(" + repeat_part + ")";

	return ans;
}



int main(){
	// Set seed
	srand((unsigned)time(NULL));

	stringstream sstr;
	sstr << 12;
	string s;
	sstr >> s;
	cout << s << endl;
	sstr.clear();
	sstr << 34;
	sstr >> s;
	cout << s << endl;


	cout << findRepeatSection(1, 3) << endl;
	cout << findRepeatSection(2, 4) << endl;
	cout << findRepeatSection(22, 7) << endl;
	cout << findRepeatSection(16, 720) << endl;

	for (int i = 0; i < NUM_TRIAL; i++){
		int numerator = rand() % MAX_INT, denominator = rand() % MAX_INT;
		cout << "numerator: " << numerator << " denominator: " << denominator << endl;
		cout << findRepeatSection(numerator, denominator) << endl;
	}

	return 0;
}