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

    edges = []

    for i in range(n_nodes):
        for j in range(n_nodes):
            if type_matrix[i][j] != 0:
                if ([i, j, type_matrix[i][j], mag_matrix[i][j]] not in edges) and ([j, i, type_matrix[i][j], mag_matrix[i][j]] not in edges):
                    edges.append([i, j, type_matrix[i][j], mag_matrix[i][j]])

                    G.add_edge(i, j)

    # Determines the nodes positions using the spring algorithm
    # pos is a dictionnary where the nodes are the keys to the respective positions, stored as a 2D array
    # For instance pos = {0: array([-0.37799656,  0.42002144]), 1: array([-0.23313452,  0.57997856]), 2: array([ 0.61113108, -1.])}
#   pos = nx.spring_layout(G)
    pos = nx.circular_layout(G)

    x, y = [], []

    for position in list(pos.values()):
        x.append(position[0])
        y.append(position[1])

    fig, ax = plt.subplots()

    for i in range(len(x)):
        ax.annotate(f"N{i}", (x[i]+0.015, y[i]+0.015))

    ax.scatter(x, y, c='k', marker='o', s=20, label='Nodes')

    # Encodes the integer element types in letters
    dict_elements = {0: '', 1: 'w', 2: 'R', 3: 'C', 4: 'L', 5: 'V', 6: 'I'}

    for connection in edges:
        start_node = connection[0]
        end_node = connection[1]
        element_type = dict_elements[connection[2]]
        magnitude = connection[3]

        # Plot black solid lines connecting the nodes with edges
        ax.plot([x[start_node], x[end_node]], [y[start_node],
                y[end_node]], 'k-', lw=2, label=element_type)

        # Calculate midpoint x-coordinate
        mid_x = (x[start_node] + x[end_node])*0.5
        # Calculate midpoint y-coordinate
        mid_y = (y[start_node] + y[end_node])*0.5

        if element_type != 'w':
            txt = f"{element_type} = {magnitude}"
            ax.annotate(txt, (mid_x+0.01, mid_y+0.01))
            print(f"Anotei o seguinte: {txt}")

    plt.axis('off')
    plt.title('Simulated Circuit')
    plt.show()

    return


# testing
mag = [[0, 0, 10, 0],
       [0, 0,  0, 0],
       [10, 0, 0, 4],
       [0, 0,  4, 0]]

type = [[0, 0, 2, 0],
        [0, 0, 0, 0],
        [2, 0, 0, 3],
        [0, 0, 3, 0]]

circuitplot(mag, type)


'''
fig, ax = plt.subplots()

# Draw nodes
nx.draw_networkx_nodes(G, pos, ax=ax, node_color='b', node_size=300)

# Draw edges
nx.draw_networkx_edges(G, pos, ax=ax, edge_color='k', width=2)

ax.text(0, 0, 'Text at (2, 25)', fontsize=12, color='red')


plt.show()

'''
