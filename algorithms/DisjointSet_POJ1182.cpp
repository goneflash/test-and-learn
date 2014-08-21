#include <iostream>
#include <vector>

using namespace std;
// relation 0 equal, 1 food, 2 enemy
int parent[50100], relation[50100];
int aa;
int findParent(int x){
	int node = x, id;
	if (parent[node] != node){
		int tmp = parent[node];
		aa++;
		if (aa > 40)
			return parent[node];
		parent[node] = findParent(parent[node]);
		aa--;
		relation[node] = (relation[node] + relation[tmp]) % 3;
	}
	return parent[node];
}

//int find(int x){
//	int node = x, id;
//	while (parent[node] != node)
//		node = parent[node];
//	while (parent[x] != x){
//		id = parent[x];
//		parent[x] = node;
//		x = id;
//	}
//	return node;
//}

void combine(int x, int y, int a, int b, int d){
	parent[b] = a;
	relation[b] = (3 - relation[y] + d - 1 + relation[x]) % 3;
}

int main(){
	int n, k;
	aa = 0;
	//cin >> n >> k;
	scanf("%d%d", &n, &k);
	//parent.resize(n + 1);	
	//relation.resize(n + 1);
	int count = 0;
	//for (vector<int>::iterator it = parent.begin(); it != parent.end(); it++)
	//	*it = count++;
	//for (vector<int>::iterator it = relation.begin(); it != relation.end(); it++)
	//	*it = 0;
	for (int i = 0; i < n + 1; i++){
		parent[i] = i;
		relation[i] = 0;
	}

	int d, x, y, f_word = 0;
	for (int i = 0; i < k; i++){
		//cin >> d >> x >> y;
		scanf("%d%d%d", &d, &x, &y);
		if (x > n || y > n || (d == 2 && x == y)){
			f_word++;
			continue;
		}
		int a = findParent(x);  
		int b = findParent(y);  
		if ( a != b )  
		{  
			///x，y不在同一集合中  
			combine(x, y, a, b, d);  
		}  
		else  
		{  
			switch(d)  
			{  
			case 1:  
				if(relation[x] != relation[y])  
					f_word++;  
				break;  
			case 2:  
				if(((relation[y] + 3 - relation[x]) % 3 ) != 1)  
					f_word++;  
				break;  
			}  
		}

	}
	cout << f_word;

	system("pause");

	return 0;
}