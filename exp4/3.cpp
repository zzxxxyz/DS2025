#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>  
#include <map>
#include <string>
#include <cstdlib>
#include <ctime>
#include <utility>

// 边界框类：添加默认构造函数，兼容C++98
struct BBox {
    float x1, y1, x2, y2;
    float score;

    // 默认构造函数（解决vector初始化问题）
    BBox() : x1(0.0f), y1(0.0f), x2(0.0f), y2(0.0f), score(0.0f) {}
    // 带参数构造函数
    BBox(float x1_, float y1_, float x2_, float y2_, float score_) 
        : x1(x1_), y1(y1_), x2(x2_), y2(y2_), score(score_) {}
};

// ====================== 排序算法实现 ======================
// 1. 冒泡排序（按置信度升序）
void bubbleSort(std::vector<BBox>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j].score > arr[j + 1].score) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// 2. 快速排序（按置信度升序）
int partition(std::vector<BBox>& arr, int low, int high) {
    float pivot = arr[high].score;
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (arr[j].score <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortHelper(std::vector<BBox>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortHelper(arr, low, pi - 1);
        quickSortHelper(arr, pi + 1, high);
    }
}

void quickSort(std::vector<BBox>& arr) {
    if (arr.empty()) return;
    quickSortHelper(arr, 0, arr.size() - 1);
}

// 3. 归并排序（按置信度升序）
void merge(std::vector<BBox>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // C++98兼容：嵌套模板添加空格
    std::vector<BBox> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].score <= R[j].score) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i; ++k;
    }
    while (j < n2) {
        arr[k] = R[j];
        ++j; ++k;
    }
}

void mergeSortHelper(std::vector<BBox>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortHelper(arr, left, mid);
        mergeSortHelper(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void mergeSort(std::vector<BBox>& arr) {
    if (arr.empty()) return;
    mergeSortHelper(arr, 0, arr.size() - 1);
}

// 4. 堆排序（按置信度升序）
void heapify(std::vector<BBox>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l].score > arr[largest].score) largest = l;
    if (r < n && arr[r].score > arr[largest].score) largest = r;

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<BBox>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; --i) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ====================== 数据生成函数 ======================
// 生成随机数（0到1之间）
float random01() {
    return static_cast<float>(rand()) / RAND_MAX;
}

// 生成指定范围的随机数
float randomFloat(float min, float max) {
    return min + random01() * (max - min);
}

// 生成边界框数据
std::vector<BBox> generateBBoxes(int num, const std::string& distType) {
    std::vector<BBox> bboxes;
    bboxes.reserve(num);

    if (distType == "random") {
        for (int i = 0; i < num; ++i) {
            float x1 = randomFloat(0.0f, 800.0f);
            float y1 = randomFloat(0.0f, 600.0f);
            float x2 = x1 + randomFloat(20.0f, 100.0f);
            float y2 = y1 + randomFloat(20.0f, 100.0f);
            float score = random01();
            bboxes.push_back(BBox(x1, y1, x2, y2, score));
        }
    } else if (distType == "cluster") {
        // C++98兼容：手动构造中心簇，嵌套模板加空格
        std::vector<std::pair<float, float> > centers;
        centers.push_back(std::make_pair(100.0f, 100.0f));
        centers.push_back(std::make_pair(300.0f, 200.0f));
        centers.push_back(std::make_pair(500.0f, 400.0f));
        centers.push_back(std::make_pair(700.0f, 300.0f));
        centers.push_back(std::make_pair(200.0f, 500.0f));

        for (int i = 0; i < num; ++i) {
            std::pair<float, float>& center = centers[rand() % centers.size()];
            float cx = center.first;
            float cy = center.second;
            float x1 = cx + randomFloat(-50.0f, 50.0f);
            float y1 = cy + randomFloat(-50.0f, 50.0f);
            float x2 = x1 + randomFloat(20.0f, 100.0f);
            float y2 = y1 + randomFloat(20.0f, 100.0f);
            float score = random01();
            bboxes.push_back(BBox(x1, y1, x2, y2, score));
        }
    }

    return bboxes;
}

// ====================== NMS算法实现 ======================
// 计算IoU（交并比）
float calculateIoU(const BBox& box1, const BBox& box2) {
    float x1 = std::max(box1.x1, box2.x1);
    float y1 = std::max(box1.y1, box2.y1);
    float x2 = std::min(box1.x2, box2.x2);
    float y2 = std::min(box1.y2, box2.y2);

    float interArea = std::max(0.0f, x2 - x1) * std::max(0.0f, y2 - y1);
    if (interArea == 0) return 0.0f;

    float box1Area = (box1.x2 - box1.x1) * (box1.y2 - box1.y1);
    float box2Area = (box2.x2 - box2.x1) * (box2.y2 - box2.y1);
    float unionArea = box1Area + box2Area - interArea;

    return interArea / unionArea;
}

// NMS算法
std::vector<BBox> nms(std::vector<BBox> bboxes, float iouThresh, void (*sortFunc)(std::vector<BBox>&)) {
    if (bboxes.empty()) return std::vector<BBox>();

    sortFunc(bboxes);
    std::reverse(bboxes.begin(), bboxes.end());

    std::vector<BBox> keep;
    while (!bboxes.empty()) {
        BBox topBox = bboxes.front();
        keep.push_back(topBox);
        bboxes.erase(bboxes.begin());

        for (std::vector<BBox>::iterator it = bboxes.begin(); it != bboxes.end();) {
            if (calculateIoU(topBox, *it) >= iouThresh) {
                it = bboxes.erase(it);
            } else {
                ++it;
            }
        }
    }

    return keep;
}

// ====================== 性能测试 ======================
struct SortAlg {
    std::string name;
    void (*func)(std::vector<BBox>&);
};

// 构造排序算法列表（C++98兼容）
std::vector<SortAlg> createSortAlgorithms() {
    std::vector<SortAlg> algos;
    SortAlg algo1 = {"Bubble Sort", bubbleSort};
    algos.push_back(algo1);
    SortAlg algo2 = {"Quick Sort", quickSort};
    algos.push_back(algo2);
    SortAlg algo3 = {"Merge Sort", mergeSort};
    algos.push_back(algo3);
    SortAlg algo4 = {"Heap Sort", heapSort};
    algos.push_back(algo4);
    return algos;
}

// 测试性能（C++98兼容：嵌套模板加空格）
std::map<std::string, std::map<std::string, std::vector<double> > > testPerformance(
    const std::vector<int>& numList,
    const std::vector<std::string>& distTypes) {

    std::map<std::string, std::map<std::string, std::vector<double> > > results;
    std::vector<SortAlg> sortAlgorithms = createSortAlgorithms();

    // 初始化结果
    for (size_t a = 0; a < sortAlgorithms.size(); ++a) {
        const std::string& algName = sortAlgorithms[a].name;
        for (size_t d = 0; d < distTypes.size(); ++d) {
            const std::string& dist = distTypes[d];
            results[algName][dist] = std::vector<double>();
        }
    }

    // 遍历数据规模
    for (size_t n = 0; n < numList.size(); ++n) {
        int num = numList[n];
        std::cout << "\n=== 测试数据规模：" << num << " ===" << std::endl;

        // 遍历数据分布
        for (size_t d = 0; d < distTypes.size(); ++d) {
            const std::string& dist = distTypes[d];
            std::vector<BBox> bboxes = generateBBoxes(num, dist);

            // 遍历排序算法
            for (size_t a = 0; a < sortAlgorithms.size(); ++a) {
                const SortAlg& alg = sortAlgorithms[a];
                const std::string& algName = alg.name;
                void (*sortFunc)(std::vector<BBox>&) = alg.func;

                const int testTimes = 3;
                double totalTime = 0.0;

                for (int t = 0; t < testTimes; ++t) {
                    clock_t start = clock();
                    nms(bboxes, 0.5f, sortFunc);
                    clock_t end = clock();
                    double timeUsed = static_cast<double>(end - start) / CLOCKS_PER_SEC;
                    totalTime += timeUsed;
                }

                double avgTime = totalTime / testTimes;
                results[algName][dist].push_back(avgTime);

                std::cout << std::fixed << std::setprecision(6)
                          << "  " << std::setw(12) << algName
                          << " | " << std::setw(8) << dist
                          << " | 耗时：" << avgTime << "s" << std::endl;
            }
        }
    }

    return results;
}

// ====================== 主函数 ======================
int main() {
    // 设置随机数种子（C++98兼容）
    srand(static_cast<unsigned int>(time(NULL)));

    // 构造测试配置（C++98兼容）
    std::vector<int> numList;
    numList.push_back(100);
    numList.push_back(500);
    numList.push_back(1000);
    numList.push_back(2000);
    numList.push_back(5000);

    std::vector<std::string> distTypes;
    distTypes.push_back("random");
    distTypes.push_back("cluster");

    // 运行性能测试（C++98兼容：嵌套模板加空格）
    std::map<std::string, std::map<std::string, std::vector<double> > > results = testPerformance(numList, distTypes);
    std::vector<SortAlg> sortAlgorithms = createSortAlgorithms();

    // 输出汇总结果
    std::cout << "\n===== 性能测试汇总 =====" << std::endl;
    for (size_t a = 0; a < sortAlgorithms.size(); ++a) {
        const SortAlg& alg = sortAlgorithms[a];
        const std::string& algName = alg.name;
        std::cout << "\n" << algName << ":" << std::endl;
        for (size_t d = 0; d < distTypes.size(); ++d) {
            const std::string& dist = distTypes[d];
            std::cout << "  " << dist << ": ";
            const std::vector<double>& times = results[algName][dist];
            for (size_t t = 0; t < times.size(); ++t) {
                std::cout << std::fixed << std::setprecision(6) << times[t] << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
