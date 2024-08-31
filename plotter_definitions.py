instances = ["kroAB100",   "kroAB200",   "kroAB500",   "kroAB1000",   "kroAB2000",
             "kroABC100",  "kroABC200",  "kroABC500",  "kroABC1000",  "kroABC2000",
             "kroABCD100", "kroABCD200", "kroABCD500", "kroABCD1000", "kroABCD2000"]
solvers = ["nsga2", "nspso", "moead", "mhaco", "ihs", "nsbrkga"]
solver_labels = {"nsga2": "NSGA-II",
                 "nspso": "NSPSO",
                 "moead": "MOEA/D-DE",
                 "mhaco": "MHACO",
                 "ihs": "IHS",
                 "nsbrkga": "NS-BRKGA"}
seeds = [305089489, 511812191, 608055156, 467424509, 944441939, 414977408, 819312498, 562386085, 287613914, 755772793]
versions = ["best", "median"]
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd", "#8c564b", "#e377c2", "#7f7f7f", "#bcbd22", "#17becf", "#8c7e6e", "#738191"]
colors2 = ["#103c5a", "#804007", "#165016", "#6b1414", "#4a345f", "#462b26", "#723c61", "#404040", "#5e5f11", "#0c5f68", "#463f37", "#3a4149"]
ms = [2, 3, 4]
instances_per_m = {2 : ["kroAB100",   "kroAB200",   "kroAB500",   "kroAB1000",   "kroAB2000"],
                   3 : ["kroABC100",  "kroABC200",  "kroABC500",  "kroABC1000",  "kroABC2000"],
                   4 : ["kroABCD100", "kroABCD200", "kroABCD500", "kroABCD1000", "kroABCD2000"]}
m_per_instance = {"kroAB100"   : 2, "kroAB200"   : 2, "kroAB500"   : 2, "kroAB1000"   : 2, "kroAB2000"   : 2,
                  "kroABC100"  : 3, "kroABC200"  : 3, "kroABC500"  : 3, "kroABC1000"  : 3, "kroABC2000"  : 3,
                  "kroABCD100" : 4, "kroABCD200" : 4, "kroABCD500" : 4, "kroABCD1000" : 4, "kroABCD2000" : 4}
sizes = [100, 200, 500, 1000, 2000]
instances_per_size = {100  : ["kroAB100",  "kroABC100",  "kroABCD100"],
                      200  : ["kroAB200",  "kroABC200",  "kroABCD200"],
                      500  : ["kroAB500",  "kroABC500",  "kroABCD500"],
                      1000 : ["kroAB1000", "kroABC1000", "kroABCD1000"],
                      2000 : ["kroAB2000", "kroABC2000", "kroABCD2000"],}
size_per_instance = {"kroAB100"   : 100, "kroAB200"   : 200, "kroAB500"   : 500, "kroAB1000"   : 1000, "kroAB2000"   : 2000,
                     "kroABC100"  : 100, "kroABC200"  : 200, "kroABC500"  : 500, "kroABC1000"  : 1000, "kroABC2000"  : 2000,
                     "kroABCD100" : 100, "kroABCD200" : 200, "kroABCD500" : 500, "kroABCD1000" : 1000, "kroABCD2000" : 2000}
num_snapshots = 30
