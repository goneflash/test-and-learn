#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class SegmentTree{
public:
	vector<int> left_v, right_v; // segment range
	vector<int> count, measure, line; // properties
	vector<int> lchild, rchild; // index of children
	vector<int> lbd, rbd; // if left point and right point are occupied
	int node_num; // the number of the nodes in the tree

public:
	SegmentTree(){
		node_num = 0;
	};
	SegmentTree(int start, int end){
		node_num = 0;
		buildTree(start, end);
	}

	void buildTree(int start, int end){
		int id = node_num;
		node_num++;
		left_v.push_back(start);
		right_v.push_back(end);
		count.push_back(0);
		line.push_back(0);
		measure.push_back(0);
		lbd.push_back(0);
		rbd.push_back(0);
		if (end - start > 1){
			lchild.push_back(node_num);rchild.push_back(node_num);
			buildTree(start, (start + end) / 2);
			rchild[id] = node_num;
			buildTree((start + end) / 2, end);
		} 
		else{
			lchild.push_back(-1);
			rchild.push_back(-1);
		}
	}
	void insertTree(int start, int end, int node_id){
		//cout << left_v[node_id] << " " << right_v[node_id] << " " << start << " " << end << endl;
		// update lbd and rbd
		if (start <= left_v[node_id])
			lbd[node_id] = 1;
		if (end >= right_v[node_id])
			rbd[node_id] = 1;
		// insert segment
		int mid = (left_v[node_id] + right_v[node_id]) / 2;
		if (start <= left_v[node_id] && end >= right_v[node_id]){
			count[node_id]++;
		}
		else{
			if (start < mid)
				insertTree(start, end, lchild[node_id]);
			if (end > mid)
				insertTree(start, end, rchild[node_id]);
		}
		// update measure and number of continuous segments
		if (count[node_id] > 0){
			measure[node_id] = right_v[node_id] - left_v[node_id];
			line[node_id] = 1;
		}
		else
			if (lchild[node_id] == -1){
				measure[node_id] = 0;
				line[node_id] = 0;
			}
			else{
				measure[node_id] = measure[lchild[node_id]] + measure[rchild[node_id]];
				if (rbd[lchild[node_id]] == 1 && lbd[rchild[node_id]] == 1)
					line[node_id] = line[lchild[node_id]] + line[rchild[node_id]] - 1;
				else
					line[node_id] = line[lchild[node_id]] + line[rchild[node_id]];
			}
	}
	void deleteTree(int start, int end, int node_id){
		int mid = (left_v[node_id] + right_v[node_id]) / 2;
		if (start <= left_v[node_id] && end >= right_v[node_id]){
			count[node_id]--;
		}
		else{
			if (start < mid)
				deleteTree(start, end, lchild[node_id]);
			if (end > mid)
				deleteTree(start, end, rchild[node_id]);
		}
		// update lbd and rbd
		if (count[node_id] == 0)
			if (lchild[node_id] == -1){
				lbd[node_id] = 0;
				rbd[node_id] = 0;
			}
			else{
				lbd[node_id] = lbd[lchild[node_id]];
				rbd[node_id] = rbd[rchild[node_id]];
			}
			// update measure and number of continuous segments
			if (count[node_id] > 0){
				measure[node_id] = right_v[node_id] - left_v[node_id];
				line[node_id] = 1;
			}
			else
				if (lchild[node_id] == -1){
					measure[node_id] = 0;
					line[node_id] = 0;
				}
				else{
					measure[node_id] = measure[lchild[node_id]] + measure[rchild[node_id]];
					if (rbd[lchild[node_id]] == 1 && lbd[rchild[node_id]] == 1)
						line[node_id] = line[lchild[node_id]] + line[rchild[node_id]] - 1;
					else
						line[node_id] = line[lchild[node_id]] + line[rchild[node_id]];
				}
	}
};

struct Segment{
	int start, end;
	int pos;
	bool in; // in : true out : false
};
struct Rect{
	int llx, lly, rux, ruy;
};
void addSegment(Rect rect, vector<Segment> & ver, vector<Segment> & hori){
	Segment seg;
	// right
	seg.start = rect.lly;
	seg.end = rect.ruy;
	seg.pos = rect.rux;
	seg.in = false;
	ver.push_back(seg);
	// left
	seg.start = rect.lly;
	seg.end = rect.ruy;
	seg.pos = rect.llx;
	seg.in = true;
	ver.push_back(seg);
	// up
	seg.start = rect.llx;
	seg.end = rect.rux;
	seg.pos = rect.ruy;
	seg.in = false;
	hori.push_back(seg);
	// down
	seg.start = rect.llx;
	seg.end = rect.rux;
	seg.pos = rect.lly;
	seg.in = true;
	hori.push_back(seg);
}
bool compare(Segment a, Segment b){
	return a.pos < b.pos; //升序排列，如果改为return a>b，则为降序
}
int main(){
	int n;
	cin >> n;
	SegmentTree tree(-10000, 10000);
	Segment seg;
	Rect rect;
	vector<Segment> ver, hori;
	for (int i = 0; i < n; i++){
		cin >> rect.llx >> rect.lly >> rect.rux >> rect.ruy;
		addSegment(rect, ver, hori);
	}
	// sort segment
	sort(ver.begin(), ver.end(), compare);
	sort(hori.begin(), hori.end(), compare);
	// calculate horizontal length
	int sum_length = 0;
	int cur_pos = ver[0].pos;
	vector<Segment>::iterator it = ver.begin(); 
	while (it != ver.end()){
		while (it != ver.end() && it->pos == cur_pos){
			if (it->in)
				tree.insertTree(it->start, it->end, 0);
			else
				tree.deleteTree(it->start, it->end, 0);
			it++;
		}
		if (it == ver.end())
			break;
		sum_length += tree.line[0] * (it->pos - cur_pos);
		cur_pos = it->pos;
	}
	// calculate vertical length
	cur_pos = hori[0].pos;
	it = hori.begin(); 
	while (it != hori.end()){
		while (it != hori.end() && it->pos == cur_pos){
			if (it->in)
				tree.insertTree(it->start, it->end, 0);
			else
				tree.deleteTree(it->start, it->end, 0);
			it++;
		}
		if (it == hori.end())
			break;
		sum_length += tree.line[0] * (it->pos - cur_pos);
		cur_pos = it->pos;
	}
	cout << sum_length * 2;
	system("pause");
	return 0;
}
