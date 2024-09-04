library(ggplot2)
library(dplyr)
library(tidyr)
library(latex2exp)
library(forcats)
library(ggpubr)

theme_set(theme_bw())
colors <- c("#D55E00", "#0072B2", "#009E73", "#E69F00", "#56B4E9", "#F0E442", "#CC79A7", "#999999")

tbl <- read.csv("data/cliques_core.csv") %>%
    select(model, n, tau, T, core_lb, core_ub, core_size, nr_cliques) %>%
    filter(round(core_lb / sqrt(n), 2) == 0.5) %>%
    mutate(
        ub_type = factor(case_when(core_ub == n ~ "n", TRUE ~ "sqrtn"),
                         levels=c("sqrtn", "n")),
        temperature = factor(case_when(model == "girg" ~ paste("T =", T), TRUE ~ "IRG"),
                             levels = c("T = 0", "T = 0.4", "T = 0.8", "IRG"))
    )

levels(tbl$ub_type) <- c(TeX("weights \\in [0.5 \\sqrt{n}, \\sqrt{n}]"),
                         TeX("weights \\in [0.5 \\sqrt{n}, n]"))

common <- list(
    stat_summary(geom = "line", fun.data = mean_se),
    stat_summary(fun.data = mean_se, size = 0.2),
    scale_color_manual(values = colors),
    ## scale_y_log10(labels = scales::label_number(scale_cut = scales::cut_si(""))),
    ## scale_x_log10(labels = scales::label_number(scale_cut = scales::cut_si(""))),
    scale_y_log10(),
    scale_x_log10(),
    labs(color = "temperature:", shape = TeX("\\tau{:}")),
    guides(color = guide_legend(order = 1), shape = guide_legend(order = 2)),
    ylab("number of maximal cliques"),
    theme(legend.text = element_text(margin = margin(r = 0.4, unit = "cm")),
          legend.spacing.x = unit(0.0, "cm"),
          legend.margin = margin(l = 0.8, unit = "cm")),
    facet_grid(. ~ ub_type, labeller = label_parsed)
)

p1 <- ggplot(tbl, aes(x = n,
                       y = nr_cliques,
                       color = temperature,
                       shape = factor(tau))) +
    common +
    xlab("n (number of vertices of the full graph)")

p2 <- ggplot(tbl, aes(x = core_size,
                      y = nr_cliques,
                      color = temperature,
                      shape = factor(tau))) +
    common +
    xlab("number of vertices of the subgraph")

p <- ggarrange(p1, p2, ncol = 1, common.legend = TRUE)


cairo_pdf("eval/irg_girg_core.pdf", width = 6, height = 4.6, onefile = TRUE)
p
dev.off()


