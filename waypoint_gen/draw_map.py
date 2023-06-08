import matplotlib.pyplot as plt
from file_read_backwards import FileReadBackwards

showPath = True    # Show path contained in "path.txt"
showExpectedPath = True    # Show path contained in "expected_path.txt"

nodes = {}
with open("waypoints.txt", "r") as f:
    for line in f:
        w = line[0:4]
        pos = list(map(float, line[5:-2].split(",")))
        nodes[w] = pos

# print(nodes)


# R1
plt.plot([-3, -2], [2, 2], "c")
plt.plot([-2, -2], [3, 2], "c")
# R2
plt.plot([3, 2], [2, 2], "c")
plt.plot([2, 2], [3, 2], "c")
# R3
plt.plot([-3, -2], [-2, -2], "c")
plt.plot([-2, -2], [-3, -2], "c")
# R4
plt.plot([3, 2], [-2, -2], "c")
plt.plot([2, 2], [-3, -2], "c")
plt.xlim(-3, 3)
plt.ylim(-3, 3)

# Plot nodes:
for w in nodes:
    if int(w[2:]) == 0:
        plt.plot(float(nodes[w][0]), float(nodes[w][1]), "g.", markersize=15)
    elif int(w[2:]) < 5:
        plt.plot(float(nodes[w][0]), float(nodes[w][1]), "b.", markersize=15)
    elif int(w[2:]) == 5:
        plt.plot(float(nodes[w][0]), float(nodes[w][1]), "r.", markersize=15)
    else:
        plt.plot(float(nodes[w][0]), float(nodes[w][1]), "k.")

# Add node connections
graph = {}
with open("graph.txt", "r") as f:
    for line in f:
        waypoints = line[:-1].split(",")
        graph[waypoints[0]] = waypoints[1:]
        for i in range(1, len(waypoints)):
            plt.plot([nodes[waypoints[0]][0], nodes[waypoints[i]][0]], [
                     nodes[waypoints[0]][1], nodes[waypoints[i]][1]], "m", linewidth=0.5)


# Show expected path
if showExpectedPath:
    with open("expected_path.txt", "r") as f:
        x = []
        y = []
        for line in f:
            w = line.strip()
            x.append(nodes[w][0])
            y.append(nodes[w][1])
        plt.plot(x, y, "black", linewidth=5, label="Expected Path")


# Show path
if showPath:
    path = []
    planner_cost = 0
    with FileReadBackwards("output.txt") as f:
        for line in f:
            aux = line.split()
            if aux[1] != "(goto_region":
                if aux[1] == "Cost:":
                    planner_cost = float(aux[2])
                    break
                continue
            path.append(aux[4][:-1])

    path.reverse()
    print("Planner Cost: ", planner_cost)

    regions = ["wp00", "wp01", "wp02", "wp03", "wp04", "wp05"]
    with open("path.txt", "r") as f:
        w1 = f.readline().strip()
        i = 0
        draw = [w1]
        aux = [w1]
        w1 = f.readline().strip()
        aux.append(w1)
        for line in f:
            w2 = line.strip()
            if i == len(path):
                break
            aux.append(w2)
            w1 = w2
            if w1 in regions:
                if w1 == f"wp0{path[i][1]}" and (draw[-1] == aux[0] or aux[0] in graph[draw[-1]]):
                    draw += aux
                    i += 1
                aux = []
                w1 = f.readline().strip()
                aux.append(w1)

        x = []
        y = []
        for i in range(len(draw)):
            x.append(nodes[draw[i]][0])
            y.append(nodes[draw[i]][1])

        plt.plot(x, y, "cyan", linewidth=3,
                 linestyle="--", label="Planned Path")


plt.legend(loc="upper center", bbox_to_anchor=(0.5, 0.5, 0, 0.65))
plt.show()
