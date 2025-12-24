#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <string>

using namespace std;

// 定义常量
const int INF = 0; // 表示无边
const int V = 8;   // 顶点数量

// 全局变量初始化（C++98兼容方式）
vector<string> vertices;
vector<vector<int> > adj_matrix; // 注意空格

// 初始化函数
void initGraph() {
    // 初始化顶点名称
    vertices.push_back("A");
    vertices.push_back("B");
    vertices.push_back("C");
    vertices.push_back("D");
    vertices.push_back("E");
    vertices.push_back("F");
    vertices.push_back("G");
    vertices.push_back("H");
    
    // 初始化邻接矩阵
    vector<int> row0;
    row0.push_back(INF); row0.push_back(4); row0.push_back(INF); row0.push_back(6); row0.push_back(INF); row0.push_back(INF); row0.push_back(7); row0.push_back(INF);
    adj_matrix.push_back(row0);
    
    vector<int> row1;
    row1.push_back(4); row1.push_back(INF); row1.push_back(12); row1.push_back(INF); row1.push_back(9); row1.push_back(1); row1.push_back(INF); row1.push_back(INF);
    adj_matrix.push_back(row1);
    
    vector<int> row2;
    row2.push_back(INF); row2.push_back(12); row2.push_back(INF); row2.push_back(INF); row2.push_back(1); row2.push_back(2); row2.push_back(INF); row2.push_back(10);
    adj_matrix.push_back(row2);
    
    vector<int> row3;
    row3.push_back(6); row3.push_back(INF); row3.push_back(INF); row3.push_back(INF); row3.push_back(13); row3.push_back(INF); row3.push_back(2); row3.push_back(INF);
    adj_matrix.push_back(row3);
    
    vector<int> row4;
    row4.push_back(INF); row4.push_back(9); row4.push_back(1); row4.push_back(13); row4.push_back(INF); row4.push_back(5); row4.push_back(11); row4.push_back(8);
    adj_matrix.push_back(row4);
    
    vector<int> row5;
    row5.push_back(INF); row5.push_back(1); row5.push_back(2); row5.push_back(INF); row5.push_back(5); row5.push_back(INF); row5.push_back(INF); row5.push_back(3);
    adj_matrix.push_back(row5);
    
    vector<int> row6;
    row6.push_back(7); row6.push_back(INF); row6.push_back(INF); row6.push_back(2); row6.push_back(11); row6.push_back(INF); row6.push_back(INF); row6.push_back(14);
    adj_matrix.push_back(row6);
    
    vector<int> row7;
    row7.push_back(INF); row7.push_back(INF); row7.push_back(10); row7.push_back(INF); row7.push_back(8); row7.push_back(3); row7.push_back(14); row7.push_back(INF);
    adj_matrix.push_back(row7);
}

// 广度优先搜索 BFS
void BFS(int start) {
    vector<bool> visited(V, false);
    queue<int> q;
    
    visited[start] = true;
    q.push(start);
    
    cout << "BFS 遍历结果: ";
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        cout << vertices[u] << " ";
        
        for (int v = 0; v < V; ++v) {
            if (adj_matrix[u][v] != INF && !visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    cout << endl;
}

// 深度优先搜索 DFS（递归）
void DFSUtil(int u, vector<bool>& visited) {
    visited[u] = true;
    cout << vertices[u] << " ";
    
    for (int v = 0; v < V; ++v) {
        if (adj_matrix[u][v] != INF && !visited[v]) {
            DFSUtil(v, visited);
        }
    }
}

void DFS(int start) {
    vector<bool> visited(V, false);
    cout << "DFS 遍历结果: ";
    DFSUtil(start, visited);
    cout << endl;
}

// Dijkstra算法求最短路径
void Dijkstra(int start) {
    vector<int> dist(V, INT_MAX);
    vector<bool> visited(V, false);
    
    dist[start] = 0;
    
    for (int i = 0; i < V - 1; ++i) {
        // 找到未访问的距离最小的顶点
        int min = INT_MAX, u = -1;
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        
        visited[u] = true;
        
        // 更新邻接顶点的距离
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && adj_matrix[u][v] != INF && dist[u] != INT_MAX && 
                dist[u] + adj_matrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adj_matrix[u][v];
            }
        }
    }
    
    // 输出结果
    cout << "从 " << vertices[start] << " 出发的最短路径：" << endl;
    for (int i = 0; i < V; ++i) {
        cout << vertices[start] << " -> " << vertices[i] << " : " << dist[i] << endl;
    }
}

// Prim算法求最小生成树
void Prim(int start) {
    vector<int> key(V, INT_MAX);
    vector<int> parent(V, -1);
    vector<bool> visited(V, false);
    
    key[start] = 0;
    
    for (int i = 0; i < V - 1; ++i) {
        // 找到未访问的key值最小的顶点
        int min = INT_MAX, u = -1;
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && key[v] < min) {
                min = key[v];
                u = v;
            }
        }
        
        if (u == -1) break;
        
        visited[u] = true;
        
        // 更新邻接顶点的key值
        for (int v = 0; v < V; ++v) {
            if (adj_matrix[u][v] != INF && !visited[v] && adj_matrix[u][v] < key[v]) {
                parent[v] = u;
                key[v] = adj_matrix[u][v];
            }
        }
    }
    
    // 输出最小生成树
    cout << "最小生成树的边及权重：" << endl;
    int total_weight = 0;
    for (int i = 0; i < V; ++i) {
        if (parent[i] != -1) {
            cout << vertices[parent[i]] << " - " << vertices[i] << " : " << adj_matrix[i][parent[i]] << endl;
            total_weight += adj_matrix[i][parent[i]];
        }
    }
    cout << "最小生成树的总权重：" << total_weight << endl;
}

// 打印邻接矩阵
void printAdjMatrix() {
    cout << "图的邻接矩阵：" << endl;
    cout << "   ";
    for (int i = 0; i < vertices.size(); ++i) {
        const string& v = vertices[i];
        cout << v << "  ";
    }
    cout << endl;
    
    for (int i = 0; i < V; ++i) {
        cout << vertices[i] << "  ";
        for (int j = 0; j < V; ++j) {
            if (adj_matrix[i][j] == INF) {
                cout << "0  ";
            } else {
                cout << adj_matrix[i][j] << "  ";
            }
        }
        cout << endl;
    }
}

int main() {
    // 初始化图数据
    initGraph();
    
    // 打印邻接矩阵
    printAdjMatrix();
    cout << endl;
    
    // 从A(索引0)开始的BFS和DFS
    BFS(0);
    DFS(0);
    cout << endl;
    
    // Dijkstra最短路径
    Dijkstra(0);
    cout << endl;
    
    // Prim最小生成树
    Prim(0);
    
    return 0;
}
