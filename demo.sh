#!/usr/bin/env bash
demos=(
  'shapes'
)

to_exec=()

shapes_demo_dir="conf/shapes/"

for arg in $*
do
  for opt in ${demos[@]}
  do
    if [ $arg == $opt ]
    then
      check="1"
      for dup in ${to_exec[@]}
      do
        if [ $dup == $opt ]
        then
          check="0"
        fi
      done
      if [ $check == "1" ]
      then
        to_exec=("${to_exec[@]}" "${opt}")
      fi
    fi
  done
done

for exec in ${to_exec[@]}
do
  if [ $exec == "shapes" ]
  then
    ./rt $shapes_demo_dir/*
  fi
  # echo "$exec"
done
