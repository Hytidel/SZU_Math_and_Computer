import numpy as np
import matplotlib.pyplot as plt

# 初始化参数
w = np.zeros(2)
b = 0
eta = 1

# 定义训练数据
X = np.array([[3, 2], [2, 1], [1, 2]])
y = np.array([1, 1, -1])


# 感知机算法
def perceptron(X, y, eta, w, b, max_iter=1000):
    for _ in range(max_iter):
        all_classified = True
        for i in range(len(y)):
            if y[i] * (np.dot(w, X[i]) + b) <= 0:   # 损失函数
                w += eta * y[i] * X[i]              # 更新权重 w
                b += eta * y[i]                     # 更新偏置 b
                all_classified = False
        if all_classified:                          # 如果所有数据点都被正确分类，则停止迭代
            break
    return w, b


# 运行感知机算法
w, b = perceptron(X, y, eta, w, b)

print(f"Final weights: {w}")
print(f"Final bias: {b}")

# 验证结果
for i in range(len(y)):
    result = np.sign(np.dot(w, X[i]) + b)
    print(f"Point {X[i]} classified as: {result}, expected: {y[i]}")

# 绘制数据点
for i in range(len(y)):
    if y[i] == 1:
        plt.scatter(X[i][0], X[i][1], color='blue', marker='o')
    else:
        plt.scatter(X[i][0], X[i][1], color='red', marker='x')

# 绘制分类线
x_values = np.linspace(0, 4, 100)
y_values = -(w[0] * x_values + b) / w[1]
plt.plot(x_values, y_values, color='green')

# 设置图例和标签
plt.xlim(0, 4)
plt.ylim(0, 4)
plt.xlabel('x1')
plt.ylabel('x2')
plt.title('Perceptron Classification')
plt.grid(True)

# 显示图形
plt.show()
