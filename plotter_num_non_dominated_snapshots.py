import csv
import matplotlib.pyplot as plt
import os
import statistics as stats
from plotter_definitions import *

dirname = os.path.dirname(__file__)

for instance in instances:
    for version in versions:
        plt.figure()
        plt.title(instance, fontsize = "xx-large")
        plt.xlabel("Time (s)", fontsize = "x-large")
        plt.ylabel("Non-dominated Solutions", fontsize = "x-large")
        for i in range(len(solvers)):
            filename = os.path.join(dirname, "num_non_dominated_snapshots/" + instance + "_" + solvers[i] + "_" + version + ".txt")
            if os.path.exists(filename):
                x = []
                y = []
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = " ")
                    for row in data:
                        x.append(float(row[1]))
                        y.append(float(row[2]))
                plt.plot(x, y, label = solver_labels[solvers[i]], color = colors[i], marker = (i + 3, 2, 0), alpha = 0.80)
        plt.xscale("log")
        plt.legend(loc = "best", fontsize = "large")
        filename = os.path.join(dirname, "num_non_dominated_snapshots/" + instance + "_" + version + ".png")
        plt.savefig(filename, format = "png")
        plt.close()

num_non_dominated_per_solver = {}
time_per_solver = {}

for solver in solvers:
    num_non_dominated_per_solver[solver] = []
    time_per_solver[solver] = []
    for i in range(num_snapshots):
        num_non_dominated_per_solver[solver].append([])
        time_per_solver[solver].append([])

num_non_dominated_per_snapshot = []

for i in range(num_snapshots):
    num_non_dominated_per_snapshot.append([])
    for solver in solvers:
        num_non_dominated_per_snapshot[i].append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "num_non_dominated_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = " ")
                    j = 0
                    for row in data:
                        time_per_solver[solvers[i]][j].append(float(row[1]))
                        num_non_dominated_per_solver[solvers[i]][j].append(float(row[2]))
                        num_non_dominated_per_snapshot[j][i].append(float(row[2]))
                        j += 1
                    csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Time (s)", fontsize = "x-large")
plt.ylabel("Non-dominated Solutions", fontsize = "x-large")
for i in range(len(solvers)):
    x = []
    y = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        y.append(stats.mean(num_non_dominated_per_solver[solvers[i]][j]))
    plt.plot(x, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xscale("log")
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "num_non_dominated_snapshots/num_non_dominated_mean_snapshots.png")
plt.savefig(filename, format = "png")
plt.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Time (s)", fontsize = "x-large")
plt.ylabel("Non-dominated Solutions", fontsize = "x-large")
for i in range(len(solvers)):
    x = []
    y0 = []
    y2 = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(num_non_dominated_per_solver[solvers[i]][j])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(x, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    x = []
    y1 = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(num_non_dominated_per_solver[solvers[i]][j])
        y1.append(quantiles[1])
    plt.plot(x, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xscale("log")
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "num_non_dominated_snapshots/num_non_dominated_quartiles_snapshots.png")
plt.savefig(filename, format = "png")
plt.close()
