library(ggplot2)
library(dplyr)
library(tidyr)

theme_set(theme_bw())
colors <- c("#D55E00", "#0072B2", "#009E73", "#E69F00", "#56B4E9", "#F0E442", "#CC79A7", "#999999")

tbl <- read.csv("data/cliques_core.csv") %>%
    select(model, n, m, tau, T, seed, core_lb, core_ub,
           core_size, nr_cliques, max_clique, time_cliques) %>%
    mutate(
        c = round(core_lb / sqrt(n), 2),
        ub_type = factor(case_when(
            core_ub == n ~
                "ub = n (no upper bound)",
            TRUE ~
                "ub = sqrt(n)"
        ), levels = c("ub = n (no upper bound)", "ub = sqrt(n)")),
        T_or_IRG = if_else(
            model == "irg",
            "IRG",
            paste("T =", T))
    )

head(tbl %>% filter(model == "irg"))

common <- list(
    geom_line(),
    geom_point(),
    scale_color_manual(values = colors),
    scale_y_log10(labels = scales::label_number(scale_cut = scales::cut_si(""))),
    scale_x_log10(labels = scales::label_number(scale_cut = scales::cut_si(""))),
    facet_grid(cols = vars(ub_type), rows = vars(c))
)

pdf("eval/runtime.pdf", width = 8, height = 7, onefile = TRUE)
##
ggplot(
    tbl,
    aes(x = n, y = time_cliques / 1000, shape = as.factor(tau), color = T_or_IRG)
) +
    common
##
ggplot(
    tbl,
    aes(x = n, y = m, shape = as.factor(tau), color = T_or_IRG)
) +
    geom_point() +
    common
#
dev.off()

