library(ggplot2)
library(dplyr)
library(ggpubr)
library(egg)

theme_set(theme_bw())
colors <- c("#D55E00", "#0072B2", "#009E73", "#E69F00", "#56B4E9", "#F0E442", "#CC79A7", "#999999")

tbl <- read.csv("data/cliques_gnp.csv") %>%
    select(model, n, p, nr_cliques, max_clique, time_cliques)

head(tbl)

y_limits <- tbl %>% 
    group_by(model, n, p) %>%
    summarize(nr_cliques = mean(nr_cliques)) %>%
    ungroup() %>% 
    summarize(min = min(nr_cliques), max = max(nr_cliques))

y_limits <- c(y_limits$min, y_limits$max)

common <- list(
    stat_summary(geom = "line", fun.data = mean_se),
    stat_summary(fun.data = mean_se, size = 0.2),
    scale_color_manual(values = colors),
    scale_y_log10(),
    ## scale_y_log10(
    ##     labels = scales::label_number(scale_cut = scales::cut_si(""))),
    coord_cartesian(ylim = y_limits, xlim = c(15, 65)),
    ylab("number of maximal cliques"),
    theme(legend.position = "top",
          legend.text = element_text(margin = margin(r = 0.4, unit = "cm")),
          legend.spacing.x = unit(0.0, "cm")))


p1 <- ggplot(tbl %>% filter(model == "superdense_gnp"),
             aes(x = n, y = nr_cliques, color = factor(round((1 - p) * n)))
             ) +
    common +
    labs(color = "c: ") +
    theme(legend.justification = 0.65) +
    annotation_logticks(sides = "l") +
    annotate("text", x = 35, y = 1e6, label = "p = 1 - c / n")
##
p2 <- ggplot(tbl %>% filter(model == "dense_gnp"),
             aes(x = n, y = nr_cliques, color = factor(p))
             ) +
    common +
    theme(legend.position = "none") +
    annotation_logticks(sides = "l")
##
p3 <- ggplot(tbl %>% filter(model == "dense_gnp"),
             aes(x = n, y = nr_cliques, color = factor(p))
             ) +
    common +
    labs(color = "p: ") +
    theme(legend.justification = 1.2) +
    scale_x_log10() +
    annotation_logticks()
##
p <- ggarrange(p2, p3 + rremove("ylab"), p1 + rremove("ylab"), nrow = 1)
##
cairo_pdf("eval/dense_gnp.pdf", width = 6, height = 2.3, onefile = TRUE)
p
dev.off()
