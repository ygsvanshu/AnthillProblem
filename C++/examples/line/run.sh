#!/bin/bash
rm -rf log.txt
../../build/runcalc --restart --history --speed=10.0 --timesteps=1000 --printout=1 2>&1 | tee -a log.txt
