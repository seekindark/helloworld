import numpy as np
import matplotlib.pyplot as plt

y1 = np.array([68.03, 68.83, 93.53, 70.67, 87.70, 91.04, 68.42, 69.92, 96.49, 88.64, 82.91, 88.22, 93.06, 95.42, 81.32, 82.38, 64.71, 61.16, 73.85, 79.17, 91.94, 94.12,100.00, 81.33,80.61])
y2 = np.array([13.78, 18.96, 1.99, 9.03, 5.74, 4.25, 21.05, 22.76, 0.81, 2.20, 5.06, 1.72, 3.47,  4.58,  7.14,  8.81,  8.82, 15.70,  4.62, 2.08,  0.00,  5.88,  0.00, 10.67,  6.12])
y3 = np.array([17.36, 10.83,  3.23, 19.60,  2.46,  3.77, 10.53,  6.91,  2.70,  5.13, 12.03,  9.48,  3.47,  0.00, 11.54,  8.29, 16.91, 22.31, 20.77, 18.75,  6.45,  0.00,  0.00,  6.67, 10.20])
y4 = np.array([0.83, 1.18, 0.75, 0.66, 2.46, 0.47, 0.00, 0.00, 0.00, 3.66, 0.00, 0.57, 0.00, 0.00, 0.00, 0.52, 8.82, 0.00, 0.77, 0.00, 1.61, 0.00, 0.00, 1.33, 2.04])
y5 = np.array([0.00, 0.20, 0.50, 0.04, 1.64, 0.47, 0.00, 0.41, 0.00, 0.37, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.74, 0.83, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 1.02])
labels = ['hold-obj', 'sit-instr', 'ride-instr', 'look-obj', 'hit-instr', 'hit-obj', 'eat-obj', 'eat-instr', 'jump-instr', 'lay-instr', 'talk_on_phone-instr', 'carry-obj',  'throw-obj', 'catch-obj', 'cut-instr',  'cut-obj', 'work_on_computer-instr', 'ski-instr', 'surf-instr', 'skateboard-instr', 'drink-instr', 'kick-obj', 'point-instr', 'read-obj', 'snowboard-instr']

plt.barh(labels, y1, color='green', label='Incorrect label')
plt.barh(labels, y2, left=y1, color='red', label='Occlusion')
plt.barh(labels, y3, left=y1+y2, color='blue', label='O_mislocalization')
plt.barh(labels, y4, left=y1+y2+y3, color='yellow', label='H_mislocalization')
plt.barh(labels, y5, left=y1+y2+y3+y4, color='black', label='Background')

plt.title("Error Analysis")                                                        #图片标题
plt.xlabel("Percent")                                                              #x轴标题
#plt.legend(loc=[0, 0])                     #图例的显示位置设置
plt.legend()                                #图例默认位置                              
res = plt.savefig("Error Analysis.png", bbox_inches='tight')          #保存图片命令一定要放在plt.show()前面
print("start to show the vertical stackedbarChar, res=", res)
plt.show()