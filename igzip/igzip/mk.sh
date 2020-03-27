#!/bin/bash
make lib0c
mkdir -p /opt/igzip
\cp  ./libigzip0c.a  /opt/igzip/
\cp  ./c_code/internal_state_size.h /opt/igzip
\cp  ./c_code/igzip_lib.h /opt/igzip
\cp  ../include/types.h /opt/igzip



exit
make lib1c
mkdir -p /opt/igzip
\cp  ./libigzip1c.a  /opt/igzip/
\cp  ./c_code/internal_state_size.h /opt/igzip
\cp  ./c_code/igzip_lib.h /opt/igzip
\cp  ../include/types.h /opt/igzip

