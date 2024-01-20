import csv
import os
from plotter_definitions import *
import pandas as pd

dirname = os.path.dirname(__file__)

metrics_labels = ["Hypervolume Ratio", "Modified Inverted Generational Distance", "Multiplicative Epsilon Indicator"]

metrics = []

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter=",")
                    for row in data:
                        metrics.append(["MOTSP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[0], float(row[0])])
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter=",")
                    for row in data:
                        metrics.append(["MOTSP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[1], float(row[0])])
                    csv_file.close()

for instance in instances:
    for i in range(len(solvers)):
        for seed in seeds:
            filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solvers[i] + "_" + str(seed) + ".txt")
            if os.path.exists(filename):
                with open(filename) as csv_file:
                    data = csv.reader(csv_file, delimiter=",")
                    for row in data:
                        metrics.append(["MOTSP", instance, m_per_instance[instance], size_per_instance[instance], solver_labels[solvers[i]], seed, metrics_labels[2], float(row[0])])
                    csv_file.close()

df = pd.DataFrame(metrics, columns=["problem", "instance", "number of objectives", "chromosome size", "solver", "seed", "metric name", "metric value"])
df.to_csv(os.path.join(dirname, "metrics.csv"), index=False)