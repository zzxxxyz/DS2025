#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <cstring>
#include <cctype>
using namespace std;

// 位图类实现 - 简化版，避免复杂内存操作
class Bitmap {
private:
    string bits; // 直接用字符串存储二进制位，更安全

public:
    Bitmap(int n = 8) { bits.resize(n, '0'); }
    
    ~Bitmap() {}
    
    int size() const { return bits.size(); }
    
    void set(int k) {
        if (k >= 0) {
            if (k >= (int)bits.size()) {
                bits.resize(k + 1, '0');
            }
            bits[k] = '1';
        }
    }
    
    void clear(int k) {
        if (k >= 0 && k < (int)bits.size()) {
            bits[k] = '0';
        }
    }
    
    bool test(int k) const {
        if (k >= 0 && k < (int)bits.size()) {
            return bits[k] == '1';
        }
        return false;
    }
    
    string bits2string(int n = -1) {
        if (n < 0 || n > (int)bits.size()) {
            n = bits.size();
        }
        return bits.substr(0, n);
    }
    
    // 添加赋值运算符，防止浅拷贝问题
    Bitmap& operator=(const Bitmap& other) {
        if (this != &other) {
            bits = other.bits;
        }
        return *this;
    }
};

// Huffman编码类型定义
typedef Bitmap HuffCode;

// 二叉树节点结构
struct BinTreeNode {
    char ch;           // 字符
    int weight;        // 权重（频率）
    BinTreeNode* left; // 左孩子
    BinTreeNode* right;// 右孩子
    
    BinTreeNode(char c = '\0', int w = 0) 
        : ch(c), weight(w), left(NULL), right(NULL) {}
};

// 比较函数，用于优先队列
struct CompareNode {
    bool operator()(BinTreeNode* a, BinTreeNode* b) {
        return a->weight > b->weight; // 小顶堆
    }
};

// Huffman树类
class HuffTree {
private:
    BinTreeNode* root;
    map<char, string> codeMap; // 直接用字符串存储编码，更简单
    
    // 递归生成编码表
    void generateCode(BinTreeNode* node, string code) {
        if (!node) return;
        
        // 叶子节点，保存编码
        if (!node->left && !node->right) {
            codeMap[node->ch] = code;
            return;
        }
        
        // 左子树：编码加0
        generateCode(node->left, code + "0");
        
        // 右子树：编码加1
        generateCode(node->right, code + "1");
    }
    
    // 释放树内存
    void destroyTree(BinTreeNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    
public:
    // 构造函数：从字符频率表构建Huffman树
    HuffTree(const map<char, int>& freq) {
        // 创建优先队列（小顶堆）
        priority_queue<BinTreeNode*, vector<BinTreeNode*>, CompareNode> pq;
        
        // 将所有字符加入队列（只加入频率>0的）
        for (map<char, int>::const_iterator it = freq.begin(); it != freq.end(); ++it) {
            if (it->second > 0) {
                pq.push(new BinTreeNode(it->first, it->second));
            }
        }
        
        // 处理空队列或只有一个节点的情况
        if (pq.empty()) {
            root = NULL;
            return;
        }
        
        if (pq.size() == 1) {
            BinTreeNode* single = pq.top();
            root = new BinTreeNode('\0', single->weight);
            root->left = single;
        } else {
            // 构建Huffman树
            while (pq.size() > 1) {
                BinTreeNode* left = pq.top(); pq.pop();
                BinTreeNode* right = pq.top(); pq.pop();
                
                // 创建新节点
                BinTreeNode* parent = new BinTreeNode('\0', left->weight + right->weight);
                parent->left = left;
                parent->right = right;
                
                pq.push(parent);
            }
            
            root = pq.top(); pq.pop();
        }
        
        // 生成编码表
        generateCode(root, "");
    }
    
    ~HuffTree() {
        destroyTree(root);
    }
    
    // 获取字符的Huffman编码
    string getCode(char c) {
        map<char, string>::iterator it = codeMap.find(c);
        if (it != codeMap.end()) {
            return it->second;
        }
        return "";
    }
    
    // 编码字符串
    string encode(const string& str) {
        string result;
        for (size_t i = 0; i < str.size(); ++i) {
            char c = str[i];
            if (isupper(c)) c = tolower(c); // 转为小写
            if (islower(c)) { // 只编码字母
                result += getCode(c);
            }
        }
        return result;
    }
    
    // 打印编码表
    void printCodeTable() {
        cout << "Huffman编码表：" << endl;
        for (char c = 'a'; c <= 'z'; c++) {
            map<char, string>::iterator it = codeMap.find(c);
            if (it != codeMap.end()) {
                cout << c << ": " << it->second << endl;
            }
        }
    }
};

// 从文本统计字符频率
map<char, int> countFrequency(const string& text) {
    map<char, int> freq;
    
    // 初始化所有字母的频率为0
    for (char c = 'a'; c <= 'z'; c++) {
        freq[c] = 0;
    }
    
    // 统计文本中的字母频率
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (isupper(c)) {
            freq[tolower(c)]++;
        } else if (islower(c)) {
            freq[c]++;
        }
    }
    
    return freq;
}

int main() {
    // 示例：使用《I have a dream》演讲稿文本
    string speech = 
        "I have a dream that one day this nation will rise up and live out the true meaning of its creed: "
        "\"We hold these truths to be self-evident, that all men are created equal.\" "
        "I have a dream that one day on the red hills of Georgia the sons of former slaves and the sons of former slave owners will be able to sit down together at the table of brotherhood. "
        "I have a dream today!";
    
    // 统计字符频率
    map<char, int> freq = countFrequency(speech);
    
    // 创建Huffman树
    HuffTree huffTree(freq);
    
    // 打印编码表
    huffTree.printCodeTable();
    
    // 测试编码
    cout << "\n编码测试：" << endl;
    string testWords[] = {"dream", "nation", "freedom", "justice", "equal"};
    int numWords = sizeof(testWords) / sizeof(testWords[0]);
    
    for (int i = 0; i < numWords; ++i) {
        string code = huffTree.encode(testWords[i]);
        cout << testWords[i] << ": " << code << endl;
    }
    
    cout << "\n程序正常结束！" << endl;
    return 0;
}
