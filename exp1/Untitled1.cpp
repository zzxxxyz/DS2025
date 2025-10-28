#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// 复数类定义
class Complex {
private:
    double real;  // 实部
    double imag;  // 虚部

public:
    // 构造函数
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // 获取实部
    double getReal() const { return real; }
    
    // 获取虚部
    double getImag() const { return imag; }
    
    // 设置实部
    void setReal(double r) { real = r; }
    
    // 设置虚部
    void setImag(double i) { imag = i; }
    
    // 计算复数的模
    double modulus() const {
        return sqrt(real * real + imag * imag);
    }
    
    // 重载相等运算符
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }
    
    // 重载不等运算符
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
    
    // 重载输出运算符
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
};

// 生成随机复数向量
vector<Complex> generateRandomComplexVector(int size, double minVal, double maxVal) {
    vector<Complex> vec;
    srand(time(0));  // 初始化随机数生成器
    
    for (int i = 0; i < size; ++i) {
        double real = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        double imag = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        vec.push_back(Complex(real, imag));
    }
    
    return vec;
}

// 向量置乱
void shuffleVector(vector<Complex>& vec) {
    srand(time(0));
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(vec[i], vec[j]);
    }
}

// 查找元素（实部和虚部均相同）
int findElement(const vector<Complex>& vec, const Complex& target) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return i;
        }
    }
    return -1;  // 未找到
}

// 插入元素
void insertElement(vector<Complex>& vec, int index, const Complex& elem) {
    if (index >= 0 && index <= vec.size()) {
        vec.insert(vec.begin() + index, elem);
    }
}

// 删除元素
void deleteElement(vector<Complex>& vec, int index) {
    if (index >= 0 && index < vec.size()) {
        vec.erase(vec.begin() + index);
    }
}

// 向量唯一化（去除重复元素）
void uniqueVector(vector<Complex>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = i + 1; j < vec.size(); ++j) {
            if (vec[i] == vec[j]) {
                vec.erase(vec.begin() + j);
                j--;  // 因为删除了元素，需要调整索引
            }
        }
    }
}

// 比较函数：先按模比较，模相同则按实部比较
bool compareComplex(const Complex& a, const Complex& b) {
    if (a.modulus() != b.modulus()) {
        return a.modulus() < b.modulus();
    }
    return a.getReal() < b.getReal();
}

// 起泡排序
void bubbleSort(vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (!compareComplex(vec[j], vec[j + 1])) {
                swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序的合并函数
void merge(vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<Complex> L(n1), R(n2);
    
    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    
    while (i < n1 && j < n2) {
        if (compareComplex(L[i], R[j])) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

// 归并排序
void mergeSort(vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        
        merge(vec, left, mid, right);
    }
}

// 区间查找：查找模介于[m1, m2)的所有元素
vector<Complex> rangeSearch(const vector<Complex>& vec, double m1, double m2) {
    vector<Complex> result;
    for (size_t i = 0; i < vec.size(); ++i) {
        const Complex& c = vec[i];
        double mod = c.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

// 打印向量
void printVector(const vector<Complex>& vec, const string& msg = "") {
    if (!msg.empty()) {
        cout << msg << endl;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    cout << endl << "向量大小: " << vec.size() << endl << endl;
}

int main() {
    // 生成随机复数向量
    int size = 10;
    vector<Complex> complexVec = generateRandomComplexVector(size, -10, 10);
    printVector(complexVec, "初始随机向量:");
    
    // 测试置乱操作
    shuffleVector(complexVec);
    printVector(complexVec, "置乱后的向量:");
    
    // 测试查找操作
    if (!complexVec.empty()) {
        Complex target = complexVec[0];
        int index = findElement(complexVec, target);
        if (index != -1) {
            cout << "查找元素 ";
            cout << target;
            cout << " 成功，位置: " << index << endl << endl;
        } else {
            cout << "查找元素 ";
            cout << target;
            cout << " 失败" << endl << endl;
        }
    }
    
    // 测试插入操作
    Complex newElem(100, 200);
    insertElement(complexVec, 2, newElem);
    cout << "插入元素 ";
    cout << newElem;
    cout << " 后的向量:" << endl;
    printVector(complexVec);
    
    // 测试删除操作
    if (complexVec.size() > 3) {
        Complex delElem = complexVec[3];
        deleteElement(complexVec, 3);
        cout << "删除元素 ";
        cout << delElem;
        cout << " 后的向量:" << endl;
        printVector(complexVec);
    }
    
    // 添加一些重复元素用于测试唯一化
    if (!complexVec.empty()) {
        complexVec.push_back(complexVec[0]);
        complexVec.push_back(complexVec[1]);
        printVector(complexVec, "添加重复元素后的向量:");
        
        // 测试唯一化操作
        uniqueVector(complexVec);
        printVector(complexVec, "唯一化后的向量:");
    }
    
    // 生成用于排序测试的向量
    int sortSize = 1000;
    vector<Complex> sortedVec = generateRandomComplexVector(sortSize, -100, 100);
    
    // 创建三种状态的向量：顺序、乱序、逆序
    vector<Complex> orderedVec = sortedVec;
    sort(orderedVec.begin(), orderedVec.end(), compareComplex);
    
    vector<Complex> shuffledVec = orderedVec;
    shuffleVector(shuffledVec);
    
    vector<Complex> reversedVec = orderedVec;
    reverse(reversedVec.begin(), reversedVec.end());
    
    // 测试起泡排序效率
    clock_t start, end;
    
    // 顺序情况
    vector<Complex> bubbleOrdered = orderedVec;
    start = clock();
    bubbleSort(bubbleOrdered);
    end = clock();
    double bubbleOrderedTime = double(end - start) / CLOCKS_PER_SEC;
    
    // 乱序情况
    vector<Complex> bubbleShuffled = shuffledVec;
    start = clock();
    bubbleSort(bubbleShuffled);
    end = clock();
    double bubbleShuffledTime = double(end - start) / CLOCKS_PER_SEC;
    
    // 逆序情况
    vector<Complex> bubbleReversed = reversedVec;
    start = clock();
    bubbleSort(bubbleReversed);
    end = clock();
    double bubbleReversedTime = double(end - start) / CLOCKS_PER_SEC;
    
    // 测试归并排序效率
    // 顺序情况
    vector<Complex> mergeOrdered = orderedVec;
    start = clock();
    mergeSort(mergeOrdered, 0, mergeOrdered.size() - 1);
    end = clock();
    double mergeOrderedTime = double(end - start) / CLOCKS_PER_SEC;
    
    // 乱序情况
    vector<Complex> mergeShuffled = shuffledVec;
    start = clock();
    mergeSort(mergeShuffled, 0, mergeShuffled.size() - 1);
    end = clock();
    double mergeShuffledTime = double(end - start) / CLOCKS_PER_SEC;
    
    // 逆序情况
    vector<Complex> mergeReversed = reversedVec;
    start = clock();
    mergeSort(mergeReversed, 0, mergeReversed.size() - 1);
    end = clock();
    double mergeReversedTime = double(end - start) / CLOCKS_PER_SEC;
    
    // 输出排序效率比较
    cout << "排序效率比较 (向量大小: " << sortSize << "):" << endl;
    cout << "起泡排序 - 顺序: " << bubbleOrderedTime << " 秒" << endl;
    cout << "起泡排序 - 乱序: " << bubbleShuffledTime << " 秒" << endl;
    cout << "起泡排序 - 逆序: " << bubbleReversedTime << " 秒" << endl << endl;
    
    cout << "归并排序 - 顺序: " << mergeOrderedTime << " 秒" << endl;
    cout << "归并排序 - 乱序: " << mergeShuffledTime << " 秒" << endl;
    cout << "归并排序 - 逆序: " << mergeReversedTime << " 秒" << endl << endl;
    
    // 测试区间查找
    double m1 = 5.0, m2 = 15.0;
    vector<Complex> rangeResult = rangeSearch(orderedVec, m1, m2);
    cout << "模介于 [" << m1 << ", " << m2 << ") 的元素:" << endl;
    printVector(rangeResult);
    
    return 0;
}

