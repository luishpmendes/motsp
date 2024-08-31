import csv
import matplotlib.pyplot as plt
import os
import seaborn as sns
import ptitprince as pt
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

fig, axs = plt.subplots(1, ncols = len(metrics), figsize = (12.0 * len(metrics), 12.0 * 1), squeeze = False, num = 1, clear = True)
fig.suptitle("Multi-Objective Travelling Salesman Problem", fontsize = 42)
for j in range(len(metrics)):
    axs[0][j].set_xlabel(xlabel = metrics_labels[j], fontsize = "xx-large")
    pt.half_violinplot(data = metrics[j], ax = axs[0][j], palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
    sns.stripplot(data = metrics[j], ax = axs[0][j], palette = colors, orient = "h", size = 2, zorder = 0)
    sns.boxplot(data = metrics[j], ax = axs[0][j], orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
    axs[0][j].set_yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "x-large")
filename = os.path.join(dirname, "metrics/raincloud.png")
plt.savefig(filename, format = "png")

fig, axs = plt.subplots(nrows = len(metrics), ncols = len(metrics), figsize = (8.0 * len(metrics), 8.0 * len(metrics)), squeeze = False, num = 1, clear = True)
fig.set_size_inches(8.0 * len(metrics), 8.0 * len(metrics))
fig.suptitle("Multi-Objective Travelling Salesman Problem", fontsize = 36)
for i in range(len(solvers)):
    for j in range(len(metrics)):
        axs[j][j].set_xlabel(xlabel = metrics_labels[j], fontsize = "xx-large")
        axs[j][j].set_yticks([])
        axs[j][j].set_ylabel(ylabel = "Density", fontsize = "xx-large")
        sns.kdeplot(data = metrics[j][i], ax = axs[j][j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), cut = 0)
        axs[j][j].legend(loc = "best", fontsize = "x-large")
        for k in range(len(metrics)):
            if j != k:
                axs[j][k].set_xlabel(xlabel = metrics_labels[k], fontsize = "xx-large")
                axs[j][k].set_ylabel(ylabel = metrics_labels[j], fontsize = "xx-large")
                axs[j][k].scatter(x = metrics[k][i], y = metrics[j][i], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.60)
                axs[j][k].legend(loc = "best", fontsize = "x-large")
filename = os.path.join(dirname, "metrics/scatter.png")
plt.savefig(filename, format = "png")

for instance in instances:
    metrics = [[], [], []]

    for solver in solvers:
        metrics[0].append([])
        metrics[1].append([])
        metrics[2].append([])
    
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics[0][i].append(float(row[0]))
                    csv_file.close()

    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics[1][i].append(float(row[0]))
                    csv_file.close()

    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        metrics[2][i].append(float(row[0]))
                    csv_file.close()

    fig, axs = plt.subplots(nrows = 1, ncols = len(metrics), figsize = (12.0 * len(metrics), 12.0 * 1), squeeze = False, num = 1, clear = True)
    fig.set_size_inches(12.0 * len(metrics), 12.0 * 1)
    fig.suptitle(instance, fontsize = 42)
    for j in range(len(metrics)):
        axs[0][j].set_xlabel(xlabel = metrics_labels[j], fontsize = "xx-large")
        pt.half_violinplot(data = metrics[j], ax = axs[0][j], palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
        sns.stripplot(data = metrics[j], ax = axs[0][j], palette = colors, orient = "h", size = 2, zorder = 0)
        sns.boxplot(data = metrics[j], ax = axs[0][j], orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
        axs[0][j].set_yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "x-large")
    filename = os.path.join(dirname, "metrics/raincloud_" + instance + ".png")
    plt.savefig(filename, format = "png")

    fig, axs = plt.subplots(nrows = len(metrics), ncols = len(metrics), figsize = (5.0 * len(metrics), 5.0 * len(metrics)), squeeze = False, num = 1, clear = True)
    fig.set_size_inches(5.0 * len(metrics), 5.0 * len(metrics))
    fig.suptitle(instance, fontsize = 25)
    for i in range(len(solvers)):
        for j in range(len(metrics)):
            axs[j][j].set_xlabel(xlabel = metrics_labels[j])
            axs[j][j].set_yticks([])
            axs[j][j].set_ylabel(ylabel = "Density")
            sns.kdeplot(data = metrics[j][i], ax = axs[j][j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), cut = 0)
            axs[j][j].legend(loc = "best")
            for k in range(len(metrics)):
                if j != k:
                    axs[j][k].set_xlabel(xlabel = metrics_labels[k])
                    axs[j][k].set_ylabel(ylabel = metrics_labels[j])
                    axs[j][k].scatter(x = metrics[k][i], y = metrics[j][i], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.60)
                    axs[j][k].legend(loc = "best")
    filename = os.path.join(dirname, "metrics/scatter_" + instance + ".png")
    plt.savefig(filename, format = "png")
