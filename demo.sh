#!/usr/bin/env bash
demos=(
  'shape'
  'light'
  'texture'
  'cutting'
  'other'
  'negative'
  'ray_effect'
  'all'
)

to_exec=()

shape_demo_dir="conf/shapes/"
light_demo_dir="conf/light/"
texture_demo_dir="conf/text/"
cutting_demo_dir="conf/cutting/"
other_demo_dir="conf/others/"
negative_demo_dir="conf/negative/"
ray_effect_demo_dir="conf/ray_effect/"
# all_dir=""

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
        if [ $opt == "all" ]
        then
          to_exec=("${opt}")
          break 2
        else
          to_exec=("${to_exec[@]}" "${opt}")
        fi
      fi
    fi
  done
done

for exec in ${to_exec[@]}
do
  if [ $exec == "shape" ] || [ $exec == "all" ]
  then
    echo "Running Shape demo"
    ./rt $shape_demo_dir/*
  fi
  if [ $exec == "light" ] || [ $exec == "all" ]
  then
    echo "Running Light demo"
    ./rt $light_demo_dir/*
  fi
  if [ $exec == "texture" ] || [ $exec == "all" ]
  then
    echo "Running Texture demo"
    ./rt $texture_demo_dir/*
  fi
  if [ $exec == "cutting" ] || [ $exec == "all" ]
  then
    echo "Running Cutting demo"
    ./rt $cutting_demo_dir/*
  fi
  if [ $exec == "other" ] || [ $exec == "all" ]
  then
    echo "Running Other demo"
    ./rt $other_demo_dir/*
  fi
  if [ $exec == "negative" ] || [ $exec == "all" ]
  then
    echo "Running Negative demo"
    ./rt $negative_demo_dir/*
  fi
  if [ $exec == "ray effect" ] || [ $exec == "all" ]
  then
    echo "Running Ray effect demo"
    ./rt $ray_effect_demo_dir/*
  fi
  # echo "$exec"
done
