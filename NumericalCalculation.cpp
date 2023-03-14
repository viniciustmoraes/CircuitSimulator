#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
using namespace std;

class Graph {
    public:     
       int n_nodes;

        Graph(int n){
            set_nodes(n);
        };

        Graph() : n_nodes(0) {};

        ~Graph() {
            // Empty destructor
        };

        void set_nodes(int n){
            // Update the number of nodes, and resize the adj matrix to be N x N. 
            n_nodes = n;
            adj.resize(n_nodes);

            for (int i = 0; i < n_nodes; ++i){
                adj[i].resize(n_nodes);
            }

        };

        void add_edge(int A, int B, float X){
            adj[A][B] = X;
        };

        void add_edge_sym(int A, int B, float X){
            add_edge(A,B,X);
            add_edge(B,A,X);
        };

        void set_diagonal(float X){
            // Set diagonal values to the input X
            for (int i = 0; i < n_nodes; i++){
                add_edge(i,i,X);
            };
        };

        void set_matrix_values(float X){
            for (int i = 0; i < n_nodes; i++){
                for (int j = 0; j < n_nodes; j++){
                    add_edge(i,j,X);
                };
            };
        };

        void print_adj_matrix(){
            for (vector<float> i : adj){
                for (float j : i){
                    if (j == 0)
                    { cout << "-";}
                    else 
                    {cout << j;}  

                    cout << " "; 
                    };
                cout << endl;
                };
            };

        vector<vector<float> > adj;   // Adjacency Matrix. Describes the connections of the respective nodes


};


class Circuit {
    public:
        Graph magnitude_matrix; // magnitude_matrix[i][j] encodes the magnitude of the element connecting nodes i and j
        Graph type_matrix;      // type_matrix[i][j] encodes the type of the element connecting nodes i and j
        
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

        Circuit(int n, Graph mag = Graph(), Graph typ = Graph()){
            n_nodes = n;

            if (n_nodes <= 0){ throw invalid_argument("Number of nodes must be bigger than 0"); }  
            
            if (mag.n_nodes == 0 & typ.n_nodes == 0 ) { mag.set_nodes(n_nodes); typ.set_nodes(n_nodes); }

            else if (mag.n_nodes != typ.n_nodes){ throw invalid_argument("Adjacency matrices do not share the same size"); }
            else if (n_nodes != mag.n_nodes) { throw invalid_argument("Adjacency matrices size does not correspond to number of nodes passed"); }

            magnitude_matrix = mag;
            type_matrix = typ;

            };

        ~Circuit() {
            // Empty destructor
        };

        void resize(int n){
            n_nodes = n;
            magnitude_matrix.set_nodes(n_nodes);
            type_matrix.set_nodes(n_nodes);
        };

        bool verify(){
            /* 
            Verify the integrity of the circuit:
            - Adjacency matrices have the same size, equal to n_nodes.
            - Every filled element of the first adjacency matrix corresponds to a filled element of the second adjacency matrix
            and vice versa
            
            -> Returns true if the Circuit passes the test, false otherwise
            */

            bool state = true;
            
            //First condition
            if (magnitude_matrix.n_nodes != type_matrix.n_nodes || magnitude_matrix.n_nodes != n_nodes){ state = false; }

            //Second condition
            else{

            for (int i = 0 ; i < n_nodes ; i++ ){
                for (int j = 0 ; j < n_nodes ; j++){
                    if (magnitude_matrix.adj[i][j] != 0 & type_matrix.adj[i][j] == 0){ state = false; }
                    else if (type_matrix.adj[i][j] != 0 & magnitude_matrix.adj[i][j] == 0){ state = false; }
                    };
                };
            
            }

            return state;
        };

        void simplify(){

        };            

        void add_connection(int A, int B, char element_type, float magnitude){
            // Adds a simple connection to nodes A and B, of type and magnitude specified.
            
            // Define mapping dictionary
            map<char, int> type_map {
                {'w', 1},  // Wire | Short-circuit
                {'r', 2},  // Resistor
                {'c', 3},  // Capacitor
                {'l', 4},  // Inductor
                {'v', 5},  // Voltage source
                {'i', 6}   // Current source
            };

            // Check if the connection type is valid
            if (type_map.count(element_type) == 0) {
                throw std::invalid_argument("Invalid connection type");
            }

            int type_code = type_map[element_type];
            
            // Add connection to the magnitude and type matrices. 

            // Voltage and current sources are considered separately. 
            // For passive elements, the matrices are filled symmetrically
            if (element_type != 'v' & element_type != 'i'){
                magnitude_matrix.add_edge_sym(A, B, magnitude);
                type_matrix.add_edge_sym(A, B, type_code);
            }
            
            // For active elements, a negative sign is added to the magnitude of connection B -> A
            else{
                magnitude_matrix.add_edge(A, B, magnitude);
                type_matrix.add_edge(A, B, type_code);

                magnitude_matrix.add_edge(B, A, -magnitude);
                type_matrix.add_edge(B, A, type_code);
            }
        
        };


}; 


int main() {

    /*
    Circuit Test = Circuit(5);

    cout << endl << "The current size of the circuit is: " << Test.n_nodes << endl << endl;
    cout << "The magnitude matrix of the circuit is: " << endl;
    Test.magnitude_matrix.print_adj_matrix();
    
    cout << endl;

    cout << "The type matrix of the circuit is: " << endl;
    Test.type_matrix.print_adj_matrix();

    cout << endl;
    
    */

   cout << endl;

    Graph magnitudes = Graph(3);
    Graph types = Graph(4);

    magnitudes.set_matrix_values(0);
    magnitudes.add_edge(1,2,5);
    magnitudes.set_diagonal(1);

    types.set_matrix_values(1);

    magnitudes.print_adj_matrix();
    cout << endl << endl;
    types.print_adj_matrix();
    cout << endl << endl;

    cout << "=============" << endl;

    Circuit Test = Circuit(2,magnitudes);

    cout << endl;

    Test.magnitude_matrix.print_adj_matrix();
    cout << endl << endl;

    Test.type_matrix.print_adj_matrix();
    cout << endl << endl;
    

   return 0;
}

