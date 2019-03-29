#!/bin/bash

export PS1="(hack-services) $PS1"

PREFIX_PATH=/home/fabian/checkout/hack-services/prefix
export PATH=$PREFIX_PATH:$PATH
export LD_LIBRARY_PATH=$PREFIX_PATH/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$PREFIX_PATH/lib/x86_64-linux-gnu/pkgconfig:$PKG_CONFIG_PATH
export GI_TYPELIB_PATH=$PREFIX_PATH/lib/x86_64-linux-gnu/girepository-1.0/:$GI_TYPELIB_PATH
