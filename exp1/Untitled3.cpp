#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// 计算柱状图中最大矩形面积
int largestRectangleArea(vector<int>& heights) {
    stack<int> s;  // 单调栈，存储索引
    int maxArea = 0;
    int n = heights.size();
    
    // 遍历所有柱子
    for (int i = 0; i < n; ++i) {
        // 当栈不为空且当前柱子高度小于栈顶柱子高度时
        while (!s.empty() && heights[i] < heights[s.top()]) {
            int height = heights[s.top()];
            s.pop();
            
            // 计算宽度
            int width = s.empty() ? i : i - s.top() - 1;
            maxArea = max(maxArea, height * width);
        }
        s.push(i);
    }
    
    // 处理栈中剩余的柱子
    while (!s.empty()) {
        int height = heights[s.top()];
        s.pop();
        
        int width = s.empty() ? n : n - s.top() - 1;
        maxArea = max(maxArea, height * width);
    }
    
    return maxArea;
}

// 初始化示例向量（兼容C++98）
vector<int> initExample1() {
    vector<int> v;
    v.push_back(2);
    v.push_back(1);
    v.push_back(5);
    v.push_back(6);
    v.push_back(2);
    v.push_back(3);
    return v;
}

vector<int> initExample2() {
    vector<int> v;
    v.push_back(2);
    v.push_back(4);
    return v;
}

// 生成随机测试数据
vector<int> generateRandomHeights(int size) {
    vector<int> heights;
    for (int i = 0; i < size; ++i) {
        heights.push_back(rand() % 10001);  // 0 <= 高度 <= 104
    }
    return heights;
}

// 打印测试结果
void printTestResult(const vector<int>& heights, int result) {
    cout << "输入: [";
    int n = heights.size();
    for (int i = 0; i < n; ++i) {
        cout << heights[i];
        if (i < n - 1) cout << ", ";
    }
    cout << "]" << endl;
    cout << "最大矩形面积: " << result << endl << endl;
}

int main() {
    // 设置随机数种子
    srand(time(0));
    
    // 示例测试（使用C++98兼容的初始化方式）
    cout << "示例测试:" << endl;
    vector<int> example1 = initExample1();
    int result1 = largestRectangleArea(example1);
    printTestResult(example1, result1);
    
    vector<int> example2 = initExample2();
    int result2 = largestRectangleArea(example2);
    printTestResult(example2, result2);
    
    // 随机生成10组测试数据
    cout << "随机测试数据:" << endl;
    for (int i = 0; i < 10; ++i) {
        // 生成1到10^5之间的随机大小
        int size = rand() % 100000 + 1;
        vector<int> heights = generateRandomHeights(size);
        
        // 对于太大的数组，只打印部分元素
        cout << "测试 " << i + 1 << " (数组大小: " << size << "):" << endl;
        if (size <= 10) {
            printTestResult(heights, largestRectangleArea(heights));
        } else {
            int result = largestRectangleArea(heights);
            cout << "最大矩形面积: " << result << endl << endl;
        }
    }
    
    return 0;
}

