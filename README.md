# YCrossFit
In this project we built a shared library named YCrossFit, for the CMEPDA exam. 

![docs](https://github.com/zenith378/Y-DiMuonResonances/actions/workflows/doc.yml/badge.svg) [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Using the Y(1S,2S,3S) resonances in two muons from the CMS open data, the main goals of this library are essentially two:
- fitting them with customizable parameters;
-  plotting the differential cross section in $p_T$.

This functionalities are built in order to recreate similar plots of the article  [Measurements of the Υ(1S), Υ(2S), and Υ(3S) differential cross sections in pp collisions at √s = 7 TeV](https://arxiv.org/pdf/1501.07750.pdf) by the CMS collaborations, even if the data set are not the same.
# A simple example usage
The whole project is built with CMake, so the first thing we want to do is to create a build directory where the makefiles and executables are going to be. This is simply done by typing in the terminal: 
```
$ cmake -S . -B ./build

$ make -C build
```
## Default options (Cross mode)

If we run the main of the library without givin anything as flag or option, as it follows:
```
$ ./build/main/YLaunch
```
the program will create the plot of the differential cross section in pt of the Y resonances. The default applied filters are:

- an invariant mass of the Dimuon pair between 8.5 and 11.5 GeV
- at least 2 muons of opposite charge in each event
- an absolute value of rapidity less than 1.2

![Final Result](https://github.com/zenith378/Y-DiMuonResonances/blob/main/Plots/png/diffCrossSection/diffCrossSection-1.png)

  
## Fit mode
This programs also offers a Fit functionality where the differential cross section is not calculated nor plotted, but a singular fit is performed. This is called by adding the option --mode (-m) fit to the executable call.
```
$ ./build/main/YLaunch --mode fit
```
The following graph is produced, while the results of the fit are printed in the terminal. 
![Fit Mode](https://github.com/zenith378/Y-DiMuonResonances/blob/main/Plots/YResonancesFit-1.png)


### Custimize options

In Fit mode we can customize some cuts on the dimuon state using the flags described in the documentation of the file  `OptionParse.h`.

Say for example, that we want to apply the cuts $ 20.0 < p_T < 30.0 $ GeV and $|y| < 0.5$ for the dimuon state and save the plot in a file named CustomizeCuts.pdf.
We can type in the terminal

```
$ ./build/main/YLaunch -m fit -n “CustomizeCuts” -p 20. -P 30. -Y 0.5
```
and the following figure is produced and saved in the file CustomizeCut.pdf inside the Plots folder.

![CustomizeCuts](https://github.com/zenith378/Y-DiMuonResonances/blob/main/Plots/png/CustomizeCuts/CustomizeCuts-1.png)


# Description of main (and functionality of libraries)

In order to use the library as the author thought, a main is provided with it. In this section, the structure of the program is explained as if we were reading the code starting from the main and following the function calls.
### Choice of the parameters and flags
The first thing one wants to do is to initialize some parameters. This step is necessary because the options and flags are passed as pointers in the function `ProcessArgs()`, so that they can be modified by the user with the flag syntax while calling the executable from terminal.
The default values are:

- depth = 0, i.e. basic cuts on two muons with opposite charge and invariant mass between 8.5 and 11.5 GeV
- fitFunction = 0, i.e. Breit-Wigner
- ptm, ptM, ym, yM = NaN, i.e. no cuts on $p_T$ or rapidity $y$
- nameFile = "YResonacesFit", name of the figure that will be saved
- verbose = 0, i.e. no output stream from Minuit
- mode = 1, i.e. CrossMode

 In the `optionParse.C` file are also defined functions in order to handle parameters out of bounds and non-existing ones. 

### Data Load

The data are loaded and manipulated (to have useful variables) using the functions defined inside  `df_set.C`. 
Since the data are heavy, it is really slow to read them online every time one calls the program. Therefore, it was decided to download them and store them in a folder Data. 
>Note: The folder Data is not in this repository because is too heavy. The first time you call this function, the data are going to be downloaded from the CMS Open Data portal, so it might take a while (up to 40 minutes depending on the internet connection).

The Dataset is saved through a Snapshot of the Dataframe with new useful variables:

- Dimuon_FourVec: Four Vector containing the Pt, Eta, Phi and Mass of the Dimuon pair
- Dimuon_mass: Invariant Mass of the Dimuon Four Vector
- Dimuon_pt: Transverse Momentum of the Dimuon Four Vector
- Dimuon_beta: Beta (velocity) of the Dimuon Four Vector
- Dimuon_y: Rapidity of the Dimuon Four Vector

This variables are defined in order to make fast and efficient cuts on the Dataframe.

In the function`df_set()` is also handled the creation of the folder Data if not already existing.

### Apply some cuts!
In the file `Cuts.C` one can decide the cuts to be applied in the dataframe.
The authors implemented three "default" ones:
- Depth 0: only two muons with opposite charge and with an invariant mass between 8.5 and 11.5 GeV are selected
- Depth 1: same as depth 0 but with an additional cut on the transverse momentum, requesting $10\lt p_T \lt 100$ GeV
- Depth 2: same as depth 1 but with an addition cut on rapidity, requesting $|y|\lt 1.2$.
In order to be faster in the future calls, these three dataframe are saved in data_cut0.root, data_cut1.root, data_cut2.root.

It is also possible to choose other cuts on the trasverse momentum and the rapidity of the dimuon state, by adding an option while calling the program from the terminal.
For example, the line
```
$ ./build/main/YLaunch -m fit -p 12 -P 50 -y 0.2 -Y 1.4
```
requests the cuts to be $12\lt p_T \lt 50$ GeV and $0.2<|y|<1.4$
, this dataframe is not saved.
Once the dataframe is cut, the report on the cuts is printed on stream.

### Draw a preliminary Spectrum Plot

The selected data are preliminary drawn on a canvas as an histohgramm of the invariant mass of the dimuon. The canvas is saved as  `nameFile`__Preliminary.pdf_, where nameFile is the name passed through command line with the flag -n or --nameFile.
>If no argument is provided, the default value for nameFile is "YResonacesFit".

In the file `Cuts.C` is also defined a function in order to handle the saving of figures in the folder Plots. If it does not exist, it is recreated.

### Fit the Function and save it in a canvas

In `fitRoo.C` the plotted histogramm is fitted by RooFit to a PDF choosable with the flag -f [--fitFunction]. The options are:
- a BreitWigner [-f bw] (default)
- a Gaussian [-f gaus]
- a t-Student [-f stud] (still not stable in this version)

The histogram and the fitted function are drawn on a canvas which is saved as _`nameFile`.pdf_. If there are custom cuts, these are printed on the canvas.

### Calculate and plot the differential cross section

Using the customizable function  `fitRoo()`, it is possible calculate the differential cross section of the process $Y->\mu^+\mu^-$, defined as
$$\frac{d\sigma}{dp_T}=\frac{N}{L \Delta p_T e_{uu} e_{sg} e_{vp} A}$$
where $N$ is a fit parameter that says how many events are under the signal function and $\Delta p_t$ is the width of the bin in $p_T$ (i.e. the width of cut on the trasverse momentum). A more detailed description of the functions used it can be found in  `[diffCrossSection.h]`.


# Testing

Since the program is built with CMake, the testing was performed using CTest. Below are presented the tests that the authors implemented.
## Test 0
Testing if the reading of the command arguments and flags is properly done. 
In this test one define some variables, call the `processArgs()` and sees if the definition stands.
## Test 1
Here is tested the online reading of the data and the behavior in case the Data folder or the Data file is missing.
In the test, the folder Data is deleted and the function `df_set()` is called. This function should handle the creation of the folder Data and the downloading and saving of the data. 
Once finished this first step, we check if the Data folder exists and if it contains the file data.root
Next, we keep the folder Data and we eliminate the file data.root.
The function `df_set()` is called again and after it finished, one check if the data is successfully recreated.
## Test 2
In this test the fit results are controlled. First off, one defines a model with a similar shape to the one expected and calls the function `fitRoo()` passing this model as an argument instead of the real data. The test checks if the fit converged by looking at the fitStatus and also check if the returned parameters are inside 5 sigma of the initial value.
## Test 3
This one tests the function `SavePlot()` which handles the saving of a canvas with a specific filename. If the folder Plots does not exist, it creates it.
## Test 4
This test is useful to check if the printing of the custom cuts on the canvas work. It compares the strings returned by the function `formatYString()` or `formatPtString()` with the expected ones.
