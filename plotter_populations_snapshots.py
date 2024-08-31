import csv
import gc
import matplotlib
import matplotlib.pyplot as plt
import os
import seaborn as sns
from math import ceil, floor, sqrt
from plotter_definitions import *

matplotlib.use("agg")

dirname = os.path.dirname(__file__)

num_rows = floor(sqrt(len(solvers)))
num_cols = ceil((len(solvers)) / num_rows)

for m in ms:
    for instance in instances_per_m[m]:
        for version in versions:
            min_ys = []
            max_ys = []
            for i in range(m):
                min_ys.append(-1)
                max_ys.append(-1)
            for solver in solvers:
                num_snapshots = 0
                while True:
                    filename = os.path.join(dirname, "populations_snapshots/" + instance + "_" + solver + "_" + version + "_" + str(num_snapshots) + ".txt")
                    if not os.path.exists(filename):
                        break
                    with open(filename) as csv_file:
                        next(csv_file)
                        data = csv.reader(csv_file, delimiter=" ")
                        for row in data:
                            if (len(row) >= m):
                                for i in range(m):
                                    if min_ys[i] == -1 or min_ys[i] > float(row[i]):
                                        min_ys[i] = float(row[i])
                                    if max_ys[i] == -1 or max_ys[i] < float(row[i]):
                                        max_ys[i] = float(row[i])
                    num_snapshots += 1
            for i in range(m):
                delta_y = max_ys[i] - min_ys[i]
                min_ys[i] -= round(0.025 * delta_y)
                max_ys[i] += round(0.025 * delta_y)
            for snapshot in range(num_snapshots):
                fig = plt.figure(figsize = (5.0 * num_cols * m, 5.0 * num_rows * m), constrained_layout = True)
                fig.set_size_inches(5.0 * num_cols * m, 5.0 * num_rows * m)
                figs = fig.subfigures(nrows = num_rows, ncols = num_cols, wspace = 0.05, hspace = 0.05)
                for i in range(len(solvers)):
                    row = floor(i / num_cols)
                    col = i % num_cols
                    figs[row][col].suptitle(solver_labels[solvers[i]], fontsize = "x-large")
                    axs = figs[row][col].subplots(nrows = m, ncols = m)
                    filename = os.path.join(dirname, "populations_snapshots/" + instance + "_" + solvers[i] + "_" + version + "_" + str(snapshot) + ".txt")
                    if os.path.exists(filename):
                        ys = []
                        for j in range(m):
                            ys.append([])
                        with open(filename) as csv_file:
                            next(csv_file)
                            data = csv.reader(csv_file, delimiter=" ")
                            for row in data:
                                if (len(row) >= m):
                                    for j in range(m):
                                        ys[j].append(float(row[j]))
                            csv_file.close()
                        for j in range(m):
                            axs[j][j].set_xlim(left = min_ys[j], right = max_ys[j])
                            axs[j][j].set_xlabel(xlabel = "$f_{" + str(j + 1) + "}$", fontsize = "large")
                            axs[j][j].set_yticks([])
                            axs[j][j].set_ylabel(ylabel = "Density", fontsize = "large")
                            sns.kdeplot(data = ys[j], ax = axs[j][j], color = colors[i], marker = (i + 3, 2, 0), alpha = 0.50)
                            for k in range(m):
                                if j != k:
                                    axs[j][k].set_xlim(left = min_ys[k], right = max_ys[k])
                                    axs[j][k].set_ylim(bottom = min_ys[j], top = max_ys[j])
                                    axs[j][k].set_xlabel(xlabel = "$f_{" + str(k + 1) + "}$", fontsize = "large")
                                    axs[j][k].set_ylabel(ylabel = "$f_{" + str(j + 1) + "}$", fontsize = "large")
                                    axs[j][k].scatter(x = ys[k], y = ys[j], color = colors[i], marker = (i + 3, 2, 0), alpha = 0.50)
                    filename = os.path.join(dirname, "best_solutions_snapshots/" + instance + "_" + solvers[i] + "_" + version + "_" + str(snapshot) + ".txt")
                    if os.path.exists(filename):
                        ys = []
                        for j in range(m):
                            ys.append([])
                        with open(filename) as csv_file:
                            next(csv_file)
                            data = csv.reader(csv_file, delimiter=" ")
                            for row in data:
                                for j in range(m):
                                    ys[j].append(float(row[j]))
                            csv_file.close()
                        for j in range(m):
                            axs[j][j].set_xlim(left = min_ys[j], right = max_ys[j])
                            axs[j][j].set_xlabel(xlabel = "$f_{" + str(j + 1) + "}$", fontsize = "large")
                            axs[j][j].set_yticks([])
                            axs[j][j].set_ylabel(ylabel = "Density", fontsize = "large")
                            sns.kdeplot(data = ys[j], ax = axs[j][j], color = colors2[i], marker = (i + 3, 2, 0), alpha = 0.75)
                            for k in range(m):
                                if j != k:
                                    axs[j][k].set_xlim(left = min_ys[k], right = max_ys[k])
                                    axs[j][k].set_ylim(bottom = min_ys[j], top = max_ys[j])
                                    axs[j][k].set_xlabel(xlabel = "$f_{" + str(k + 1) + "}$", fontsize = "large")
                                    axs[j][k].set_ylabel(ylabel = "$f_{" + str(j + 1) + "}$", fontsize = "large")
                                    axs[j][k].scatter(x = ys[k], y = ys[j], color = colors2[i], marker = (i + 3, 2, 0), alpha = 0.75)
                fig.suptitle(instance, fontsize = "xx-large")
                filename = os.path.join(dirname, "populations_snapshots/" + instance + "_" + version + "_" + str(snapshot) + ".png")
                plt.savefig(filename, format = "png")
                plt.close(fig)
                plt.cla()
                del fig
                gc.collect()
