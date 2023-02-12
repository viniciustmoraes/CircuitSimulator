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
            adjacency_matrix.resize(n_nodes, vector<string>(n_nodes));
        };

        void add_connection(int A, int B, string X){
            adjacency_matrix[A][B] = X;
        };

        void add_connection_sym(int A, int B, string X){
            add_connection(A,B,X);
            add_connection(B,A,X);
        };

        void set_diagonal(string X){
            // Set diagonal values to the input X
            for (int i = 0; i < n_nodes; i++){
                add_connection(i,i,X);
            };
        };

        void set_matrix_values(string X){
            for (int i = 0; i < n_nodes; i++){
                for (int j = 0; j < n_nodes; j++){
                    add_connection(i,j,X);
                };
            };
        };

        void print_adj_matrix(){
            for (vector<string> i : adjacency_matrix){
                for (string j : i){
                    if (j == "")
                    { cout << "-";}
                    else 
                    {cout << j;}  

                    cout << " "; 
                    };
                cout << endl;
                };
            };

    private:
        vector<vector<string> > adjacency_matrix;   // Describes the connections of the respective nodes. If adj[i][j] = 'xxx', then nodes i and j are connected by means of a 'xxx' element.


};

/*
class Circuit : public Graph {
    public:
        int n_nodes;
        vector<vector<string> > circuit_matrix;

        Circuit(int n){
            Graph(n);
        };
             

}; 
*/


int main() {

    Graph teste(5);
    
    teste.set_matrix_values("0");

    teste.add_connection(1,2,"vv");
    teste.add_connection(3,4,"seg");
    teste.set_diagonal("a");

    teste.print_adj_matrix();
    

   return 0;
}