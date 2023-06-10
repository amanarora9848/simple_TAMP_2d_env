# Simple Task and Motion Planning

This project is the 2nd assignment of AI for Robotics 2 course at the University of Genoa, Italy.

### Team:
- [@amanarora9848](https://github.com/amanarora9848)
- [@davideCaligola](https://github.com/davideCaligola)
- [@Lucas-Pardo](https://github.com/Lucas-Pardo)
- [@pablomoreno555](https://github.com/pablomoreno555)

The goal of this project is to implement a planning domain for a robot in a region with 4 "collection" desks and 1 "submission" desk. After sampling random waypoints in the environment, the robot needs to collect 2 assignments and submit them to the submission desk while minimizing the motion cost, i.e. the length of the covered path by the robot. For this assignment we have used the popf-tif planner found [here](https://github.com/popftif/popf-tif).

More details are included in the attached report.

## Getting Started

1. Make sure the popf-tif planner is properly installed and working. For more information, please refer to the [popf-tif repository](https://github.com/popftif/popf-tif).

2. Clone the repository
    ```bash
    git clone https://github.com/amanarora9848/simple_TAMP_2d_env.git
    ```

3. The planner executable has been left in the main folder of the repository. 

4. A shell script was created which builds the external module (when required, as specified), runs this planner, generates new waypoints (code for which is located in the `waypoint_gen` directory) and displays the map which shows the path (both the expected and the planned path) the robot follows. An option can be specified to run the planner with or without using the external module, as defined in the directory `visits_module`. The script is located in `visits_domain` the usage of which can be seen by running the following:

    ```bash
    cd visits_domain
    chmod +x run_tamp_dom.sh
    ./run_tamp_dom.sh -h
    ```

5. A typical way to execute the planner is as follows:

    ```bash
    ./run_tamp_dom.sh dom1.pddl prob1.pddl -b -w
    ```

    This generates the planner output in the file `waypoint_gen/output.txt`, prints the motion costs on the terminal, and finally displays a map which shows the path the robot follows.

6. Note that to keep generating new set of waypoints everytime, it is necessary to set the variable `keep_waypoints` to `False` and `random_waypoints` to `True` in the file `waypoint_gen/waypoints_gen.py`
