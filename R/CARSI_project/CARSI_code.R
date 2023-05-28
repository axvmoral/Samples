
#The code below was completed as part of my final project for the ECON 114 class at the University
#of California, Santa Cruz. Look for my official write up of the results computed here, along with
#additional results, in the pdf CARSI_write_up.pdf published in the same repository. Look for the
#markdown document carsi-homicide-markdown.md for a visual display of the results produced by this
#code.


library(tidyverse) # For data wrangling
library(reshape2) # For melting data
library(readxl) # For reading xlsx files
library(plotly) # For interactive maps
library(lmtest) # For robust standard errors
library(sandwich) # For robust standard errors


balance.test <- function(data_path, target, specification, title){
  #Wrangles datasets used for balance computation between the control and 
  #treatment groups, and returns either a ggplot object visually displaying
  #balance, or a difference-in-differences model to compute balance. 

  #:param data_path: a string indicating the path to a dataset
  #:param target: a string indicating the balance test target
  #:param specification: a string indicating wether to return a model or plot
  #:param title: a string indicating the title of the plot object
  #:return either a plot displaying the target balance, or a model to compute
  #        the target balance
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



#Below I run the balance.test function for the relevant balance targets,
#both for a plot and model.

setwd("C:/Users/axvmo/Desktop/Apllying/example code/data/") # This is where all the relevant datasets were stored
balance.test("Gross national income (GNI) per capita (constant 2017 PPP$).xlsx", "GNI per Capita", "plot", "Year Gross National Income (GNI) per Capita by Region")
balance.test("Gross national income (GNI) per capita (constant 2017 PPP$).xlsx", specification = "model")

balance.test("Mean years of schooling (years).xlsx", "Mean Years of Schooling", "plot", "Year Mean Years of Schooling by Region")
balance.test("Mean years of schooling (years).xlsx", specification = "model")

balance.test("Life expectancy at birth (years).xlsx", "Life Expectancy at Birth (Years)", "plot", "Year Life Expectancy at Birth by Region")
balance.test("Life expectancy at birth (years).xlsx", specification = "model")

balance.test("Human Development Index (HDI).xlsx", "HDI", "plot", "Year Human Development Index (HDI) by Region")
balance.test("Human Development Index (HDI).xlsx", specification = "model")



#Below I wrangle the homicide rate data in a similar fashion to the data used for 
#the balance targets. I then produce a similar plot.

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



#Below I create an interactive time-series choreopleath map of the 
#homicide rate, essentially relaying the same information as the 
#above static plot, but in an interactive geographic map format. 
#This is ommited in the markdown document.

# Prepare data for map
reps<-length(unique(rate.homicide.data$Year))
country.names <- c(rep("Nicaragua", reps),
                 rep("El Salvador", reps),
                 rep("Honduras", reps),
                 rep("Guatemala", reps))
NT.rate.data <- data.frame(subset(rate.homicide.data, name == "Northern Triangle"))
interactive.plot.data <- rbind(data.frame(subset(rate.homicide.data, name == "Nicaragua")),
                             NT.rate.data, NT.rate.data, NT.rate.data)
interactive.plot.data <- interactive.plot.data%>%
  mutate(plot.names = country.names,
         hover = paste0(name, "\n", round(rate, 2)))
fontStyle <- list(family = "DM Sans", size = 15, color = "black")
fontStyle1<-list(family = "DM Sans", size = 17, color = "white")
label<-list(bgcolor = "#EEEEEE", bordercolor = "transparent", font = fontStyle)


# Plot the interactive choreopleath map
plot_geo(interactive.plot.data, locationmode = "country names", frame = ~Year) %>%
  add_trace(type = "choropleth", locations = ~plot.names, z = ~rate,color = ~rate,
            zmin = min(interactive.plot.data$rate),
            zmax = max(interactive.plot.data$rate),
            colorscale = "RdBu",
            text = ~hover,
            hoverinfo = "text") %>%
  colorbar(y = 0.75, title = "Homicide Rate") %>%
  layout(geo = list(fitbounds = "locations", font = list(type = "DM Sans")), title = "Year Homicide Rate per 100,000 Persons by Region") %>%
  style(hoverlabel = label) %>%
  config(displayModeBar = FALSE) %>%
  animation_opts(easing = "elastic")



#Below I create a difference-in-differences model to compute the effect of CARSI on the 
#homicide rate.

# Create and compute the model
model.data<-subset(rate.homicide.data, Year>1998 & Year<2012)
model<-lm(rate ~ NT.indicator + post.policy.indicator + NT.indicator * post.policy.indicator, data = model.data)
robust.model <- coeftest(model, vcov = vcovHC(model, type="HC0"))
