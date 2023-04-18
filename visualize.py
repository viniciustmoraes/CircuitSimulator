import networkx as nx
import matplotlib.pyplot as plt
import numpy as np


def circuitplot(mag_matrix, type_matrix):
    '''
    Given the adjacency matrices of a circuit, plots the circuit in a matplotlib figure.

    The nodes are positioned according to the spring layout algorithm
    '''
    if len(mag_matrix) != len(type_matrix):
        raise Exception(
            "Magnitude and Type matrices do not have the same size")
    else:
        n_nodes = len(mag_matrix)

    # Creates an empty nx graph
    G = nx.Graph()

    # Adds the respective nodes and connections
    for i in range(n_nodes):
        G.add_node(i)

    connections_list = []

    for i in range(n_nodes):
        for j in range(n_nodes):
            if type_matrix[i][j] != 0:
                if ([i, j] not in connections_list) and ([j, i] not in connections_list):
                    connections_list.append([i, j])

                    G.add_edge(i, j)

    # Determines the nodes positions using the spring algorithm
    # pos is a dictionnary where the nodes are the keys to the respective positions, stored as a 2D array
    pos = nx.spring_layout(G)

    return G, pos


G, pos = circuitplot([[0, 1, 0], [1, 0, 0], [0, 0, 0]],
                     [[0, 4, 0], [4, 0, 0], [0, 0, 0]])


print(pos)

'''
fig, ax = plt.subplots()

# Draw nodes
nx.draw_networkx_nodes(G, pos, ax=ax, node_color='b', node_size=300)

# Draw edges
nx.draw_networkx_edges(G, pos, ax=ax, edge_color='k', width=2)

ax.text(0, 0, 'Text at (2, 25)', fontsize=12, color='red')


plt.show()

'''
