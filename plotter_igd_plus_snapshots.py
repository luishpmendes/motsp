import csv
import matplotlib.pyplot as plt
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

# for version in versions:
#     num_rows = floor(sqrt(len(instances)))
#     num_cols = ceil(len(instances)/floor(sqrt(len(instances))))
#     fig = plt.figure(figsize = (5 * num_cols, 5 * num_rows), constrained_layout = True)
#     figs = fig.subfigures(nrows = num_rows, ncols = num_cols, wspace = 0.05, hspace = 0.05)
#     for i in range(len(instances)):
#         row = floor(i/num_cols)
#         col = i%num_cols
#         figs[row][col].suptitle(instances[i], fontsize = "x-large")
#         ax = figs[row][col].subplots()
#         ax.set_ylabel("IGD+ x Time", fontsize = "large")
#         ax.set_xlabel("Time (s)", fontsize = "large")
#         for j in range(len(solvers)):
#             filename = os.path.join(dirname, "igd_plus_snapshots/" + instances[i] + "_" + solvers[j] + "_" + version + ".txt")
#             if os.path.exists(filename):
#                 x = []
#                 y = []
#                 with open(filename) as csv_file:
#                     data = csv.reader(csv_file, delimiter = ",")
#                     for row in data:
#                         x.append(float(row[1]))
#                         y.append(float(row[2]))
#                 ax.plot(x, y, label = solver_labels[solvers[j]], marker = (j + 3, 2, 0), color = colors[j], alpha = 0.80)
#         ax.set_xlim(left = 0.0, right = max_time)
#         ax.set_ylim(bottom = min_igd_plus, top = max_igd_plus)
#         ax.legend(loc = "best")
#     fig.suptitle("MOTSP", fontsize = "xx-large")
#     filename = os.path.join(dirname, "igd_plus_snapshots/igd_plus_snapshots_" + version + ".png")
#     plt.savefig(filename, format = "png")
#     plt.close(fig)

igd_plus_per_solver = {}
time_per_solver = {}

for solver in solvers:
    igd_plus_per_solver[solver] = []
    time_per_solver[solver] = []
    for i in range(num_snapshots + 1):
        igd_plus_per_solver[solver].append([])
        time_per_solver[solver].append([])

for instance in instances:
    for solver in solvers:
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus_snapshots/" + instance + "_" + solver + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter = ",")
                    i = 0
                    for row in data:
                        time_per_solver[solver][i].append(float(row[1]))
                        igd_plus_per_solver[solver][i].append(float(row[2]))
                        i += 1
                    csv_file.close()

plt.figure()
plt.title("MOTSP", fontsize = "xx-large")
plt.xlabel("Time (s)", fontsize = "x-large")
plt.ylabel("IGD+", fontsize = "x-large")
for i in range(len(solvers)):
    x = []
    y = []
    for j in range(num_snapshots + 1):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        y.append(stats.mean(igd_plus_per_solver[solvers[i]][j]))
    plt.plot(x, y, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.80)
plt.xlim(left = 0.0, right = max_time)
plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
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
    for j in range(num_snapshots + 1):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(igd_plus_per_solver[solvers[i]][j])
        y0.append(quantiles[0])
        y2.append(quantiles[2])
    plt.fill_between(x, y0, y2, color = colors[i], alpha = 0.25)
for i in range(len(solvers)):
    x = []
    y1 = []
    for j in range(num_snapshots + 1):
        x.append(stats.mean(time_per_solver[solvers[i]][j]))
        quantiles = stats.quantiles(igd_plus_per_solver[solvers[i]][j])
        y1.append(quantiles[1])
    plt.plot(x, y1, label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), color = colors[i], alpha = 0.75)
plt.xlim(left = 0.0, right = max_time)
plt.ylim(bottom = min_igd_plus, top = max_igd_plus)
plt.legend(loc = "best", fontsize = "large")
filename = os.path.join(dirname, "igd_plus_snapshots/igd_plus_quartiles_snapshots.png")
plt.savefig(filename, format = "png")
plt.close()
