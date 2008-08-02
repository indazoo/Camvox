#!/bin/sh

python test.py >test.pov ; povray +w800 +h600 +A +P +Itest.pov

