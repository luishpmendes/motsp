instances = ["kroAB100",   "kroAB200",   "kroAB500",   "kroAB1000",
             "kroABC100",  "kroABC200",  "kroABC500",  "kroABC1000",
             "kroABCD100", "kroABCD200", "kroABCD500", "kroABCD1000"]
solvers = ["nsga2", "nspso", "moead", "mhaco", "ihs", "nsbrkga"]
solver_labels = {"nsga2": "NSGA-II",
                 "nspso": "NSPSO",
                 "moead": "MOEA/D-DE",
                 "mhaco": "MHACO",
                 "ihs": "IHS",
                 "nsbrkga": "NS-BRKGA"}
seeds = [355841728, 682375620, 147607335, 606401489, 935899510]
versions = ["best", "median"]
colors = ["#1f77b4", "#ff7f0e", "#2ca02c", "#d62728", "#9467bd", "#8c564b", "#e377c2", "#7f7f7f", "#bcbd22", "#17becf", "#8c7e6e", "#738191"]
colors2 = ["#103c5a", "#804007", "#165016", "#6b1414", "#4a345f", "#462b26", "#723c61", "#404040", "#5e5f11", "#0c5f68", "#463f37", "#3a4149"]
ms = [2, 3, 4]
instances_per_m = {2 : ["kroAB100",   "kroAB200",   "kroAB500",   "kroAB1000"],
                   3 : ["kroABC100",  "kroABC200",  "kroABC500",  "kroABC1000"],
                   4 : ["kroABCD100", "kroABCD200", "kroABCD500", "kroABCD1000"]}
sizes = [100, 200, 500, 1000]
instances_per_size = {100  : ["kroAB100",  "kroABC100",  "kroABCD100"],
                      200  : ["kroAB200",  "kroABC200",  "kroABCD200"],
                      500  : ["kroAB500",  "kroABC500",  "kroABCD500"],
                      1000 : ["kroAB1000", "kroABC1000", "kroABCD1000"]}
num_snapshots = 60
