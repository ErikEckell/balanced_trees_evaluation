#include "/home/edauandes/eda_cpp/trees_tarea3/rb_tree/include/trees/rb.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>

int* readKeysFromFile(std::string filename, int* n_keys){
    std::ifstream  fin(filename, std::ios::binary); 
    char* val = new char[4];
    int n = 0;
    fin.read(val, 4);
    while (!fin.eof()){
        n = n + 1;        
        fin.read(val, 4);
    }
    fin.close();
    fin.open(filename, std::ios::binary); 
    int* keys = new int[n];
    for(int i=0; i < n; i++){
        fin.read(val, 4);
        keys[i] = *reinterpret_cast<int*>(val);
    }
    fin.close();
    *n_keys = n;
    delete[] val;
    return keys;
}

int main(int nargs, char** vargs){
    int n_data = 0;
	int n_queries = 0;
    int* data = readKeysFromFile("/home/edauandes/eda_cpp/data_trees/keys_1048576.bin", &n_data);
	int* queries = readKeysFromFile("/home/edauandes/eda_cpp/data_trees/queries_1000.bin", &n_queries);

	auto inicio = std::chrono::high_resolution_clock::now();
    trees::RB rbtree;
    for(int i=0; i<n_data;i++){
        rbtree.insert(data[i]);
    } 

	auto fin = std::chrono::high_resolution_clock::now();
	auto duracion = std::chrono::duration_cast<std::chrono::nanoseconds>(fin - inicio).count();
	std::cout << "Tiempo de construcción: " << duracion << " nanosegundos" << std::endl;

	auto inicio2 = std::chrono::high_resolution_clock::now();
    for(int i=0; i<1000;i++){
        rbtree.find(queries[i]);
    } 
	auto fin2 = std::chrono::high_resolution_clock::now();
	auto duracion2 = std::chrono::duration_cast<std::chrono::nanoseconds>(fin2 - inicio2).count();
	std::cout << "Tiempo de búsqueda: " << duracion2 << " nanosegundos" << std::endl;
	
    return 0;
}