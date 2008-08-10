#!/bin/sh

python shaft.py >shaft.pov ; povray +w800 +h600 +A +P +Ishaft.pov

