import csv
import matplotlib.pyplot as plt
import os
import statistics as stats
from math import ceil, floor, sqrt
from plotter_definitions import *

dirname = os.path.dirname(__file__)

min_multiplicative_epsilon = 1.0
max_multiplicative_epsilon = 0.0
for instance in instances:
    for solver in solvers:
        filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solver + ".txt")
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                min_multiplicative_epsilon = min(min_multiplicative_epsilon, float(row[0]))
                max_multiplicative_epsilon = max(max_multiplicative_epsilon, float(row[0]))
delta_multiplicative_epsilon = max_multiplicative_epsilon - min_multiplicative_epsilon
min_multiplicative_epsilon = max(min_multiplicative_epsilon - round(0.025 * delta_multiplicative_epsilon), 0.00)
max_multiplicative_epsilon = min(max_multiplicative_epsilon + round(0.025 * delta_multiplicative_epsilon), 1.00)

num_rows = floor(sqrt(len(instances)))
num_cols = ceil(len(instances)/floor(sqrt(len(instances))))
fig = plt.figure(figsize = (5 * num_cols, 5 * num_rows), constrained_layout = True)
figs = fig.subfigures(nrows = num_rows, ncols = num_cols, wspace = 0.05, hspace = 0.05)
for i in range(len(instances)):
    row = floor(i/num_cols)
    col = i%num_cols
    figs[row][col].suptitle(instances[i], fontsize = "x-large")
    ax = figs[row][col].subplots()
    ax.set_ylabel("Multiplicative Epsilon", fontsize = "large")
    ax.set_xlabel("Solver", fontsize = "large")
    xs = []
    for solver in solvers:
        filename = os.path.join(dirname, "multiplicative_epsilon/" + instances[i] + "_" + solver + ".txt")
        x = []
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                x.append(float(row[0]))
        xs.append(x)
    bp = ax.boxplot(xs, labels = [solver_labels[solver] for solver in solvers], patch_artist = True)
    for j in range(len(solvers)) :
        bp["boxes"][j].set_facecolor(colors[j])
        bp["medians"][j].set_color("black")
    ax.set_ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
fig.suptitle("MOTSP", fontsize = "xx-large")
filename = os.path.join(dirname, "multiplicative_epsilon/multiplicative_epsilon.png")
plt.savefig(filename, format = "png")
plt.close(fig)

multiplicative_epsilon = []

for solver in solvers:
    multiplicative_epsilon.append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        multiplicative_epsilon[i].append(float(row[0]))
                    csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Solver", fontsize = "x-large")
plt.ylabel("Multiplicative Epsilon", fontsize = "x-large")
bp = plt.boxplot(multiplicative_epsilon, labels = [solver_labels[solver] for solver in solvers], patch_artist = True)
for i in range(len(solvers)) :
    bp["boxes"][i].set_facecolor(colors[i])
    bp["medians"][i].set_color("black")
plt.ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
filename = os.path.join(dirname, "multiplicative_epsilon/multiplicative_epsilons.png")
plt.savefig(filename, format = "png")
plt.close()

multiplicative_epsilon_per_m = {}

for solver in solvers:
    multiplicative_epsilon_per_m[solver] = {}
    for m in ms:
        multiplicative_epsilon_per_m[solver][m] = []

for m in ms:
    for instance in instances_per_m[m]:
        for solver in solvers:
            for seed in seeds:
                filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solver + "_" + str(seed) + ".txt")
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            multiplicative_epsilon_per_m[solver][m].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Objectives", fontsize = "x-large")
plt.ylabel("Multiplicative Epsilon", fontsize = "x-large")
plt.xticks(ms)
for i in range(len(solvers)):
    y = []
    for m in ms:
        y.append(stats.mean(multiplicative_epsilon_per_m[solvers[i]][m]))
    plt.plot(ms, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.8)
plt.xlim(left = max(min(ms) - 1, 0), right = max(ms) + 1)
plt.ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "multiplicative_epsilon/multiplicative_epsilon_mean_per_m.png")
plt.savefig(filename, format = "png")
plt.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Objectives", fontsize = "x-large")
plt.ylabel("Multiplicative Epsilon", fontsize = "x-large")
plt.xticks(ms)
for i in range(len(solvers)):
    y0 = []
    y2 = []
    for m in ms:
        quantiles = stats.quantiles(multiplicative_epsilon_per_m[solvers[i]][m])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(ms, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    y1 = []
    for m in ms:
        quantiles = stats.quantiles(multiplicative_epsilon_per_m[solvers[i]][m])
        y1.append(quantiles[1])
    plt.plot(ms, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xlim(left = max(min(ms) - 1, 0), right = max(ms) + 1)
plt.ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "multiplicative_epsilon/multiplicative_epsilon_quartiles_per_m.png")
plt.savefig(filename, format = "png")
plt.close()

multiplicative_epsilon_per_size = {}

for solver in solvers:
    multiplicative_epsilon_per_size[solver] = {}
    for size in sizes:
        multiplicative_epsilon_per_size[solver][size] = []

for size in sizes:
    for instance in instances_per_size[size]:
        for solver in solvers:
            for seed in seeds:
                filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solver + "_" + str(seed) + ".txt")
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            multiplicative_epsilon_per_size[solver][size].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Vertices", fontsize = "x-large")
plt.ylabel("Multiplicative Epsilon", fontsize = "x-large")
plt.xticks(sizes)
for i in range(len(solvers)):
    y = []
    for size in sizes:
        y.append(stats.mean(multiplicative_epsilon_per_size[solvers[i]][size]))
    plt.plot(sizes, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xlim(left = max(min(sizes) - 1, 0), right = max(sizes) + 1)
plt.ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "multiplicative_epsilon/multiplicative_epsilon_mean_per_size.png")
plt.savefig(filename, format = "png")
plt.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Vertices", fontsize = "x-large")
plt.ylabel("Multiplicative Epsilon", fontsize = "x-large")
plt.xticks(sizes)
for i in range(len(solvers)):
    y0 = []
    y2 = []
    for size in sizes:
        quantiles = stats.quantiles(multiplicative_epsilon_per_size[solvers[i]][size])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(sizes, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    y1 = []
    for size in sizes:
        quantiles = stats.quantiles(multiplicative_epsilon_per_size[solvers[i]][size])
        y1.append(quantiles[1])
    plt.plot(sizes, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xlim(left = max(min(sizes) - 1, 0), right = max(sizes) + 1)
plt.ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "multiplicative_epsilon/multiplicative_epsilon_quartiles_per_size.png")
plt.savefig(filename, format = "png")
plt.close()
