import matplotlib.pyplot as plt
import csv

instances = ["kroAB100", "kroAB200"]
versions = ["best", "median"]
solvers = ["nsga2", "nspso", "moead", "mhaco", "ihs", "nsmpbrkga", "nsmpbrkgaB", "nsmpbrkgaC", "nsmpbrkgaD"]

for instance in instances:
    for version in versions:
        plt.figure()
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Hypervolume")
        for solver in solvers:
            filename = "hypervolume_snapshots/" + instance + "_" + solver + "_" + version + ".txt"
            x = []
            y = []
            with open(filename) as csv_file:
                data = csv.reader(csv_file, delimiter = ",")
                for row in data:
                    x.append(float(row[1]))
                    y.append(float(row[2]))
            plt.plot(x, y, label = solver.upper())
        plt.xlim(left = 0)
        plt.ylim(bottom = 0, top = 1)
        plt.legend(loc = 'best')
        plt.savefig("hypervolume_snapshots/" + instance + "_" + version + ".svg", format = "svg")
        plt.savefig("hypervolume_snapshots/" + instance + "_" + version + ".eps", format = "eps")
        plt.savefig("hypervolume_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()

        plt.figure()
        plt.title(instance)
        for i in range(len(solvers)):
            filename = "pareto/" + instance + "_" + solvers[i] + "_" + version + ".txt"
            x = []
            y = []
            with open(filename) as csv_file:
                data = csv.reader(csv_file, delimiter=" ")
                for row in data:
                    x.append(int(row[0]))
                    y.append(float(row[1]))
            plt.scatter(x, y, label = solvers[i].upper(), marker = (i + 3, 2, 0))
        plt.legend(loc = 'best')
        plt.savefig("pareto/" + instance + "_" + version + ".svg", format = "svg")
        plt.savefig("pareto/" + instance + "_" + version + ".eps", format = "eps")
        plt.savefig("pareto/" + instance + "_" + version + ".png", format = "png")
        plt.close()

        plt.figure()
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Non-dominated Solutions")
        for solver in solvers:
            filename = "non_dominated_snapshots/" + instance + "_" + solver + "_" + version + ".txt"
            x = []
            y = []
            with open(filename) as csv_file:
                data = csv.reader(csv_file, delimiter = " ")
                for row in data:
                    x.append(float(row[1]))
                    y.append(float(row[2]))
            plt.plot(x, y, label = solver.upper())
        plt.xlim(left = 0)
        plt.ylim(bottom = 0)
        plt.legend(loc = 'best')
        plt.savefig("non_dominated_snapshots/" + instance + "_" + version + ".svg", format = "svg")
        plt.savefig("non_dominated_snapshots/" + instance + "_" + version + ".eps", format = "eps")
        plt.savefig("non_dominated_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()

        plt.figure()
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Non-dominated Fronts")
        for solver in solvers:
            filename = "fronts_snapshots/" + instance + "_" + solver + "_" + version + ".txt"
            x = []
            y = []
            with open(filename) as csv_file:
                data = csv.reader(csv_file, delimiter = " ")
                for row in data:
                    x.append(float(row[1]))
                    y.append(float(row[2]))
            plt.plot(x, y, label = solver.upper())
        plt.xlim(left = 0)
        plt.ylim(bottom = 0)
        plt.legend(loc = 'best')
        plt.savefig("fronts_snapshots/" + instance + "_" + version + ".svg", format = "svg")
        plt.savefig("fronts_snapshots/" + instance + "_" + version + ".eps", format = "eps")
        plt.savefig("fronts_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()

        plt.figure()
        plt.title(instance)
        plt.xlabel("Time (s)")
        plt.ylabel("Elite Size")
        filename = "elite_sizes_snapshots/" + instance + "_nsmpbrkga_" + version + ".txt"
        x = []
        y = []
        with open(filename) as csv_file:
            data = csv.reader(csv_file, delimiter = " ")
            for row in data:
                x.append(float(row[1]))
                y.append(float(row[2]))
        plt.plot(x, y, label = solver.upper())
        plt.xlim(left = 0)
        plt.ylim(bottom = 0)
        plt.legend(loc = 'best')
        plt.savefig("elite_sizes_snapshots/" + instance + "_" + version + ".svg", format = "svg")
        plt.savefig("elite_sizes_snapshots/" + instance + "_" + version + ".eps", format = "eps")
        plt.savefig("elite_sizes_snapshots/" + instance + "_" + version + ".png", format = "png")
        plt.close()

    plt.figure()
    plt.title(instance)
    plt.ylabel("Hypervolume")
    xs = []
    for i in range(len(solvers)):
        filename = "hypervolume/" + instance + "_" + solvers[i] + ".txt"
        x = []
        with open(filename) as csv_file:
            data = csv.reader(csv_file)
            for row in data:
                x.append(float(row[0]))
        xs.append(x)
    plt.boxplot(xs, labels = [solver.upper() for solver in solvers])
    plt.ylim(bottom = 0, top = 1)
    plt.savefig("hypervolume/" + instance + ".svg", format = "svg")
    plt.savefig("hypervolume/" + instance + ".eps", format = "eps")
    plt.savefig("hypervolume/" + instance + ".png", format = "png")
    plt.close()

