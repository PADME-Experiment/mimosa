#!/bin/bash
sed \
  -e $'s/fafafafa/\e[1;34m&\e[0m/' \
  -e $'s/800[0123]800[0123]/\e[1;31m&\e[0m/' \
  -e $'s/80088007/\e[1;32m&\e[0m/' \
  -e $'s/aaa8aaa7/\e[0;32m&\e[0m/' \
  -e $'s/8bb[0123]8bb[0123]/\e[0;31m&\e[0m/' \
  -e $'s/abcdabcd/\e[0;34m&\e[0m/' \
  < $1

