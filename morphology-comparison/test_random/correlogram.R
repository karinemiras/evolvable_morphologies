# devtools::install_github("kassambara/ggcorrplot")
library(ggplot2)
library(ggcorrplot)

# Correlation matrix
#data(mtcars)

measures <- read.table("/Users/karinemiras/Documents/measures.txt",  header = TRUE, dec=".")
measures <- measures[, setdiff(names(measures), c("genome", "effective_ap_h_joints"))]

corr <- round(cor(measures), 1)

# Plot
ggcorrplot(corr, hc.order = TRUE, 
           type = "lower", 
           lab = TRUE, 
           lab_size = 3, 
           method="circle", 
           colors = c("tomato2", "white", "springgreen3"), 
           title="Correlogram", 
           ggtheme=theme_bw)
