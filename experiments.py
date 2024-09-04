#!/usr/bin/env python3
import os
import glob
import run
import multiprocessing
import re
from math import sqrt

run.use_cores(multiprocessing.cpu_count() - 4)

run.group("models")

######################################################################
# generate random graphs and compute their cliques
run.add(
    "gnp",
    "code/release/cliques_in_gnp -n [[n]] -p [[p]] --seed [[seed]] --no-header",
    {
        "n": [20, 25, 30, 35, 40, 45, 50, 55, 60, 65],
        "i": list(range(4)),
        "type": ["dense", "superdense"],
        "p": lambda args: (
            1 - [1, 2, 4, 8][int(args["i"])] / int(args["n"])
            if args["type"] == "superdense"
            else [0.6, 0.7, 0.8, 0.9][int(args["i"])]
        ),
        "seed": [
            17,
            34,
            56,
            291,
            482,
            1928,
            2839,
            3728,
            4029,
            7821,
            917,
            934,
            956,
            9291,
            9482,
            91928,
            92839,
            93728,
            94029,
            97821,
        ],
        "name": "gnp_n=[[n]]_p=[[p]]_seed=[[seed]]",
    },
    stdout_res="[[type]]_gnp,[[stdout]]",
    stdout_file="data/cliques_gnp/[[name]].csv",
    header_command="code/release/cliques_in_gnp --only-header",
    header_mod=lambda head: f"model,{head}",
)


def magnitude(model, tau, T):
    tau = float(tau)
    T = float(T)
    if tau == 2.2:
        return 1e3
    if tau == 2.5:
        return 1e6
    if tau == 2.8:
        return 1e12


run.add(
    "[[model]]",
    "code/release/cliques_in_core --model [[model]] -n [[n]] -T [[T]] --tau [[tau]] --core_lb [[core_lb]] --core_ub [[core_ub]] --seed [[seed]]",
    {
        "model": ["irg", "girg"],
        "tau": [2.2, 2.5, 2.8],
        "T": [0, 0.4, 0.8],
        "n_": [2**i for i in range(9)],
        "n": lambda a: int(a["n_"]) * int(magnitude(a["model"], a["tau"], a["T"])),
        "lb": [0.3, 0.5, 0.7],
        "core_lb": lambda args: round(float(args["lb"]) * sqrt(int(args["n"])), 2),
        "ub": [0, 1],
        "core_ub": lambda a: (
            round(sqrt(int(a["n"])), 2) if a["ub"] == "1" else int(a["n"])
        ),
        "seed": [17, 34, 56, 291, 482, 1928, 2839, 3728, 4029, 7821],
        # "seed": 34,
        "name": "[[model]]_core_n=[[n]]_tau=[[tau]]_T=[[T]]_core_lb=[[core_lb]]_core_ub=[[core_ub]]_seed=[[seed]]",
    },
    stdout_file="data/cliques_core/[[name]].csv",
    combinations_filter=lambda a: a["model"] == "girg" or a["T"] == 0,
)

run.run()

######################################################################
# merge csv

output_names = [
    name
    for name in [os.path.basename(os.path.dirname(dir)) for dir in glob.glob("data/*/")]
    if name != "graphs"
]
run.group("post")
run.add(
    "merge_csv",
    "data/merge-csv.sh data/[[name]]",
    {"name": output_names},
    # creates_file="data/[[name]].csv",
)
run.run()
