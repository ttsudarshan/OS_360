#include <iostream>
#include <pthread.h>
#include <vector>

using namespace std;


vector<int> fiboseq;


struct thread {
    int num;
};

void* generatefibo(void* arg) {
    thread* data = (thread*)arg;
    int n = data->num;


    fiboseq.resize(n);

    if (n > 0) fiboseq[0] = 0;
    if (n > 1) fiboseq[1] = 1; 

    for (int i = 2; i < n; ++i) {
        fiboseq[i] = fiboseq[i - 1] + fiboseq[i - 2];
    }

    pthread_exit(0);
}

int main() {
    pthread_t tid; 
    thread data;
    
    cout << "How many numbers to generate? ";
    cin >> data.num;

    if (data.num <= 0) {
        cout << "Number must be +ve" << endl;
        return 1;
    }

    
    pthread_create(&tid, nullptr, generatefibo, &data);

    
    pthread_join(tid, nullptr);

    
    cout << "Fibonacci sequence: ";
    for (int i = 0; i < data.num; ++i) {
        cout << fiboseq[i] << " ";
    }
    cout << endl;

    return 0;
}
