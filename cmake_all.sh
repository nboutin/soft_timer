#!/usr/bin/env bash
# Usage:
# (Bash)$ ./cmake_all.sh
# ./cmake_all.sh --no-clean

# On error, exit immediately
set -e

script_title="CMake all"
log_filename="cmake_all"

# echo -e option enables interpretation of backslash escapes
title() {
	echo -e "# $1"
}

header1() {
	echo -e "\n## $1"
}

header2() {
	echo -e "\n### $1"
}

info() {
	echo -e "$1"
}

error() {
	RED='\033[0;41;30m'
	STD='\033[0;0;39m'
	echo -e "ERROR: ${RED}$1${STD}\n"
	exit 1
}

# ---
# Parse command line arguments
while getopts c option
do
    case "${option}"
    in
        c) no_clean=1;;
    esac
done

main() {
	title "${script_title}"

    if [ -z ${no_clean+x} ]; then
        header1 "Delete build folder"
        rm -rf build
    else
        header1 "Skip delete build folder"
    fi

    header1 "Build GCC ARM Debug"
    cmake --preset gcc_arm_debug
    cmake --build --preset gcc_arm_debug

    header1 "Build GCC ARM MinSizeRel"
    cmake --preset gcc_arm_minsizerel
    cmake --build --preset gcc_arm_minsizerel

    header1 "Build GCC Release"
    cmake --preset gcc_release
    cmake --build --preset gcc_release

    header1 "Build GCC test"
    cmake --preset gcc_test
    cmake --build --preset gcc_test

    header1 "Run test"
    ctest --preset gcc_test
    cmake --build --preset gcc_test --target test_report
    cmake --build --preset gcc_test --target test_coverage

	header1 "Execution time"
}

#--- Main

mkdir -p report
(
	time main
	header1 "${script_title}: done"
) 2>&1 | tee report/"${log_filename}".md # redirect stdout/stderr to a file
