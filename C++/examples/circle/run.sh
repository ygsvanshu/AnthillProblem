#!/bin/bash
rm -rf log.txt
../../build/runcalc --restart --timesteps=200 --printout=1 2>&1 | tee -a log.txt