#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define DEBUG

// match s0 to s
// if matched return first matched position
// if not return 0
int KMPMatch(string s, string s0){
	vector<int> pre(s.size());
#ifdef DEBUG 
	cout << "String Length:" << s.size() << endl;
#endif
	// calculate pre vector
	pre[0] = -1;
#ifdef DEBUG
	cout << pre[0] << " ";
#endif
	int k = -1;
	for (unsigned int i = 1; i < s.size(); i++){
		//cout << i << endl;
		while ((k > -1) && (s[i] != s[k + 1])) k = pre[k];
		if (s[i] == s[k + 1]) k++;
		pre[i] = k;
#ifdef DEBUG
		cout << pre[i] << " ";
#endif
	}
#ifdef DEBUG
	cout << endl;
#endif

	// match
	k = -1;
	for (unsigned int i = 0; i < s.size(); i++){
		while (k > -1 && s[i] != s0[k + 1]) k = pre[k];
		if (s[i] == s0[k + 1]) k++;
		if (k == s0.size() - 1)
			return (i - k);
	}
	return 0;
}


int main(){
	string s = "ababcababaab";
	string s0 = "ababa";

	cout << KMPMatch(s, s0) << endl;

	return 0;
}