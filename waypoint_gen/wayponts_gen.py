import numpy as np

k = 4
n = 24      # Can be a maximum of 2 digits (<100) otherwise parsing breaks
keep_waypoints = True

nodes = {
        "wp00": [0, 0, 0],
        "wp01": [-2.5, 2.5, 0],
        "wp02": [2.5, 2.5, 0],
        "wp03": [-2.5, -2.5, 0],
        "wp04": [2.5, -2.5, 0],
        "wp05": [3, 0, 0]
        }

# Generate n random waypoints
if keep_waypoints:
    with open("waypoints.txt", "r") as f:
        c = 0
        for line in f:
            if c > 5:
                nodes[line[0:4]] = list(map(float, line[5:-2].split(",")))
            c += 1
else:
    with open("waypoints.txt", "w+") as f:
        for node in nodes:
            f.write(f"{node}[{nodes[node][0]},{nodes[node][1]},{nodes[node][2]}]\n")
        for i in range(6, n + 7):
            pos = np.random.uniform(-3, 3, 3)
            while abs(pos[0]) >= 2 and abs(pos[1]) >= 2:
                pos = np.random.uniform(-3, 3, 3)
            pos[2] = 0
            if i < 10:
                nodes[f"wp0{i}"] = pos
                f.write(f"wp0{i}[{pos[0]},{pos[1]},{pos[2]}]\n")
            else:
                nodes[f"wp{i}"] = pos
                f.write(f"wp{i}[{pos[0]},{pos[1]},{pos[2]}]\n")
    
        
def dist(x, y):
    return np.sqrt((x[0] - y[0])**2 + (x[1] - y[1])**2)

      
# Generate the graph by connecting nodes to their k-nearest neighbours
keys = [i for i in nodes.keys()]
connections = {} 
for i in range(len(keys)):
    min_dists = []
    for j in range(len(keys)):
        if i == j:
            continue
        min_dists.append((keys[j], dist(nodes[keys[i]], nodes[keys[j]])))
        min_dists.sort(key=lambda x: x[1])
        if len(min_dists) > k:
            min_dists.pop()
    connections[keys[i]] = []
    for w in min_dists:
        connections[keys[i]].append(w[0])
        

# Write connections to a file        
with open("graph.txt", "w+") as f:
    for node in connections:
        msg = node
        for w in connections[node]:
            msg += "," + w
        f.write(msg + "\n")
            