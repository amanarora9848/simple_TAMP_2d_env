import matplotlib.pyplot as plt

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
    elif int(w[2:]) == 5 :
        plt.plot(float(nodes[w][0]), float(nodes[w][1]), "r.", markersize=15)
    else:
        plt.plot(float(nodes[w][0]), float(nodes[w][1]),"k.")

# Add node connections
with open("graph.txt", "r") as f:
    for line in f:
        waypoints = line[:-1].split(",")
        for i in range(1, len(waypoints)):
            plt.plot([nodes[waypoints[0]][0], nodes[waypoints[i]][0]], [nodes[waypoints[0]][1], nodes[waypoints[i]][1]], "m", linewidth=0.5)

# Show path
if showPath:
    with open("path.txt", "r") as f:
        w1 = f.readline().strip()
        for line in f:
            w2 = line.strip()
            plt.plot([nodes[w1][0], nodes[w2][0]], [nodes[w1][1], nodes[w2][1]], "lime", linewidth=2)
            w1 = w2
            
# Show expected path
if showExpectedPath:
    with open("expected_path.txt", "r") as f:
        w1 = f.readline().strip()
        for line in f:
            w2 = line.strip()
            plt.plot([nodes[w1][0], nodes[w2][0]], [nodes[w1][1], nodes[w2][1]], "darkorange", linewidth=1.5)
            w1 = w2

plt.show()