import csv
import os
import numpy as np
from plotter_definitions import *

dirname = os.path.dirname(__file__)

hypervolume_per_solver_per_instance = {}
hypervolume_per_solver = {}

for solver in solvers:
    hypervolume_per_solver_per_instance[solver] = {}
    hypervolume_per_solver[solver] = []

    for instance in instances:
        hypervolume_per_solver_per_instance[solver][instance] = []

        filename = os.path.join(dirname, "hypervolume/" + instance + "_" + solver + ".txt")
        if os.path.exists(filename):
            with open(filename) as csv_file:
                data = csv.reader(csv_file)
                for row in data:
                    hypervolume_per_solver_per_instance[solver][instance].append(float(row[0]))
                    hypervolume_per_solver[solver].append(float(row[0]))
                csv_file.close()
        else:
            print("File not found: " + filename)

print("Hypervolume:")

for instance in instances:
    print()
    print(instance + ":")
    for solver in solvers:
        print(solver + ":")
        print(str(np.mean(hypervolume_per_solver_per_instance[solver][instance])) + " +/- " + str(np.std(hypervolume_per_solver_per_instance[solver][instance])))

print()
print()
print("All intances:")
for solver in solvers:
    print(solver + ":")
    print(str(np.mean(hypervolume_per_solver[solver])) + " +/- " + str(np.std(hypervolume_per_solver[solver])))

print()
print()
print()

igd_plus_per_solver_per_instance = {}
igd_plus_per_solver = {}

for solver in solvers:
    igd_plus_per_solver_per_instance[solver] = {}
    igd_plus_per_solver[solver] = []

    for instance in instances:
        igd_plus_per_solver_per_instance[solver][instance] = []

        filename = os.path.join(dirname, "igd_plus/" + instance + "_" + solver + ".txt")
        if os.path.exists(filename):
            with open(filename) as csv_file:
                data = csv.reader(csv_file)
                for row in data:
                    igd_plus_per_solver_per_instance[solver][instance].append(float(row[0]))
                    igd_plus_per_solver[solver].append(float(row[0]))
                csv_file.close()
        else:
            print("File not found: " + filename)

print("IGD+:")

for instance in instances:
    print()
    print(instance + ":")
    for solver in solvers:
        print(solver + ":")
        print(str(np.mean(igd_plus_per_solver_per_instance[solver][instance])) + " +/- " + str(np.std(igd_plus_per_solver_per_instance[solver][instance])))

print()
print()
print("All intances:")
for solver in solvers:
    print(solver + ":")
    print(str(np.mean(igd_plus_per_solver[solver])) + " +/- " + str(np.std(igd_plus_per_solver[solver])))

print()
print()
print()

multiplicative_epsilon_per_solver_per_instance = {}
multiplicative_epsilon_per_solver = {}

for solver in solvers:
    multiplicative_epsilon_per_solver_per_instance[solver] = {}
    multiplicative_epsilon_per_solver[solver] = []

    for instance in instances:
        multiplicative_epsilon_per_solver_per_instance[solver][instance] = []

        filename = os.path.join(dirname, "multiplicative_epsilon/" + instance + "_" + solver + ".txt")
        if os.path.exists(filename):
            with open(filename) as csv_file:
                data = csv.reader(csv_file)
                for row in data:
                    multiplicative_epsilon_per_solver_per_instance[solver][instance].append(float(row[0]))
                    multiplicative_epsilon_per_solver[solver].append(float(row[0]))
                csv_file.close()
        else:
            print("File not found: " + filename)

print("Multiplicative Epsilon:")

for instance in instances:
    print()
    print(instance + ":")
    for solver in solvers:
        print(solver + ":")
        print(str(np.mean(multiplicative_epsilon_per_solver_per_instance[solver][instance])) + " +/- " + str(np.std(multiplicative_epsilon_per_solver_per_instance[solver][instance])))

print()
print()
print("All intances:")
for solver in solvers:
    print(solver + ":")
    print(str(np.mean(multiplicative_epsilon_per_solver[solver])) + " +/- " + str(np.std(multiplicative_epsilon_per_solver[solver])))
