#!/usr/bin/env bash
rt_path="./rt"

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

if [ ${#to_exec[@]} -eq 0 ]
then
  echo "Usage: ./demo.sh CATEGORY ..."
  echo "Possible values: shape light texture cutting negative ray_effect other"
  exit 1
fi

for exec in ${to_exec[@]}
do
  if [ $exec == "shape" ] || [ $exec == "all" ]
  then
    echo "Running Shape demo"
    $rt_path $shape_demo_dir/*
  fi
  if [ $exec == "light" ] || [ $exec == "all" ]
  then
    echo "Running Light demo"
    $rt_path $light_demo_dir/*
  fi
  if [ $exec == "texture" ] || [ $exec == "all" ]
  then
    echo "Running Texture demo"
    $rt_path $texture_demo_dir/*
  fi
  if [ $exec == "cutting" ] || [ $exec == "all" ]
  then
    echo "Running Cutting demo"
    $rt_path $cutting_demo_dir/*
  fi
  if [ $exec == "negative" ] || [ $exec == "all" ]
  then
    echo "Running Negative demo"
    $rt_path $negative_demo_dir/*
  fi
  if [ $exec == "ray_effect" ] || [ $exec == "all" ]
  then
    echo "Running Ray effect demo"
    $rt_path $ray_effect_demo_dir/*
  fi
  if [ $exec == "other" ] || [ $exec == "all" ]
  then
    echo "Running Other demo"
    $rt_path $other_demo_dir/*
  fi
done
