import csv
import matplotlib.pyplot as plt
import seaborn as sns
import ptitprince as pt
import os
import statistics as stats
from math import ceil, floor, sqrt
from plotter_definitions import *

dirname = os.path.dirname(__file__)

min_igd_plus = 1.0
max_igd_plus = 0.0
max_time = 0.0
for instance in instances:
    for solver in solvers:
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus_snapshots/" + instance + "_" + solver + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        min_igd_plus = min(min_igd_plus, float(row[2]))
                        max_igd_plus = max(max_igd_plus, float(row[2]))
                        max_time = max(max_time, float(row[1]))
delta_igd_plus = max_igd_plus - min_igd_plus
min_igd_plus = max(min_igd_plus - round(0.025 * delta_igd_plus), 0.00)
max_igd_plus = min(max_igd_plus + round(0.025 * delta_igd_plus), 1.00)

igd_plus_per_solver = {}
time_per_solver = {}

for solver in solvers:
    igd_plus_per_solver[solver] = []
    time_per_solver[solver] = []
    for i in range(num_snapshots):
        igd_plus_per_solver[solver].append([])
        time_per_solver[solver].append([])

igd_plus_per_snapshot = []

for i in range(num_snapshots):
    igd_plus_per_snapshot.append([])
    for solver in solvers:
        igd_plus_per_snapshot[i].append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    j = 0
                    for row in data:
                        time_per_solver[solvers[i]][j].append(float(row[1]))
                        igd_plus_per_solver[solvers[i]][j].append(float(row[2]))
                        igd_plus_per_snapshot[j][i].append(float(row[2]))
                        j += 1
                    csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Time (s)", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
for i in range(len(solvers)):
    x = []
    y = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        y.append(stats.mean(igd_plus_per_solver[solvers[i]][j]))
    plt.plot(x, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
# plt.xlim(left = 0.0, right = max_time)
# plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.xscale("log")
plt.yscale("log")
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus_snapshots/igd_plus_mean_snapshots.png")
plt.savefig(filename, format = "png")
plt.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Time (s)", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
for i in range(len(solvers)):
    x = []
    y0 = []
    y2 = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(igd_plus_per_solver[solvers[i]][j])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(x, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    x = []
    y1 = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(igd_plus_per_solver[solvers[i]][j])
        y1.append(quantiles[1])
    plt.plot(x, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
# plt.xlim(left = 0.0, right = max_time)
# plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.xscale("log")
plt.yscale("log")
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus_snapshots/igd_plus_quartiles_snapshots.png")
plt.savefig(filename, format = "png")
plt.close()

for snapshot in range(num_snapshots):
    plt.figure(figsize = (11, 11))
    plt.title("Multi-Objective Travelling Salesman Problem", fontsize = "xx-large")
    plt.xlabel("Modified Inverted Generational Distance", fontsize = "x-large")
    pt.half_violinplot(data = igd_plus_per_snapshot[snapshot], palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
    sns.stripplot(data = igd_plus_per_snapshot[snapshot], palette = colors, orient = "h", size = 2, zorder = 0)
    sns.boxplot(data = igd_plus_per_snapshot[snapshot], orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
    plt.xlim(left = 0.0, right = 1.0)
    plt.xticks(ticks = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], fontsize = "large")
    plt.yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "large")
    filename = os.path.join(dirname, "igd_plus_snapshots/snapshot_" + str(snapshot) + ".png")
    plt.savefig(filename, format = "png")
    plt.close()
