import matplotlib.pyplot as plt

labels = ['G1', 'G2', 'G3', 'G4', 'G5']

men_means = [20, 35, 30, 35, 27]
women_means = [25, 32, 34, 20, 25]
pet_means = [50, 10, 70, 80, 90]

men_std = [2, 3, 4, 1, 2]
women_std = [3, 5, 2, 3, 3]
pet_std = [1, 2, 3, 4, 5]

width = 0.3       # the width of the bars: can also be len(x) sequence

fig, ax = plt.subplots()

ax.bar(labels, pet_means, width, label='Pet', hatch='/')
ax.bar(labels, men_means, width, bottom=pet_means, label='Men')
ax.bar(labels, women_means, width, label='Women')

ax.set_ylabel('Scores')

ax.set_title('Scores by group and gender')
ax.legend()

plt.show()