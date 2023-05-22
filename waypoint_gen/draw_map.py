import matplotlib.pyplot as plt


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
        plt.plot(float(nodes[w][0]), float(nodes[w][1]), "b.")
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

plt.show()