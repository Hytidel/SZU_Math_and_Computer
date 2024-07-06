import numpy as np

# 定义输入的4x4数字图像
f = np.array([[1, 0, 2, 0],
              [3, 0, 4, 0],
              [5, 0, 6, 0],
              [7, 0, 8, 0]], dtype=float)

# 对图像进行二维FFT运算
F = np.fft.fft2(f)

# 打印结果
print(F)
