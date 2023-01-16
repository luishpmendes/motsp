import csv
import matplotlib.pyplot as plt
import os
import seaborn as sns
from plotter_definitions import *

dirname = os.path.dirname(__file__)

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
                    filename = os.path.join(dirname, "best_solutions_snapshots/" + instance + "_" + solver + "_" + version + "_" + str(num_snapshots) +  ".txt")
                    if not os.path.exists(filename):
                        break
                    with open(filename) as csv_file:
                        next(csv_file)
                        data = csv.reader(csv_file, delimiter = " ")
                        for row in data:
                            for i in range(m):
                                if min_ys[i] == -1 or min_ys[i] > float(row[i]):
                                    min_ys[i] = float(row[i])
                                if max_ys[i] == -1 or max_ys[i] < float(row[i]):
                                    max_ys[i] = float(row[i])
                        csv_file.close()
                    num_snapshots += 1
            for i in range(m):
                delta_y = max_ys[i] - min_ys[i]
                min_ys[i] = min_ys[i] - round(0.025 * delta_y)
                max_ys[i] = max_ys[i] + round(0.025 * delta_y)
            for snapshot in range(num_snapshots):
                fig, axs = plt.subplots(nrows = m, ncols = m, figsize = (5.0 * m, 5.0 * m), squeeze = False, num = 1, clear = True)
                fig.set_size_inches(5.0 * m, 5.0 * m)
                fig.suptitle(instance, fontsize = "xx-large")
                for i in range(len(solvers)):
                    filename = os.path.join(dirname, "best_solutions_snapshots/" + instance + "_" + solvers[i] + "_" + version + "_" + str(snapshot) +  ".txt")
                    if os.path.exists(filename):
                        ys = []
                        for j in range(m):
                            ys.append([])
                        with open(filename) as csv_file:
                            next(csv_file)
                            data = csv.reader(csv_file, delimiter = " ")
                            for row in data:
                                for j in range(m):
                                    ys[j].append(float(row[j]))
                            csv_file.close()
                        for j in range(m):
                            axs[j][j].set_xlim(left = min_ys[j], right = max_ys[j])
                            axs[j][j].set_xlabel(xlabel = "$f_{" + str(j + 1) + "}$", fontsize = "x-large")
                            axs[j][j].set_yticks([])
                            axs[j][j].set_ylabel(ylabel = "Density", fontsize = "x-large")
                            sns.kdeplot(data = ys[j], ax = axs[j][j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.80)
                            axs[j][j].legend(loc = "best", fontsize = "large")
                            for k in range(m):
                                if (j != k):
                                    axs[j][k].set_xlim(left = min_ys[k], right = max_ys[k])
                                    axs[j][k].set_ylim(bottom = min_ys[j], top = max_ys[j])
                                    axs[j][k].set_xlabel(xlabel = "$f_{" + str(k + 1) + "}$", fontsize = "x-large")
                                    axs[j][k].set_ylabel(ylabel = "$f_{" + str(j + 1) + "}$", fontsize = "x-large")
                                    axs[j][k].scatter(x = ys[k], y = ys[j], color = colors[i], label = solver_labels[solvers[i]], marker = (i + 3, 2, 0), alpha = 0.80)
                                    axs[j][k].legend(loc = "best", fontsize = "large")
                plt.subplots_adjust(wspace = 0.15 + 0.05 * m, hspace = 0.15 + 0.05 * m)
                filename = os.path.join(dirname, "best_solutions_snapshots/" + instance + "_" + version + "_" + str(snapshot) + ".png")
                plt.savefig(filename, format = "png")
