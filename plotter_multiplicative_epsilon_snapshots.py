import csv
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
import seaborn as sns
import ptitprince as pt
import os
import statistics as stats
from plotter_definitions import *
from functools import partial
import numpy as np
import pandas as pd

dirname = os.path.dirname(__file__)

min_multiplicative_epsilon = 1.0
max_multiplicative_epsilon = 0.0
max_time = 0.0
for instance in instances:
    for solver in solvers:
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/" + instance + "_" + solver + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    for row in data:
                        min_multiplicative_epsilon = min(min_multiplicative_epsilon, float(row[2]))
                        max_multiplicative_epsilon = max(max_multiplicative_epsilon, float(row[2]))
                        max_time = max(max_time, float(row[1]))
delta_multiplicative_epsilon = max_multiplicative_epsilon - min_multiplicative_epsilon
min_multiplicative_epsilon = max(min_multiplicative_epsilon - round(0.025 * delta_multiplicative_epsilon), 0.00)
max_multiplicative_epsilon = min(max_multiplicative_epsilon + round(0.025 * delta_multiplicative_epsilon), 1.00)

multiplicative_epsilon_per_solver = {}
time_per_solver = {}

for solver in solvers:
    multiplicative_epsilon_per_solver[solver] = []
    time_per_solver[solver] = []
    for i in range(num_snapshots):
        multiplicative_epsilon_per_solver[solver].append([])
        time_per_solver[solver].append([])

multiplicative_epsilon_per_snapshot = []

for i in range(num_snapshots):
    multiplicative_epsilon_per_snapshot.append([])
    for solver in solvers:
        multiplicative_epsilon_per_snapshot[i].append([])

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    j = 0
                    for row in data:
                        time_per_solver[solvers[i]][j].append(float(row[1]))
                        multiplicative_epsilon_per_solver[solvers[i]][j].append(float(row[2]))
                        multiplicative_epsilon_per_snapshot[j][i].append(float(row[2]))
                        j += 1
                    csv_file.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Time (s)")
plt.ylabel(fontsize="large", ylabel="Inverse Multiplicative Epsilon")
plt.tick_params(axis="both", which="both", labelsize="large")
plt.grid(alpha=0.5, color='gray', linestyle='dashed', linewidth=0.5, which='both')
for i in range(len(solvers)):
    x = []
    y = []
    z = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        y.append(stats.mean(multiplicative_epsilon_per_solver[solvers[i]][j]))
        z.append(stats.stdev(multiplicative_epsilon_per_solver[solvers[i]][j]))
    # Create a DataFrame to store the data
    data = {
        'Solver': [],
        'Time': [],
        'epsilon': [],
        'Standard Deviation': []
    }

    for solver in solvers:
        for j in range(num_snapshots):
            data['Solver'].append(solver)
            data['Time'].append(stats.mean(time_per_solver[solver][j]))
            data['epsilon'].append(stats.mean(multiplicative_epsilon_per_solver[solver][j]))
            data['Standard Deviation'].append(stats.stdev(multiplicative_epsilon_per_solver[solver][j]))

    df = pd.DataFrame(data)

    # Save the DataFrame to a CSV file
    csv_filename = os.path.join(dirname, "hypervolume_snapshots/hypervolume_data.csv")
    df.to_csv("multiplicative_epsilon_data.csv", index=False)
    plt.plot(x, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xscale("log")
plt.yscale("function", functions = (partial(np.power, 10.0), np.log10))
plt.legend(fontsize="large", loc="best")
plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%.1f'))
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.2f'))
plt.gca().yaxis.set_minor_formatter(FormatStrFormatter('%.2f'))
plt.tight_layout()
filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/multiplicative_epsilon_mean_snapshots.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/multiplicative_epsilon_mean_snapshots.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
plt.close()

plt.figure()
plt.xlabel(fontsize="large", xlabel="Time (s)")
plt.ylabel(fontsize="large", ylabel="Inverse Multiplicative Epsilon")
plt.tick_params(axis="both", which="both", labelsize="large")
plt.grid(alpha=0.5, color='gray', linestyle='dashed', linewidth=0.5, which='both')
for i in range(len(solvers)):
    x = []
    y0 = []
    y2 = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(multiplicative_epsilon_per_solver[solvers[i]][j])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(x, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    x = []
    y1 = []
    for j in range(num_snapshots):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(multiplicative_epsilon_per_solver[solvers[i]][j])
        y1.append(quantiles[1])
    plt.plot(x, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xscale("log")
plt.yscale("function", functions = (partial(np.power, 10.0), np.log10))
plt.legend(fontsize="large", loc="best")
plt.gca().xaxis.set_major_formatter(FormatStrFormatter('%.1f'))
plt.gca().yaxis.set_major_formatter(FormatStrFormatter('%.1f'))
plt.gca().yaxis.set_minor_formatter(FormatStrFormatter('%.1f'))
plt.tight_layout()
filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/multiplicative_epsilon_quartiles_snapshots.png")
plt.savefig(bbox_inches='tight', fname=filename, format="png")
filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/multiplicative_epsilon_quartiles_snapshots.pdf")
plt.savefig(bbox_inches='tight', fname=filename, format="pdf")
plt.close()

for snapshot in range(num_snapshots):
    plt.figure(figsize = (11, 11))
    plt.title("Multi-Objective Travelling Salesman Problem", fontsize = "xx-large")
    plt.xlabel("Multiplicative Epsilon", fontsize = "x-large")
    pt.half_violinplot(data = multiplicative_epsilon_per_snapshot[snapshot], palette = colors, orient = "h", width = 0.6, cut = 0.0, inner = None)
    sns.stripplot(data = multiplicative_epsilon_per_snapshot[snapshot], palette = colors, orient = "h", size = 2, zorder = 0)
    sns.boxplot(data = multiplicative_epsilon_per_snapshot[snapshot], orient = "h", width = 0.20, color = "black", zorder = 10, showcaps = True, boxprops = {'facecolor' : 'none', "zorder" : 10}, showfliers = True, whiskerprops = {'linewidth' : 2, "zorder" : 10}, flierprops = {'markersize' : 2})
    # plt.ylim(bottom = min_multiplicative_epsilon, top = max_multiplicative_epsilon)
    plt.xticks(ticks = [0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0], fontsize = "large")
    plt.yticks(ticks = list(range(len(solvers))), labels = [solver_labels[solver] for solver in solvers], fontsize = "large")
    filename = os.path.join(dirname, "multiplicative_epsilon_snapshots/snapshot_" + str(snapshot) + ".png")
    plt.savefig(filename, format = "png")
    plt.close()
