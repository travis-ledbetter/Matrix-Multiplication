#include <pthread.h>
#include <time.h>
#include <iostream>
#include <string>
using namespace std;

// Function prototypes
void *multiply(void *arg);
void menu();
void createCustom(int);
void threading(int);

// Define constant row and column sizes
int const R1 = 4, C1 = 4, R2 = 4, C2 = 4;

// Define and initialize default matrices
int matA[R1][C1] = {
    {1,1,1,1},
    {2,2,2,2},
    {3,3,3,3},
    {4,4,4,4}
};
int matB[R2][C2] = {
    {1,1,1,1},
    {2,2,2,2},
    {3,3,3,3},
    {4,4,4,4}
};

// Define result matrix and user generated matrices
int *resultMat = NULL;
int userMatA[R1][C1], userMatB[R1][C1];

// Char array used to add constant spaces in output
char formatter[] = "    ";

// Entry point
int main() {
    do {
        menu();
    } while(true);

    return 0;
}

/*
*   Function: createCustom()
*   Purpose: this function accepts an int to be used as a flag. Depending on the status
*   of the flag, it either fills userMatA and userMatB with user input or with random
*   ints
*/
void createCustom(int flag) {
    if(!flag) {
        cout << "\nInput Matrix A:";
        for(int i = 0; i < R1; i++) {
            for(int j = 0; j < C1; j++) {
                cout << "\nEnter number at (" << i << "," << j << "): ";
                cin >> userMatA[i][j];
                while(cin.fail()) {
                    cout << "\nInvalid input" << endl;
                    cout << "\nEnter number at (" << i << "," << j << "): ";
                    cin >> userMatA[i][j];
                }
            }
        }
        cout << "\nInput Matrix B:";
        for(int i = 0; i < R1; i++) {
            for(int j = 0; j < C1; j++) {
                cout << "\nEnter number at (" << i << "," << j << "): ";
                cin >> userMatB[i][j];
                while(cin.fail()) {
                    cout << "\nInvalid input" << endl;
                    cout << "\nEnter number at (" << i << "," << j << "): ";
                    cin >> userMatB[i][j];
                }
            }
        }
    } else {
        srand(time(0));
        for(int i = 0; i < R1; i++) {
            for(int j = 0; j < C1; j++) {
                userMatA[i][j] = rand() % 20 + 1;
            }
        }
        for(int i = 0; i < R1; i++) {
            for(int j = 0; j < C1; j++) {
                userMatB[i][j] = rand() % 20 + 1;
            }
        }
    }
    threading(1);
}

/*
*   Function: menu()
*   Purpose: this function creates a menu allowing the user to select which type
*   of matrix they would like to use. Options are default, custom, and random
*/
void menu() {
    int selection = 0;

    cout << "\n\nMatrix Multiplication" << endl;
    cout << formatter << "1. Use Default Matrices" << endl;
    cout << formatter << "2. Enter Custom Matrices" << endl;
    cout << formatter << "3. Use Random Number Matrices" << endl;
    cout << formatter << "4. Exit" << endl;
    cout << "Selection: ";
    cin >> selection;
    while(cin.fail() || selection < 1 || selection > 4) {
        cin.clear();
        cin.ignore();
        cout << "\nInvalid selection" << endl;
        cout << "\nSelection: ";
        cin >> selection;
    }

    switch(selection) {
        case 1:
            threading(0);
            break;
        case 2:
            createCustom(0);
            break;
        case 3:
            createCustom(1);
            break;
        case 4:
            cout << "\nQuitting. Goodbye!" << endl;
            exit(0);
    }
}

/*
*   Function: multiply()
*   Purpose: this function uses each thread to multiply a single element
*/
void *multiply(void *arg) {
    int *data = (int *)arg;
    int k = 0, i = 0;

    int x = data[0];
    for(i = 1; i <= x; i++) {
        k += data[i] * data[i+x];
    }

    int *p = (int *)malloc(sizeof(int));
    *p = k;

    pthread_exit(p);
}
/*
*   Function: threading()
*   Purpose: this function creates and then joins 16 threads to be used in the matrix multiplication
*/
void threading(int flag) {
    pthread_t *threads;
    threads = (pthread_t*)malloc(4*sizeof(pthread_t));

    int count = 0;
    cout << "\nMatrix A:" << endl;
    for(int i = 0; i < R1; i++) {
        cout << formatter;
        for(int j = 0; j < C1; j++) {
            if(!flag) {
                if(to_string(matA[i][j]).length() < 2)
                    cout << matA[i][j] << formatter << " ";
                else
                    cout << matA[i][j] << formatter;
            } else {
                if(to_string(userMatA[i][j]).length() < 2)
                    cout << userMatA[i][j] << formatter << " ";
                else
                    cout << userMatA[i][j] << formatter;
            }
        }
        cout << endl;
    }

    cout << "\nMatrix B:" << endl;
    for(int i = 0; i < R1; i++) {
        cout << formatter;
        for(int j = 0; j < C1; j++) {
            if(!flag) {
                if(to_string(matB[i][j]).length() < 2)
                    cout << matB[i][j] << formatter << " ";
                else
                    cout << matB[i][j] << formatter;
            } else {
                if(to_string(userMatB[i][j]).length() < 2)
                    cout << userMatB[i][j] << formatter << " ";
                else
                    cout << userMatB[i][j] << formatter;
            }
        }
        cout << endl;
    }

    for(int i = 0; i < R1; i++) {
        for(int j = 0; j < C2; j++) {
            resultMat = (int *)malloc((20)*sizeof(int));
            resultMat[0] = C1;
            for(int k = 0; k < C1; k++) {
                if(!flag) {
                    resultMat[k+1] = matA[i][k];
                } else {
                    resultMat[k+1] = userMatA[i][k];
                }
            }
            for(int k = 0; k < R2; k++) {
                if(!flag) {
                    resultMat[k+C1+1] = matB[k][j];
                } else {
                    resultMat[k+C1+1] = userMatB[k][j];
                }
            }
            pthread_create(&threads[count++], NULL, multiply,(int*)(resultMat));
        }
    }

    cout << "\nMatrix A * Matrix B:" << endl;
    
    for(int i = 0; i < R1 * C2; i++) {
        cout << formatter;
        void *k;
        pthread_join(threads[i],&k);
        int *p = (int *)k;
        cout << *p;
        if((i+1) % C2 == 0) {
            cout << endl;
        }
        
    }
}