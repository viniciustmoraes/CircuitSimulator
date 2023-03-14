#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class Graph {
    public:     
       int n_nodes;

        Graph(int n){
            set_nodes(n);
        };

        Graph() : n_nodes(0) {};

        void set_nodes(int n){
            // Update the number of nodes, and resize the adj matrix to be N x N. 
            n_nodes = n;
            adj.resize(n_nodes);

            for (int i = 0; i < n_nodes; ++i){
                adj[i].resize(n_nodes);
            }

        };

        void add_connection(int A, int B, int X){
            adj[A][B] = X;
        };

        void add_connection_sym(int A, int B, int X){
            add_connection(A,B,X);
            add_connection(B,A,X);
        };

        void set_diagonal(int X){
            // Set diagonal values to the input X
            for (int i = 0; i < n_nodes; i++){
                add_connection(i,i,X);
            };
        };

        void set_matrix_values(int X){
            for (int i = 0; i < n_nodes; i++){
                for (int j = 0; j < n_nodes; j++){
                    add_connection(i,j,X);
                };
            };
        };

        void print_adj_matrix(){
            for (vector<int> i : adj){
                for (int j : i){
                    if (j == 0)
                    { cout << "-";}
                    else 
                    {cout << j;}  

                    cout << " "; 
                    };
                cout << endl;
                };
            };

        vector<vector<int> > adj;   // Adjacency Matrix. Describes the connections of the respective nodes


};


class Circuit {
    private:
        Graph magnitude_matrix; // magnitude_matrix[i][j] encodes the magnitude of the element connecting nodes i and j
        Graph type_matrix;      // type_matrix[i][j] encodes the type of the element connecting nodes i and j
        
        /*

        The following convention is used for the element types:


        0 - Default value (No connection between nodes)
        1 - Wire / Short-circuit
        2 - Resistor
        3 - Capacitor
        4 - Inductor
        5 - Voltage source
        6 - Current source
        
        */
    
    public:
        int n_nodes;
        
        Circuit(int n, Graph mag = Graph(0), Graph typ = Graph(0)){
            n_nodes = n;
            int n_m = mag.n_nodes;
            int n_t = typ.n_nodes;

            if (n_m != n_t){ throw invalid_argument("Adjacency matrices do not share the same size"); }
            else if (n_nodes <= 0){ throw invalid_argument("Number of nodes must be bigger than 0"); }
            else if (n_m == 0){ mag.set_nodes(n_nodes); typ.set_nodes(n_nodes); }
            
            magnitude_matrix = mag;
            type_matrix = typ;
            
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

}; 


int main() {

    Graph teste(5);
    
    teste.set_matrix_values(0);

    //teste.add_connection(1,2, 10);
   // teste.add_connection(3,4,20);
    teste.set_diagonal(45);

    teste.print_adj_matrix();
    cout << endl << "===========" << endl << endl;

    teste.set_nodes(3);
    
    teste.print_adj_matrix();
    

   return 0;
}

