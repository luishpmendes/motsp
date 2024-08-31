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
for instance in instances:
    for solver in solvers:
        filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solver + ".txt")
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                min_igd_plus = min(min_igd_plus, float(row[0]))
                max_igd_plus = max(max_igd_plus, float(row[0]))
delta_igd_plus = max_igd_plus - min_igd_plus
min_igd_plus = max(min_igd_plus - round(0.025 * delta_igd_plus), 0.00)
max_igd_plus = min(max_igd_plus + round(0.025 * delta_igd_plus), 1.00)

for instance in instances:
    plt.figure(figsize = (11, 11))
    plt.title(instance, fontsize = "xx-large")
    plt.xlabel("Modified Inverted Generational Distance", fontsize = "x-large")
    xs = []
    for solver in solvers:
        filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solver + ".txt")
        x = []
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                x.append(float(row[0]))
        xs.append(x)
    pt.half_violinplot(data = xs, palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
    sns.stripplot(data = xs, palette = colors, orient = "h", size = 2, zorder = 0)
    sns.boxplot(data = xs, orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
    plt.yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "large")
    filename = os.path.join(dirname, "igd_plus/" + instance + ".png")
    plt.savefig(filename, format = "png")
    plt.close()

igd_plus = []

for solver in solvers:
    igd_plus.append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        igd_plus[i].append(float(row[0]))
                    csv_file.close()

plt.figure(figsize = (11, 11))
plt.title("Multi-Objective Travelling Salesman Problem", fontsize = "xx-large")
plt.xlabel("Modified Inverted Generational Distance", fontsize = "x-large")
pt.half_violinplot(data = igd_plus, palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
sns.stripplot(data = igd_plus, palette = colors, orient = "h", size = 2, zorder = 0)
sns.boxplot(data = igd_plus, orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
plt.yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "large")
filename = os.path.join(dirname, "igd_plus/igd_plus.png")
plt.savefig(filename, format = "png")
plt.close()

igd_plus_per_m = {}

for solver in solvers:
    igd_plus_per_m[solver] = {}
    for m in ms:
        igd_plus_per_m[solver][m] = []

for m in ms:
    for instance in instances_per_m[m]:
        for solver in solvers:
            for seed in seeds:
                filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solver + "_" + str(seed) + ".txt")
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            igd_plus_per_m[solver][m].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Objectives", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
plt.xticks(ms)
for i in range(len(solvers)):
    y = []
    for m in ms:
        y.append(stats.mean(igd_plus_per_m[solvers[i]][m]))
    plt.plot(ms, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xlim(left = max(min(ms) - 1, 0), right = max(ms) + 1)
plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus/igd_plus_mean_per_m.png")
plt.savefig(filename, format = "png")
plt.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Objectives", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
plt.xticks(ms)
for i in range(len(solvers)):
    y0 = []
    y2 = []
    for m in ms:
        quantiles = stats.quantiles(igd_plus_per_m[solvers[i]][m])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(ms, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    y1 = []
    for m in ms:
        quantiles = stats.quantiles(igd_plus_per_m[solvers[i]][m])
        y1.append(quantiles[1])
    plt.plot(ms, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xlim(left = max(min(ms) - 1, 0), right = max(ms) + 1)
plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus/igd_plus_quartiles_per_m.png")
plt.savefig(filename, format = "png")
plt.close()

igd_plus_per_size = {}

for solver in solvers:
    igd_plus_per_size[solver] = {}
    for size in sizes:
        igd_plus_per_size[solver][size] = []

for size in sizes:
    for instance in instances_per_size[size]:
        for solver in solvers:
            for seed in seeds:
                filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solver + "_" + str(seed) + ".txt")
                if os.path.exists(filename):
                    with open(filename) as csv_file:
                        data = csv.reader(csv_file, delimiter = ",")
                        for row in data:
                            igd_plus_per_size[solver][size].append(float(row[0]))
                        csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Vertices", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
plt.xticks(sizes)
for i in range(len(solvers)):
    y = []
    for size in sizes:
        y.append(stats.mean(igd_plus_per_size[solvers[i]][size]))
    plt.plot(sizes, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xlim(left = max(min(sizes) - 1, 0), right = max(sizes) + 1)
plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus/igd_plus_mean_per_size.png")
plt.savefig(filename, format = "png")
plt.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Number of Vertices", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
plt.xticks(sizes)
for i in range(len(solvers)):
    y0 = []
    y2 = []
    for size in sizes:
        quantiles = stats.quantiles(igd_plus_per_size[solvers[i]][size])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(sizes, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    y1 = []
    for size in sizes:
        quantiles = stats.quantiles(igd_plus_per_size[solvers[i]][size])
        y1.append(quantiles[1])
    plt.plot(sizes, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xlim(left = max(min(sizes) - 1, 0), right = max(sizes) + 1)
plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus/igd_plus_quartiles_per_size.png")
plt.savefig(filename, format = "png")
plt.close()
