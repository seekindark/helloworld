import numpy as np
import matplotlib.pyplot as plt

mu, sigma = 100, 15
x = mu + sigma * np.random.randn(10000)

# 数据的直方图 , 计算X 列表中的数值的分布 ！！
n, bins, patches = plt.hist(x, 'sqrt', density=True, facecolor='g',alpha=0.75)
print(x)
print("number of counts > 140 is ", sum(i > 140.0 for i in x))
'''
[103.55893023 120.64068019  88.04965886 ...  99.57227839  91.28307586
 101.57174131]
number of counts > 140 is  40
'''
plt.xlabel('Smarts')
plt.ylabel('Probability')
plt.title('Histogram of IQ')
plt.text(60, .025, r'$\mu=100,\ \sigma=15$')
plt.axis([40, 160, 0, 0.03])
plt.grid(True)
plt.show()