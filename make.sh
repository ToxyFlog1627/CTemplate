#!/bin/sh
docker container run --rm -v $PWD:/app -it gcc $@