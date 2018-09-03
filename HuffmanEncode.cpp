#include<iostream>
#include<vector>
#include<string>

using namespace std;
//1 定义常量，定义一个无穷量，定义编码位最多有(2^5)个，霍夫曼节点和编码字最多有(2^10)个
const int INF = 1000000000;
const int maxBit = 1 << 5;
const int maxNode = 1 << 10;
const int maxCode = 1 << 10;


struct Node {
	string value;
	float weight;
	int parent;
	int lchild;
	int rchild;
};
struct Code {
	int bit[maxBit];
	int start;
	string value;
};

int n;
Node huffmanNode[maxNode];
Code huffmanCode[maxCode];



//初始化霍夫曼树节点（2*n-1）
void initialHuffmanTree(){
	for (int i = 0; i<2 * n - 1; i++) {
		huffmanNode[i].value = " ";
		huffmanNode[i].weight = 0;
		huffmanNode[i].parent = -1;
		huffmanNode[i].lchild = -1;
		huffmanNode[i].rchild = -1;
	}
}


void CreatHuffmanTree() {
	for (int i = 0; i<n - 1; i++) {    //构造原始叶节点（待编码字符）后面的n-1个合成父节点
		int w1 = INF;    //每一次初始化最小权值
		int w2 = INF;    //每一次初始化次小权值
		int index1 = 0;
		int index2 = 0;

		for (int j = 0; j<n + i; j++) {
			if (huffmanNode[j].weight<w1&&huffmanNode[j].parent == -1) {
				w2 = w1;    //找到新的最小值后，将之前的最小值w1保存变为次小值w2
				index2 =index1;
				w1 = huffmanNode[j].weight;   //新的最小值
				index1 = j;
			}
			else if (huffmanNode[j].weight<w2&&huffmanNode[j].parent == -1) {
				w2 = huffmanNode[j].weight;
				index2 = j;
			}
		}
		huffmanNode[index1].parent = n + i;
		huffmanNode[index2].parent = n + i;
		huffmanNode[n + i].weight = w1 + w2;
		huffmanNode[n + i].lchild = index1;   //注意：在定义时parent和lchild、rchild都是int
		huffmanNode[n + i].rchild = index2;   //型数据，并不是指针，这是为了节省空间并且操作简单

	}
}

//编码
/*Huffman编码方式为每个字符编码是在节点数组huffmanNode中从该字符节点开始，向上追溯其父节点，
若该节点是其父节点的左子节点，则其霍夫曼编码的[n-1]位是0，若是其父节点的右子节点，则其霍夫
曼编码的【n-1】位是1；再对父节点找父节点，一直向上追溯，将start--，将其每一位编码的bit值弄
清楚，直到其父节点不存在（parent=-1即到达根节点）。因为对每一个字符其start都设为n-1（因为n个
字符的霍夫曼编码最长为n-1），其每一位bit值是从后往前追溯，此时的start值表明输出的霍夫曼编码应
从第start位开始输出。因为前面的值没有确定了。只有从start当前值到n-1才是真正的编码值。*/
void HuffmanEncoding() {
	int  child, parent;
	Code cd;
	for (int i = 0; i<n; i++) {
		cd.value = huffmanNode[i].value;
		cd.start = n - 1;
		child = i;
		parent = huffmanNode[child].parent;
		while (parent != -1) {
			if (huffmanNode[parent].lchild == child) {
				cd.bit[cd.start] = 0;
			}
			else if (huffmanNode[parent].rchild == child) {
				cd.bit[cd.start] = 1;
			}
			cd.start--;
			child = parent;
			parent = huffmanNode[child].parent;
		}

		//保存每一个字符的霍夫曼编码
		for (int j = cd.start + 1; j<n; j++)
			huffmanCode[i].bit[j] = cd.bit[j];
		huffmanCode[i].value = cd.value;  //保存霍夫曼编码的原始字符
		huffmanCode[i].start = cd.start;   //之后输出编码值时要从start+1位开始输起
	}
}
//打印霍夫曼编码
void PrintHuffman() {
	for (int i = 0; i < n; i++) {
		cout << "第" << i + 1 << "个字符" << huffmanNode[i].value << "的Huffman编码是:";
		for (int j = huffmanCode[i].start + 1; j < n; j++) {
			cout << huffmanCode[i].bit[j];
		}
		cout << "编码起始值是：" << huffmanCode[i].start << endl;
	}
}


//解码
/*对0/1序列s的bit位，从s[0]-s.length()从霍夫曼树的根节点开始往下遍历直到找到叶子节点，
将其叶子节点的字符（原码）存入容器，找到一个原码后接着再对当前的s[i]从根节点向下遍历，
每到达一个叶子节点就将字符存入容器，直到遍历到序列末端，输出容器中的值即是对应的原码。
或是到达序列末端却发现缺失，则中断解码。*/
void HuffmanDecoding(string s) {
	vector<string> v;  //存放解码出来的一个个原码
	int ok = 1;   //标志位，为1表示正常解码，为0表示解码有误
	for (int i = 0; i<s.length();) {
		int x = 2 * n - 1 - 1;         //从根节点开始遍历，每遍历到一个叶子节点，将叶子节
									   //的字符存入vector中。
		while (huffmanNode[x].lchild != -1 && huffmanNode[x].rchild != -1) {
			if (s[i] == '0')
				x = huffmanNode[x].lchild;
			else if(s[i] =='1')
				x = huffmanNode[x].rchild;
			i++;
			//未找到叶子节点
			if (i == s.length() && huffmanNode[x].lchild != -1) {
				ok = 0;
				break;  //退出循环
			}
		} //到达叶子节点或序列末端，while结束
		  //找到叶子节点，将叶子节点中的原始字符存入容器中
		if (ok) {
			v.push_back(huffmanNode[x].value);
		}
	}   //将解码出的字符都存入vector中，for循环结束

		//将解码字符串输出
	if (ok) {
		for (int i = 0; i<v.size(); i++)
			cout <<v[i];
		cout << endl;
	}
	else
		cout << "解码失败！"<< endl;
}

//主函数

int main() {
	//编码
	while (true) {
		cout << "请输入字符串个数：";
		cin >> n;
		if (!n) {
			break;
		}

		// 初始化Huffman树
		initialHuffmanTree();

		for (int i = 0; i<n; i++) {
			cout << "请输入第" << i + 1 << "个字符串和其权值";
			cin >> huffmanNode[i].value >> huffmanNode[i].weight;
			//将其存入Huffman树的节点中
		}
		CreatHuffmanTree();

		HuffmanEncoding();

		PrintHuffman();

		//解码
		while (true) {
			cout << "请输入符合上述编码规则的0 / 1序列或按q进行下一次编码：";
			string s;
			cin >> s;
			if (s[0] == 'q') {    //考虑未输入序列的情况
				cout << endl;
				break;
			}

			cout << "输入的序列是:" << s << endl;
			cout << "解码的结果是：";
			HuffmanDecoding(s);
		}
	}
	return 0;
}