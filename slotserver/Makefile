# Copyright (c) 2020 Hugh Wade
# SPDX-License-Identifier: MIT

# .PHONY defines parts of the makefile that are not dependant on any specific file
# This is most often used to store functions
.PHONY: all test clean lint coverage
.DEFAULT_GOAL: all

all: lint test

test:
	pytest -vv  --cov=.

coverage:
	pytest -vv  --cov-report xml:cov.xml --cov=.

lint:
	flake8

clean:
	rm -rf ./lib ./lib64 ./include ./bin ./share pyvenv.cfg
