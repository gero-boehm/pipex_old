#!/bin/bash
git add *
git commit -m "$(echo $@)"
git push