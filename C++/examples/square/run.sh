#!/bin/bash
rm -rf log.txt
../../build/runcalc --restart --speed=10.0 --timesteps=100 --printout=1 2>&1 | tee -a log.txt