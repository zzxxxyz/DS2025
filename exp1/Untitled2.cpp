#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <stdexcept>
#include <cmath>

using namespace std;

// 定义运算符类型
typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator;

// 运算符总数
#define N_OPTR 9

// 运算符优先级表，pri[栈顶运算符][当前运算符]
const char pri[N_OPTR][N_OPTR] = {
    // 当前运算符: +    -    *    /    ^    !    (    )    \0
    /* + */    { '>', '>', '<', '<', '<', '<', '<', '>', '>' },
    /* - */    { '>', '>', '<', '<', '<', '<', '<', '>', '>' },
    /* * */    { '>', '>', '>', '>', '<', '<', '<', '>', '>' },
    /* / */    { '>', '>', '>', '>', '<', '<', '<', '>', '>' },
    /* ^ */    { '>', '>', '>', '>', '>', '<', '<', '>', '>' },
    /* ! */    { '>', '>', '>', '>', '>', '>', ' ', '>', '>' },
    /* ( */    { '<', '<', '<', '<', '<', '<', '<', '=', ' ' },
    /* ) */    { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
    /* \0 */   { '<', '<', '<', '<', '<', '<', '<', ' ', '=' }
};

// 将字符转换为运算符类型
Operator charToOperator(char c) {
    switch (c) {
        case '+': return ADD;
        case '-': return SUB;
        case '*': return MUL;
        case '/': return DIV;
        case '^': return POW;
        case '!': return FAC;
        case '(': return L_P;
        case ')': return R_P;
        case '\0': return EOE;
        default: throw runtime_error("无效的运算符");
    }
}

// 计算阶乘
long long factorial(int n) {
    if (n < 0) throw runtime_error("阶乘参数不能为负数");
    long long result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// 计算两个数的运算结果
double calculate(double a, double b, Operator op) {
    switch (op) {
        case ADD: return a + b;
        case SUB: return a - b;
        case MUL: return a * b;
        case DIV:
            if (b == 0) throw runtime_error("除数不能为零");
            return a / b;
        case POW: return pow(a, b);
        default: throw runtime_error("不支持的二元运算符");
    }
}

// 字符串计算器主函数
double evaluateExpression(const string& expr) {
    stack<double> numStack;     // 存储数字的栈
    stack<Operator> opStack;   // 存储运算符的栈
    int i = 0;
    opStack.push(EOE);  // 起始符入栈

    while (i < (int)expr.length() || opStack.top() != EOE) {
        // 跳过空格
        if (i < (int)expr.length() && isspace(expr[i])) {
            i++;
            continue;
        }

        // 处理数字
        if (i < (int)expr.length() && isdigit(expr[i])) {
            double num = 0;
            // 处理整数部分
            while (i < (int)expr.length() && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            // 处理小数部分
            if (i < (int)expr.length() && expr[i] == '.') {
                i++;
                double fraction = 0.1;
                while (i < (int)expr.length() && isdigit(expr[i])) {
                    num += (expr[i] - '0') * fraction;
                    fraction *= 0.1;
                    i++;
                }
            }
            numStack.push(num);
        }
        // 处理运算符
        else {
            Operator currOp;
            if (i < (int)expr.length()) {
                currOp = charToOperator(expr[i]);
            } else {
                currOp = EOE;  // 表达式结束，当前运算符为终止符
            }

            Operator topOp = opStack.top();
            char priority = pri[topOp][currOp];

            switch (priority) {
                case '<':  // 栈顶运算符优先级低，当前运算符入栈
                    opStack.push(currOp);
                    if (i < (int)expr.length()) i++;
                    break;
                case '=':  // 括号匹配或表达式结束
                    opStack.pop();
                    if (i < (int)expr.length()) i++;
                    break;
                case '>':  // 栈顶运算符优先级高，计算
                    opStack.pop();  // 弹出运算符
                    if (topOp == FAC) {  // 单目运算符（阶乘）
                        if (numStack.empty()) {
                            throw runtime_error("阶乘运算缺少操作数");
                        }
                        double num = numStack.top();
                        numStack.pop();
                        // 计算阶乘并压栈
                        numStack.push(factorial((long long)num));
                    } else {  // 二元运算符
                        if (numStack.size() < 2) {
                            throw runtime_error("表达式格式错误，操作数不足");
                        }
                        double b = numStack.top();
                        numStack.pop();
                        double a = numStack.top();
                        numStack.pop();
                        numStack.push(calculate(a, b, topOp));
                    }
                    break;
                default:
                    throw runtime_error("表达式格式错误，运算符优先级关系异常");
            }
        }
    }

    if (numStack.size() != 1) {
        throw runtime_error("表达式格式错误，最终数字栈大小不为1");
    }

    return numStack.top();
}

// 测试案例
void testCalculator() {
    // 基本运算测试
    try {
        string expr1 = "3 + 4 * 2 / (1 - 5)";
        double res1 = evaluateExpression(expr1);
        cout << expr1 << " = " << res1 << endl;  // 预期结果: 1
    } catch (const exception& e) {
        cout << "表达式 3 + 4 * 2 / (1 - 5) 错误: " << e.what() << endl;
    }

    // 带括号的运算测试
    try {
        string expr2 = "(3 + 4) * 2 / (1 - 5)";
        double res2 = evaluateExpression(expr2);
        cout << expr2 << " = " << res2 << endl;  // 预期结果: -3.5
    } catch (const exception& e) {
        cout << "表达式 (3 + 4) * 2 / (1 - 5) 错误: " << e.what() << endl;
    }

    // 幂运算测试
    try {
        string expr3 = "2 ^ 3 + 5";
        double res3 = evaluateExpression(expr3);
        cout << expr3 << " = " << res3 << endl;  // 预期结果: 13
    } catch (const exception& e) {
        cout << "表达式 2 ^ 3 + 5 错误: " << e.what() << endl;
    }

    // 阶乘测试
    try {
        string expr6 = "5! + 3";
        double res6 = evaluateExpression(expr6);
        cout << expr6 << " = " << res6 << endl;  // 预期结果: 123
    } catch (const exception& e) {
        cout << "表达式 5! + 3 错误: " << e.what() << endl;
    }
}

int main() {
    // 运行测试案例
    testCalculator();

    // 交互式计算
    string expression;
    cout << "\n请输入表达式（输入q退出）: ";
    while (getline(cin, expression)) {
        if (expression == "q" || expression == "Q") {
            break;
        }

        try {
            double result = evaluateExpression(expression);
            cout << "结果: " << result << endl;
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }

        cout << "请输入表达式（输入q退出）: ";
    }

    return 0;
}

