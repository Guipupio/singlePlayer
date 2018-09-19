#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

 
std::vector<float> rola(std::vector<float> teste){
	teste.push_back(100);

	std::vector<float> v = teste;
	return v;
}

int main()
{
    // Create a vector containing integers
    std::vector<float> v(5);
    std::vector<float> velocidade(2);
 
    // Add two more integers to vector
    v.push_back(25);
    v.push_back(13);
 
    // Iterate and print values of vector
    for(int n : v) {
        std::cout << n << '\n';
    }

    std::cout << rola(velocidade)[1] << '\n';
}

