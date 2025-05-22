#!/bin/bash

export LD_LIBRARY=.
export LD_PRELOAD=libft_malloc.so
$@
