# Maximal Cliques in Scale-Free Random Graphs #

Code for reproducing the experiments in
[arxiv.org/abs/2309.02990](https://arxiv.org/abs/2309.02990).

```bibtex
@article{Maxim_Cliqu_Scale_Rando_pre2023,
  title     = {Maximal Cliques in Scale-Free Random Graphs},
  author    = {Thomas Bläsius and Maximilian Katzmann and Clara Stegehuis},
  year      = {2023},
  journal   = {Computing Research Repository (CoRR)},
  volume    = {abs/2309.02990},
  doi       = {10.48550/arXiv.2309.02990}
}
```

The following instructions assume that you are on a Linux system with
basic development tools (python, cmake, c++ compiler...).  Adjustments
may be necessary for something different.

## Setup ##

We use the following other projects that are included as git
submodules.

  * [girgs](https://github.com/chistopher/girgs): Generator for
    Geometric Inhomogeneous Random Graphs (GIRGs).
  * [quick-cliques](https://github.com/darrenstrash/quick-cliques):
    Algorithm for enumerating all maximal cliques in a graph.
  * [run](https://github.com/thobl/run): Python module for running
    experiments.

Download these submodules with:

```console
git submodule init
git submodule update
```

Then compile the code:

```console
mkdir code/release
cd code/release
cmake ..
make
```

## Running the Experiments ##

The following commands set up a python virtual environment, enter it,
install the required python dependencies, run the experiments, and
exist the virtual environment.

```console
python -m venv .venv
source .venv/bin/activate
pip install pathos filelock tqdm
python experiments.py models post
deactivate
```

The command that actually runs the experiments is `python
experiments.py models post`.  It takes some time (about 20 min on my
laptop) and uses all but 4 available cores.  To adjust the number of
used cores, modify the line
`run.use_cores(multiprocessing.cpu_count() - 4)` in `experiments.py`
to your liking.

The resulting data can be found in `data/cliques_core.csv` and
`data/cliques_gnp.csv`.

## Creating the Plots ##

Make sure that the files `data/cliques_core.csv` and
`data/cliques_gnp.csv` exist (either by creating them as described
above or by renaming the precomputed data files
`precomp_cliques_core.csv` and `precomp_cliques_gnp.csv`).

Creating the plots requires some `R` packages, which you might need to
install.

```console
R -e "install.packages(c('tidyverse', 'egg', 'ggpubr', 'latex2exp'), repos = 'http://cran.us.r-project.org')"
```

Create the plots by running the `R` scripts.  The plots will be
created in the `eval/` folder.

```console
Rscript eval/dense_gnp.R
Rscript eval/irg_girg_core.R
Rscript eval/runtime.R
```
