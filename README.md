# Lorentz Renderer
A 3D Lorentz Tranform Visualiser based on GLT OpenGL toolkit 

This is the final project of "Trattamento Numerico dei Dati Sperimentali" 2019/2020 at Università degli Studi di Milano for my degree in BSc degree in Physics

## The problem

This program falls into the category of visualizing tools with an educational purpose and wants to adress the famous statement from Special Relativity that "Time dilate and lenght contracts".
This statemens implies some weird object stretching that most of the time gets addressed just as computing how a unlicky stick in the middle of space gets twisted and is never fully visualized on a complex object.

As the short and not-in-english README.pdf guide written for my supervisor explains this problem actually consists of two superimposed issues: the problem of light phisically reaching you (seeing) and the problem of describing this process in two distinct inertial frames of reference (being).

It is funny to learn how actually the first of the two mechanism is the one responsible for the major twisting, while the second, which is the genuine relativistic contribution, is quite boring.
Some months ago (Nov 2023) I stumbled accross an interesting youtube video that addresses just that: https://www.youtube.com/watch?v=l44JSW-1RvE


## Compiling

This program mainly depends on a quite old, and unmantained github project containing an OpenGL library, if you really wish to embark in the compilation journey continue reading otherwise skip to "Running without compiling"

### Dependencies

#### GLT
    Version: commit 3e5db25 13 Nov 2016
    https://github.com/nigels-com/glt

#### Boost
    Version 1.70.0
    Libraries : Regex, System, Thread
    https://boostorg.jfrog.io/artifactory/main/release/1.70.0/source/


### Makefile

The repo is equipped with a high quality makefile (hopefully, since it was part of the coursework).
Provided that all the required libraries are installed one only needs to make a small modification to two variables in "makefile".

    win.GLTROOT = path-to-glt-folder

    BOOSTROOT = path-to-boost-folder

The executable is then obtained by launching the command

    make win

(even if you are not on windows hehe)    


## Running without compiling

Disregarding any possible software shipping standar & safety measure the executable is part of the repo and is equipped with all the dll required to run it without compiling.
You have my word that everything is safe :) 

This of course is OS limited and only works on Windows.
It has been tested on Windows 10/11


The executable might be sometimes unstable and I'm working to resolve this issue.


##

### Have fun!
