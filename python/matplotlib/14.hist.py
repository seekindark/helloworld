import matplotlib.pyplot as plt
import numpy as np

plt.figure()
x = np.random.gamma(3, 4, 200)
plt.subplot(221)
plt.hist(x, bins=30)
plt.subplot(222)
plt.hist(x, bins=30)
plt.subplot(223)
plt.hist(x, bins=30, cumulative=True)
plt.subplot(224)
plt.hist(x, bins=30, cumulative=30, histtype="step")

plt.show()