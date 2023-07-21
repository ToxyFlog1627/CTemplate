#!/bin/sh
docker container run --rm -v $PWD:/app -e DEBUG=$DEBUG -e TEST=$TEST -it gcc $@