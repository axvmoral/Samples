The Effect of the Central America Regional Security Initiative on
Homicide
================
Axel V. Morales Sanchez

## Introduction

In this document I analyze the effect of the Central America Regional
Security Initiative (CARSI) on the homicide rate per 100,000 persons.
This analysis was completed as part of my final project for the Advanced
Quantitative Methods (ECON114) class at UCSC. I use a
difference-in-differences approach to estimate the effect in question.
The treatment group for this approach is taken to be the three countries
where CARSI was implemented in 2006: Honduras, El Salvador, and
Guatemala. These countries are often referred to as "The Northern
Triangle", and from here on will be referred to as such. The control
group is taken to be Nicaragua. The rationalization for this decision
comes from its likeness in many important measures to The Northern
Triangle, as well as its direct regional proximity. Balance tests for
these measures are computed to quantitatively justify this likeness.

The primary purpose for this document is to serve as an example of my
coding and data analysis capabilities. Please look to the article I
wrote that is published in the same repository for a proper write up of
the methods and results shown here.

## Analysis

``` r
library(tidyverse) # For data wrangling
library(reshape2) # For melting data
library(readxl) # For reading xlsx files
library(plotly) # For interactive maps
library(lmtest) # For robust standard errors
library(sandwich) # For robust standard errors
```

The chunk below creates a function that returns either a plot of
different measures of likeness between the treatment and control, or a
balance test for these measures between both groups. Note that for two
measures, the treatment and control are significantly different from
eachother. This is okay, as the difference-in-difference approach relies
heavily on the trend of data. That is, these measures, which are
established conflators of crime, do not significantly differ post CARSI,
and this ensures that these are not conflating the rise in the homicide
rate that we see in the treatment group. This is is shown in the lack of
significance for the interaction term in all balance tests. For the
other two measures, both the plot and balance tests show that they are
not significantly different from eachother.

``` r
balance.test <- function(data_path, target, specification, title){
  '''
  Wrangles datasets used for balance computation between the control and 
  treatment groups, and returns either a ggplot object visually displaying
  balance, or a difference-in-differences model to compute balance. 

  :param data_path: a string indicating the path to a dataset
  :param target: a string indicating the balance test target
  :param specification: a string indicating wether to return a model or plot
  :param title: a string indicating the title of the plot object
  :return either a plot displaying the target balance, or a model to compute
          the target balance
  '''
  source.data <- read_excel(data_path)
  ts.data <- source.data[-1] %>%
    filter(Country == "Nicaragua" | Country == "El Salvador" | Country == "Honduras" | Country == "Guatemala") %>%
    melt(id.vars = "Country", variable.name = "year", na.rm=TRUE)%>%
    mutate(NT.indicator = ifelse(Country == "El Salvador" | Country == "Honduras" | Country == "Guatemala", 1, 0),
           name = ifelse(NT.indicator == 1, "Northern Triangle", "Nicaragua"),
           value = as.numeric(value), year = as.integer(as.character(year))) %>%
    group_by(name, year, NT.indicator) %>%
    summarise(target = mean(value)) %>%
    mutate(post.policy.indicator = ifelse(year > 2006, 1, 0))
  if(specification == "plot"){
    plot <- ggplot(data = ts.data, mapping = aes(year, target)) +
      geom_line(mapping = aes(linetype = name, color = name)) +
      geom_vline(xintercept = 2007, linetype = "dotted", alpha=0.5) +
      geom_point(mapping = aes(shape = name, color = name)) +
      labs(x = "Year", y = target, color = "Region", linetype = "Region", shape = "Region", title = title)+
      scale_color_brewer(palette = "Paired") +
      theme_classic()
    return(plot)
  }
  if(specification == "model"){
    model.data <- subset(ts.data,year > 1998 & year < 2012)
    model <- lm(target ~ NT.indicator + post.policy.indicator + NT.indicator * post.policy.indicator, data = model.data)
    robust.model <- coeftest(model, vcov = vcovHC(model, type = "HC0"))
    return(robust.model)
  }
}
```

``` r
'''
Below I run the balance.test function for the relevant balance targets,
both for a plot and model.
'''
setwd("C:/Users/axvmo/Desktop/Apllying/example code/data/") # This is where all the relevant datasets were stored
balance.test("Gross national income (GNI) per capita (constant 2017 PPP$).xlsx", "GNI per Capita", "plot", "Year Gross National Income (GNI) per Capita by Region")
```

![](carsi-homicide-markdown_files/figure-gfm/balance%20tests-1.png)<!-- -->

``` r
balance.test("Gross national income (GNI) per capita (constant 2017 PPP$).xlsx", specification = "model")
```

    ## 
    ## t test of coefficients:
    ## 
    ##                                    Estimate Std. Error t value  Pr(>|t|)    
    ## (Intercept)                        3961.375     71.101 55.7148 < 2.2e-16 ***
    ## NT.indicator                       1755.958     93.374 18.8056 4.806e-15 ***
    ## post.policy.indicator               561.425     89.262  6.2896 2.493e-06 ***
    ## NT.indicator:post.policy.indicator   54.708    114.333  0.4785     0.637    
    ## ---
    ## Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

``` r
balance.test("Mean years of schooling (years).xlsx", "Mean Years of Schooling", "plot", "Year Mean Years of Schooling by Region")
```

![](carsi-homicide-markdown_files/figure-gfm/balance%20tests-2.png)<!-- -->

``` r
balance.test("Mean years of schooling (years).xlsx", specification = "model")
```

    ## 
    ## t test of coefficients:
    ## 
    ##                                     Estimate Std. Error t value  Pr(>|t|)    
    ## (Intercept)                         5.287500   0.067169 78.7190 < 2.2e-16 ***
    ## NT.indicator                       -0.687500   0.097706 -7.0364 4.639e-07 ***
    ## post.policy.indicator               0.612500   0.092259  6.6389 1.125e-06 ***
    ## NT.indicator:post.policy.indicator  0.080833   0.165944  0.4871     0.631    
    ## ---
    ## Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

``` r
balance.test("Life expectancy at birth (years).xlsx", "Life Expectancy at Birth (Years)", "plot", "Year Life Expectancy at Birth by Region")
```

![](carsi-homicide-markdown_files/figure-gfm/balance%20tests-3.png)<!-- -->

``` r
balance.test("Life expectancy at birth (years).xlsx", specification = "model")
```

    ## 
    ## t test of coefficients:
    ## 
    ##                                      Estimate Std. Error  t value  Pr(>|t|)    
    ## (Intercept)                        70.3625000  0.2703802 260.2354 < 2.2e-16 ***
    ## NT.indicator                       -0.4625000  0.3633581  -1.2728    0.2164    
    ## post.policy.indicator               1.8175000  0.3134094   5.7991 7.801e-06 ***
    ## NT.indicator:post.policy.indicator  0.0091667  0.4359742   0.0210    0.9834    
    ## ---
    ## Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

``` r
balance.test("Human Development Index (HDI).xlsx", "HDI", "plot", "Year Human Development Index (HDI) by Region")
```

![](carsi-homicide-markdown_files/figure-gfm/balance%20tests-4.png)<!-- -->

``` r
balance.test("Human Development Index (HDI).xlsx", specification = "model")
```

    ## 
    ## t test of coefficients:
    ## 
    ##                                     Estimate Std. Error  t value  Pr(>|t|)    
    ## (Intercept)                        0.5890000  0.0041345 142.4612 < 2.2e-16 ***
    ## NT.indicator                       0.0016667  0.0062386   0.2672    0.7918    
    ## post.policy.indicator              0.0298000  0.0048750   6.1128 3.749e-06 ***
    ## NT.indicator:post.policy.indicator 0.0022667  0.0071333   0.3178    0.7537    
    ## ---
    ## Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

``` r
'''
Below I wrangle the homicide rate data in a similar fashion to the data used for 
the balance targets. I then produce a similar plot.
'''
# Wrangle homicide data
source.homicide.data <- read_excel("homicide_country.xlsx")
rate.homicide.data <- source.homicide.data %>%
  select(Unit, Gender, Year, country, iso3_code, Value) %>%
  filter(Unit == "Rate per  100,000 population",
         Gender == "Total (all ages)",
         Year >= 1999 & Year <= 2014,
         iso3_code == "SLV" |
         iso3_code == "GTM" |
         iso3_code == "HND" |
         iso3_code == "NIC") %>%
  mutate(NT.indicator = ifelse(iso3_code == "SLV" |
         iso3_code == "GTM" |
         iso3_code == "HND", 1, 0),
         name = ifelse(NT.indicator == 1, "Northern Triangle", "Nicaragua"),
         Value = as.numeric(Value)) %>%
  group_by(name, Year, NT.indicator) %>%
  summarise(rate = mean(Value)) %>%
  mutate(post.policy.indicator = ifelse(Year>2006,1,0))
```

In the plot below, note the sharp rise in the homicide rate in the
treatment group after CARSI is implemented in 2006 (dotted line at 2007
to account for lagged effects) compared to the control. This gives
indication that CARSI had a positive effect on the homicide rate in the
treatment group. However, the sharp rise dies down after 2011. This
makes sense, as we would expect the effect of CARSI to start dying down
after half a decade of its implementation. Given this, the model is only
ran for years prior to 2012. This was the case for the balance tests
above, to keep everything consistent.

``` r
# Plot the homicide rate
ggplot(data = rate.homicide.data, mapping = aes(Year, rate)) +
  geom_line(mapping = aes(linetype = name, color = name)) +
  geom_vline(xintercept = 2007, linetype = "dotted", alpha = 0.5) +
  geom_point(mapping = aes(shape = name, color = name)) +
  labs(x = "Year", y = "Homicide Rate", color = "Region",
  linetype = "Region", shape = "Region",
  title = "Year Homicide Rate per 100,000 Persons by Region") +
  scale_color_brewer(palette = "Paired") +
  theme_classic()
```

![](carsi-homicide-markdown_files/figure-gfm/homicide%20plot-1.png)<!-- -->

The results of the model below show that CARSI had a statistically
signicant positive effect on the homicide rate. Extrapolating these
results, one can say that for countries similar to the treatment and
control groups, militant War on Drugs policies like CARSI will have a
short term increase in the homicide rate.

``` r
'''
Below I create a difference-in-differences model to compute the effect of CARSI on the 
homicide rate.
'''
# Create and compute the model
model.data<-subset(rate.homicide.data, Year>1998 & Year<2012)
model<-lm(rate ~ NT.indicator + post.policy.indicator + NT.indicator * post.policy.indicator, data = model.data)
robust.model <- coeftest(model, vcov = vcovHC(model, type="HC0"))
```

    ## 
    ## t test of coefficients:
    ## 
    ##                                    Estimate Std. Error t value  Pr(>|t|)    
    ## (Intercept)                        11.52625    0.45259 25.4673 < 2.2e-16 ***
    ## NT.indicator                       35.39125    1.08830 32.5197 < 2.2e-16 ***
    ## post.policy.indicator               1.61375    0.50904  3.1702  0.004432 ** 
    ## NT.indicator:post.policy.indicator  8.24608    2.91236  2.8314  0.009716 ** 
    ## ---
    ## Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

Again, for a proper write up of the results and methods here, as well as
additional results, please refer to the paper in the same repository.
