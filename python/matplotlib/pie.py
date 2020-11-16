
import matplotlib.pyplot as plt
labels = 'frogs', 'hogs', 'dogs', 'logs'
sizes = 15, 20, 45, 10
colors = 'yellowgreen', 'gold', 'lightskyblue', 'lightcoral'
explode = 0.2, 0.1, 0, 0
plt.pie(sizes, explode=explode, labels=labels, colors=colors, shadow=True, startangle=50)
plt.axis('equal')
print("Start to show the pie diagram !")
plt.show()
