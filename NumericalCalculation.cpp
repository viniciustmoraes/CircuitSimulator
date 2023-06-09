#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <cstdlib>
#include <utility>
using namespace std;

class Graph
{
public:
    int n_nodes;

    Graph(int n)
    {
        set_nodes(n);
    };

    Graph() : n_nodes(0){};

    ~Graph(){
        // Empty destructor
    };

    void set_nodes(int n)
    {
        // Update the number of nodes, and resize the adj matrix to be N x N.
        n_nodes = n;
        adj.resize(n_nodes);

        for (int i = 0; i < n_nodes; ++i)
        {
            adj[i].resize(n_nodes);
        }
    };

    void add_edge(int A, int B, float X)
    {
        adj[A][B] = X;
    };

    void add_edge_sym(int A, int B, float X)
    {
        add_edge(A, B, X);
        add_edge(B, A, X);
    };

    void set_diagonal(float X)
    {
        // Set diagonal values to the input X
        for (int i = 0; i < n_nodes; i++)
        {
            add_edge(i, i, X);
        };
    };

    void set_matrix_values(float X)
    {
        for (int i = 0; i < n_nodes; i++)
        {
            for (int j = 0; j < n_nodes; j++)
            {
                add_edge(i, j, X);
            };
        };
    };

    void print_adj_matrix()
    {
        for (vector<float> i : adj)
        {
            for (float j : i)
            {
                if (j == 0)
                {
                    cout << "-";
                }
                else
                {
                    cout << j;
                }

                cout << " ";
            };
            cout << endl;
        };
    };

    string adj_matrix_to_list()
    {
        int k = 1;
        string adj_list = "[";
        for (vector<float> i : adj)
        {
            adj_list.append("[");

            int p = 1;

            for (float j : i)
            {
                adj_list.append(to_string(j));

                if (p != n_nodes)
                {
                    adj_list.append(",");
                }

                p++;
            };

            adj_list.append("]");

            if (k != n_nodes)
            {
                adj_list.append(",");
            }

            k++;
        };

        adj_list.append("]");

        return adj_list;
    }

    vector<vector<float>> adj; // Adjacency Matrix. Describes the connections of the respective nodes
};

class Circuit
{
public:
    Graph magnitude_matrix; // magnitude_matrix[i][j] encodes the magnitude of the element connecting nodes i and j
    Graph type_matrix;      // type_matrix[i][j] encodes the type of the element connecting nodes i and j
    int ground_node;        // Index of the node conected to ground.

    /*

    The following convention is used for the element types:


    0 - Default value (No connection between nodes)
    1 - Wire | Short-circuit
    2 - Resistor
    3 - Capacitor
    4 - Inductor
    5 - Voltage source
    6 - Current source

    */

    int n_nodes;

    Circuit(int n, Graph mag = Graph(), Graph typ = Graph())
    {
        /* Starts the circuit. If no mag and typ adj matrices are passed, then empty n by n matrices are initialized
        Basic consistency verifications are performed, verifying the matrices sizes in case arguments are passed.
        */
        n_nodes = n;

        if (n_nodes <= 0)
        {
            throw invalid_argument("Number of nodes must be bigger than 0");
        }

        if (mag.n_nodes == 0 & typ.n_nodes == 0)
        {
            mag.set_nodes(n_nodes);
            typ.set_nodes(n_nodes);
        }

        else if (mag.n_nodes != typ.n_nodes)
        {
            throw invalid_argument("Adjacency matrices do not share the same size");
        }
        else if (n_nodes != mag.n_nodes)
        {
            throw invalid_argument("Adjacency matrices size does not correspond to number of nodes passed");
        }

        magnitude_matrix = mag;
        type_matrix = typ;
        ground_node = 0;
    };

    ~Circuit(){
        // Empty destructor
    };

    void set_ground(int n)
    {
        ground_node = n;
    };

    void resize(int n)
    {
        n_nodes = n;
        magnitude_matrix.set_nodes(n_nodes);
        type_matrix.set_nodes(n_nodes);
    };

    bool verify()
    {
        /*
        Verify the integrity of the circuit:
        - Adjacency matrices have the same size, equal to n_nodes.
        - Every filled element of the first adjacency matrix corresponds to a filled element of the second adjacency matrix
        and vice versa

        -> Returns true if the Circuit passes the test, false otherwise
        */

        bool state = true;

        // First condition
        if (magnitude_matrix.n_nodes != type_matrix.n_nodes || magnitude_matrix.n_nodes != n_nodes)
        {
            state = false;
        }

        // Second condition
        else
        {

            for (int i = 0; i < n_nodes; i++)
            {
                for (int j = 0; j < n_nodes; j++)
                {
                    if (magnitude_matrix.adj[i][j] != 0 & type_matrix.adj[i][j] == 0)
                    {
                        state = false;
                    }
                    else if (type_matrix.adj[i][j] != 0 & magnitude_matrix.adj[i][j] == 0)
                    {
                        state = false;
                    }
                };
            };
        }

        return state;
    };

    void simplify(){

    };

    void circuit_to_figure()
    {

        string command = "python3 visualize.py --mag " + magnitude_matrix.adj_matrix_to_list() + " --type " + type_matrix.adj_matrix_to_list();

        // Execute the Python script with arguments
        int returnCode = system(command.c_str());

        // Check the return code for any errors
        if (returnCode == -1)
        {
            throw runtime_error("Error while executing python script for circuit representation");
        }
    };

    void add_connection(int A, int B, char element_type, float magnitude)
    {
        // Adds a simple connection to nodes A and B, of type and magnitude specified.

        // Define mapping dictionary. Requires c++ 11 or greater. Use flag -std=c++17
        map<char, int> type_map{
            {'w', 1}, // Wire | Short-circuit
            {'r', 2}, // Resistor
            {'c', 3}, // Capacitor
            {'l', 4}, // Inductor
            {'v', 5}, // Voltage source
            {'i', 6}  // Current source
        };

        // Check if the connection type is valid
        if (type_map.count(element_type) == 0)
        {
            throw std::invalid_argument("Invalid connection type");
        };

        if (max(A, B) > n_nodes - 1)
        {
            throw std::invalid_argument("One of the chosen nodes does not exist");
        };

        int type_code = type_map[element_type];

        // Add connection to the magnitude and type matrices.

        int X;

        // No previous connections exist between A and B
        if (type_matrix.adj[A][B] == 0)
        {
            X = B;
        }

        else // If a connection exists, then perform a parallel connection addition
        {
            // Adds a new dummy node at the end of the list, and shorts it with B. The desired connection is performed between this node and A
            resize(n_nodes + 1);
            X = n_nodes - 1;

            add_connection(B, X, 'w', 0);
        }

        // Voltage and current sources are considered separately.
        // For passive elements, the matrices are filled symmetrically
        if (element_type != 'v' & element_type != 'i')
        {
            magnitude_matrix.add_edge_sym(A, X, magnitude);
            type_matrix.add_edge_sym(A, X, type_code);
        }

        // For active elements, a negative sign is added to the magnitude of connection X -> A
        else
        {
            magnitude_matrix.add_edge(A, X, magnitude);
            type_matrix.add_edge(A, X, type_code);

            magnitude_matrix.add_edge(X, A, -magnitude);
            type_matrix.add_edge(X, A, type_code);
        }
    };
};

vector<vector<float>> adj;

pair<vector<vector<float>>, vector<vector<float>>> nodal(Circuit circuit)
{
    /*
    Nodal Analysis Implementation

    Takes as an input the circuit, and outputs matrices A and B of the linear system Av = B, where v is the vector formed by the
    voltage variables.
    */
    vector<vector<float>> matrixA;
    vector<vector<float>> matrixB;

    circuit.ground_node;
    for (int i = 0; i < circuit.n_nodes; i++)
    {
        for (int j = 0; j < circuit.n_nodes; j++)
        {
            circuit.type_matrix.adj[i][j];
        };

        i++;
    };

    return make_pair(matrixA, matrixB);
}

int main()
{

    // ================= CIRCUIT INITIALIZATION ===================

    Circuit Test = Circuit(4);

    Test.add_connection(0, 2, 'r', 10);
    Test.add_connection(2, 3, 'r', 4);
    Test.add_connection(1, 2, 'r', 13);
    Test.add_connection(1, 3, 'w', 1111);
    Test.add_connection(3, 0, 'v', 1);

    // ======================== PRINTING ==========================

    cout << endl
         << "Magnitude Matrix" << endl;

    Test.magnitude_matrix.print_adj_matrix();
    cout << endl
         << endl
         << "Type Matrix" << endl;

    Test.type_matrix.print_adj_matrix();
    cout << endl
         << endl;

    Test.circuit_to_figure();

    return 0;
};
