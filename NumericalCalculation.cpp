#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Graph {
    public:     
       int n_nodes;

        Graph(int n){
            set_nodes(n);
        };

        void set_nodes(int n){
            // Update the number of nodes, and resize the adj matrix to be N x N. 
            n_nodes = n;
            adjacency_matrix.resize(n_nodes);

            for (int i = 0; i < n_nodes; ++i){
                adjacency_matrix[i].resize(n_nodes);
            }

        };

        void add_connection(int A, int B, int X){
            adjacency_matrix[A][B] = X;
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
            for (vector<int> i : adjacency_matrix){
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

    private:
        vector<vector<int> > adjacency_matrix;   // Describes the connections of the respective nodes. If adj[i][j] = 'xxx', then nodes i and j are connected by means of a 'xxx' element.


};

/*
class Circuit {
    private:
        int n_nodes;
        Graph magnitude_matrix;
        Graph type_matrix;
    
    public:

        Circuit(int n, Graph mag = Graph(0), Graph typ = Graph(0)){
            Graph x(n);
        };
             

}; 
*/

int main() {

    Graph teste(5);
    Graph Tesste = 0;
    
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

