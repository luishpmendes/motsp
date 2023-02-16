import csv
import matplotlib.pyplot as plt
import os
import seaborn as sns
from plotter_definitions import *

dirname = os.path.dirname(__file__)

metrics_labels = ["Hypervolume Ratio", "Modified Inverted Generational Distance", "Multiplicative Epsilon Indicator"]

metrics = [[], [], []]

for solver in solvers:
    metrics[0].append([])
    metrics[1].append([])
    metrics[2].append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics[0][i].append(float(row[0]))
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics[1][i].append(float(row[0]))
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics[2][i].append(float(row[0]))
                    csv_file.close()

fig, axs = plt.subplots(nrows = len(metrics), ncols = len(metrics), figsize = (5.0 * len(metrics), 5.0 * len(metrics)), squeeze = False, num = 1, clear = True)
fig.set_size_inches(5.0 * len(metrics), 5.0 * len(metrics))
fig.suptitle("Multi-Objective Multi-Dimensional Knapsack Problem", fontsize = 25)
for i in range(len(solvers)):
    for j in range(len(metrics)):
        axs[j][j].set_xlabel(xlabel = metrics_labels[j])
        axs[j][j].set_yticks([])
        axs[j][j].set_ylabel(ylabel = "Density")
        sns.kdeplot(data = metrics[j][i], ax = axs[j][j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.75)
        axs[j][j].legend(loc = "best")
        for l in range(len(metrics)):
            if j != l:
                axs[j][l].set_xlabel(xlabel = metrics_labels[l])
                axs[j][l].set_ylabel(ylabel = metrics_labels[j])
                axs[j][l].scatter(x = metrics[l][i], y = metrics[j][i], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.75)
                axs[j][l].legend(loc = "best")
        for l in range(len(metrics)):
            if j != l:
                axs[l][j].set_xlim(left = axs[j][j].get_xlim()[0], right = axs[j][j].get_xlim()[1])
                axs[j][l].set_ylim(bottom = axs[j][j].get_xlim()[0], top = axs[j][j].get_xlim()[1])
plt.subplots_adjust(wspace = 0.16 + 0.07 * len(metrics), hspace = 0.16 + 0.07 * len(metrics))
filename = os.path.join(dirname, "metrics/metrics.png")
plt.savefig(filename, format = "png")
