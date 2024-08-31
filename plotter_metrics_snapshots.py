import csv
import matplotlib.pyplot as plt
import os
import seaborn as sns
import ptitprince as pt
from plotter_definitions import *

dirname = os.path.dirname(__file__)

metrics_labels = ["Hypervolume Ratio", "Modified Inverted Generational Distance", "Multiplicative Epsilon Indicator"]

metrics_per_snapshot = [[], [], []]

for i in range(num_snapshots):
    metrics_per_snapshot[0].append([])
    metrics_per_snapshot[1].append([])
    metrics_per_snapshot[2].append([])
    for solver in solvers:
        metrics_per_snapshot[0][i].append([])
        metrics_per_snapshot[1][i].append([])
        metrics_per_snapshot[2][i].append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    j = 0
                    for row in data:
                        metrics_per_snapshot[0][j][i].append(float(row[2]))
                        j += 1
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    j = 0
                    for row in data:
                        metrics_per_snapshot[1][j][i].append(float(row[2]))
                        j += 1
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    j = 0
                    for row in data:
                        metrics_per_snapshot[2][j][i].append(float(row[2]))
                        j += 1
                    csv_file.close()

for snapshot in range(num_snapshots):
    fig, axs = plt.subplots(1, ncols = len(metrics_per_snapshot), figsize = (12.0 * len(metrics_per_snapshot), 12.0 * 1), squeeze = False, num = 1, clear = True)
    fig.suptitle("Multi-Objective Travelling Salesman Problem", fontsize = 42)
    for j in range(len(metrics_per_snapshot)):
        axs[0][j].set_xlabel(xlabel = metrics_labels[j], fontsize = "xx-large")
        pt.half_violinplot(data = metrics_per_snapshot[j][snapshot], ax = axs[0][j], palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
        sns.stripplot(data = metrics_per_snapshot[j][snapshot], ax = axs[0][j], palette = colors, orient = "h", size = 2, zorder = 0)
        sns.boxplot(data = metrics_per_snapshot[j][snapshot], ax = axs[0][j], orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
        # axs[0][j].set_xlim(left = 0.0, right = 1.0)
        axs[0][j].set_xticks(ticks = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], fontsize = "x-large")
        axs[0][j].set_yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "x-large")
    filename = os.path.join(dirname, "metrics_snapshots/raincloud_" + str(snapshot) + ".png")
    plt.savefig(filename, format = "png")

for snapshot in range(num_snapshots):
    fig, axs = plt.subplots(nrows = len(metrics_per_snapshot), ncols = len(metrics_per_snapshot), figsize = (8.0 * len(metrics_per_snapshot), 8.0 * len(metrics_per_snapshot)), squeeze = False, num = 1, clear = True)
    fig.set_size_inches(8.0 * len(metrics_per_snapshot), 8.0 * len(metrics_per_snapshot))
    fig.suptitle("Multi-Objective Travelling Salesman Problem", fontsize = 36)
    for i in range(len(solvers)):
        for j in range(len(metrics_per_snapshot)):
            axs[j][j].set_xlabel(xlabel = metrics_labels[j], fontsize = "xx-large")
            axs[j][j].set_ylabel(ylabel = "Density", fontsize = "xx-large")
            # axs[j][j].set_xlim(left = 0.0, right = 1.0)
            axs[j][j].set_xticks(ticks = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], fontsize = "x-large")
            axs[j][j].set_yticks([])
            sns.kdeplot(data = metrics_per_snapshot[j][snapshot][i], ax = axs[j][j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), cut = 0)
            axs[j][j].legend(loc = "best", fontsize = "x-large")
            for k in range(len(metrics_per_snapshot)):
                if j != k:
                    axs[j][k].set_xlabel(xlabel = metrics_labels[k], fontsize = "xx-large")
                    axs[j][k].set_ylabel(ylabel = metrics_labels[j], fontsize = "xx-large")
                    # axs[j][k].set_xlim(left = 0.0, right = 1.0)
                    # axs[j][k].set_ylim(bottom = 0.0, top = 1.0)
                    axs[j][k].set_xticks(ticks = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], fontsize = "x-large")
                    axs[j][k].set_yticks(ticks = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], fontsize = "x-large")
                    axs[j][k].scatter(x = metrics_per_snapshot[k][snapshot][i], y = metrics_per_snapshot[j][snapshot][i], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.60)
                    axs[j][k].legend(loc = "best", fontsize = "x-large")
    filename = os.path.join(dirname, "metrics_snapshots/scatter_" + str(snapshot) + ".png")
    plt.savefig(filename, format = "png")
