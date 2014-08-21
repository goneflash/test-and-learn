#include <iostream>
#include <vector>
using namespace std;


class BipartiteGraphMatching{
public:
	int left_nodes_num, right_nodes_num;
	vector<vector<int>> left_nodes_links, right_nodes_links;
	vector<int> left_matches, right_matches;
	vector<bool> left_marked, right_marked;
public:
	BipartiteGraphMatching(){
		left_nodes_num = right_nodes_num = 0;
	}
	BipartiteGraphMatching(int left_num, int right_num){
		left_nodes_num = left_num;
		right_nodes_num = right_num;
		left_nodes_links.resize(left_nodes_num);
		right_nodes_links.resize(right_nodes_num);
	}
	~BipartiteGraphMatching(){
		for (unsigned int i = 0; i < right_nodes_links.size(); i++)
			right_nodes_links[i].clear();
		right_nodes_links.clear();
		for (unsigned int i = 0; i < left_nodes_links.size(); i++)
			left_nodes_links[i].clear();
		left_nodes_links.clear();
		left_matches.clear();
		right_matches.clear();
		left_nodes_num = 0;
		right_nodes_num = 0;
	}
	// first node id is 0
	void addLink(int left_id, int right_id){
		if (left_id >= left_nodes_num){
			left_nodes_num = left_id + 1;
			left_nodes_links.resize(left_nodes_num);
		}
		if (right_id >= right_nodes_num){
			right_nodes_num = right_id + 1;
			right_nodes_links.resize(right_nodes_num);
		}
		for (unsigned int i = 0; i < left_nodes_links[left_id].size(); i++)
			if (right_id == left_nodes_links[left_id][i])
				return;
		left_nodes_links[left_id].push_back(right_id);
		right_nodes_links[right_id].push_back(left_id);
	}
	void addLeftNodes(int num){
		left_nodes_num += num;
		left_nodes_links.resize(left_nodes_num);
	}
	void addRightNodes(int num){
		right_nodes_num += num;
		right_nodes_links.resize(right_nodes_num);
	}
	void addLinks(vector<int> left_id, vector<int> right_id){
		for (unsigned int i = 0; i < left_id.size(); i++)
			addLink(left_id[i], right_id[i]);
	}
	bool findMatchFromLeft(int node_id){
		for (unsigned int j = 0; j < left_nodes_links[node_id].size(); j++)
			if (right_marked[left_nodes_links[node_id][j]] == false){
				right_marked[left_nodes_links[node_id][j]] = true;
				if (right_matches[left_nodes_links[node_id][j]] == -1 || findMatchFromLeft(right_matches[left_nodes_links[node_id][j]])){
					right_matches[left_nodes_links[node_id][j]] = node_id;
					left_matches[node_id] = left_nodes_links[node_id][j];
					return true;
				}
			}
			return false;
	}
	void findMaxMatching(){
		left_matches.resize(right_nodes_num);
		right_matches.resize(left_nodes_num);
		left_marked.resize(left_nodes_num);
		right_marked.resize(right_nodes_num);
		for (int i = 0; i < left_nodes_num; i++)
			left_matches[i] = -1;
		for (int i = 0; i < right_nodes_num; i++)
			right_matches[i] = -1;

		for (int i = 0; i < left_nodes_num; i++){
			for (int j = 0; j < right_nodes_num; j++)
				right_marked[j] = false;
			findMatchFromLeft(i);
		}
	}
	int getMaxMatchingNum(){
		int num = 0;
		for (int i = 0; i < left_nodes_links.size(); i++)
			if (right_matches[i] != -1)
				num++;
		return num;
	}
};

int main(){
	BipartiteGraphMatching bigraph;
	int n;
	while (scanf("%d",&n) != EOF){
		BipartiteGraphMatching bigraph(n, n);
		for (int i = 0; i < n; i++){
			int id, num, r_node;
			scanf("%d: (%d)", &id, &num);
			for (int j = 0; j < num; j++){
				scanf(" %d", &r_node);
				bigraph.addLink(id, r_node);
			}
		}
		bigraph.findMaxMatching();
		printf("%d\n", n - (bigraph.getMaxMatchingNum() / 2));
	}

	return 0;
}