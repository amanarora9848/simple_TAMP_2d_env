#!/bin/bash

display_help() {
    echo "Usage: ./script.sh [DOM_FILE] [PROB_FILE] [-n|--no-extmod] [-b|--build]"
    echo
    echo "Run the script with specified domain and problem PDDL files."
    echo
    echo "Arguments:"
    echo "  DOM_FILE     path to the domain PDDL file"
    echo "  PROB_FILE    path to the problem PDDL file"
    echo "  -n, --no-extmod     Run the planner without using the external module"
    echo "  -b, --build         Build the module before running"
    echo "  -w, --gen-waypoints Generate waypoints"
    exit 1
}

USE_EXTMOD=true
BUILD=false
GEN_WAYPOINTS=false

# Check for flags
for arg in "$@"
do
    case $arg in
        -h|--help)
            display_help
            ;;
        -n|--no-extmod)
            USE_EXTMOD=false
            shift # Remove --no-extmod from processing
            ;;
        -b|--build)
            BUILD=true
            shift # Remove --build from processing
            ;;
        -w|--gen-waypoints)
            GEN_WAYPOINTS=true
            shift # Remove --gen-waypoints from processing
            ;;
        *)
            OTHER_ARGUMENTS+=("$1")
            shift # Move to next argument
            ;;
    esac
done

# Check if not enough arguments are provided
if [[ ${#OTHER_ARGUMENTS[@]} -ne 2 ]]
then
    echo "Error: Not enough arguments"
    display_help
fi

DOMAIN_DIR=$(pwd)
DOM_FILE=${OTHER_ARGUMENTS[0]}
PROB_FILE=${OTHER_ARGUMENTS[1]}

# Run build instructions if the build flag is set
if $BUILD; then
    cd ../visits_module/src/
    ./buildInstruction.txt
    cd $DOMAIN_DIR
fi

cd ../waypoint_gen
rm path.txt

if $GEN_WAYPOINTS; then
    python3 waypoints_gen.py
fi

cd $DOMAIN_DIR

# Final command, using extmod if not disabled
if $USE_EXTMOD; then
    ../popf3-clp -E -x $DOM_FILE $PROB_FILE ../visits_module/build/libVisits.so region_poses.txt > ../waypoint_gen/output.txt
else
    ../popf3-clp $DOM_FILE $PROB_FILE > ../waypoint_gen/output.txt
fi

cd ../waypoint_gen
python3 draw_map.py
cd $DOMAIN_DIR
