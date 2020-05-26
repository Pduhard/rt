#!/usr/bin/env bash
demos=(
  'shapes'
  'lights'
  'textures'
  'cutting'
  'others'
)

to_exec=()

shapes_demo_dir="conf/shapes/"
lights_demo_dir="conf/lights/"
textures_demo_dir="conf/textures/"
cutting_demo_dir="conf/cutting/"
others_demo_dir="conf/others/"

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
  if [ $exec == "lights" ]
  then
    ./rt $lights_demo_dir/*
  fi
  if [ $exec == "textures" ]
  then
    ./rt $textures_demo_dir/*
  fi
  if [ $exec == "cutting" ]
  then
    ./rt $cutting_demo_dir/*
  fi
  if [ $exec == "others" ]
  then
    ./rt $others_demo_dir/*
  fi
  # echo "$exec"
done
