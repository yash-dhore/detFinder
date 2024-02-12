#include <iostream>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <fstream>

using namespace std;

void clearTerminal() {
    cout << "\033[2J\033[1;1H";
}

size_t getDigits(long double value) {
    string s = to_string(value);

    s.erase(s.find_last_not_of('0') + 1, string::npos);

    if (!s.empty() && !std::isdigit(s.back()))
        s.pop_back();
    
    return s.length();
}

void printMatrix(vector<vector<long double>> matrix) {
    size_t maxLen = 0;
    for (vector<long double> row: matrix) {
        for (long double val : row) {
            if (getDigits(val) > maxLen)
                maxLen = getDigits(val);
        }
    }

    for (size_t i = 0; i < matrix.size(); ++i) {
        cout << "| ";
        for (size_t j = 0; j < matrix.at(i).size(); ++j) {
            cout.width(maxLen);
            cout << matrix.at(i).at(j);
            if (j != matrix.at(i).size() - 1)
                cout << " ";
        }
        cout << " |" << endl;
    }
}

long double det2x2(vector<vector<long double>> matrix2x2) {
    return (matrix2x2.at(0).at(0) * matrix2x2.at(1).at(1)) - (matrix2x2.at(0).at(1) * matrix2x2.at(1).at(0)); // ad - bc
}

// efficiency can be increased by performing cofactor expansion along row/column with most 0s
long double det(vector<vector<long double>> matrix) {
    if (matrix.size() == 2)
        return det2x2(matrix);

    long double determinant = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        if (matrix.at(0).at(i) == 0)
            continue;

        vector<vector<long double>> downsize(matrix.size() - 1, vector<long double> (matrix.size() - 1, 0));

        for (size_t j = 1; j < matrix.size(); ++j) {
            for (size_t k = 0; k < matrix.at(j).size(); ++k) {
                if (k != i) {
                    unsigned short int downsizeColInd = k < i ? k : k - 1;
                    downsize.at(j-1).at(downsizeColInd) = matrix.at(j).at(k);
                }
            }
        }

        determinant += (1.0 - (2.0 * (i % 2))) * matrix.at(0).at(i) * det(downsize);
    }

    return determinant;
}

int main() {
    clearTerminal();

    bool fileInput = false;
    string filename;
    ifstream infile;

    cout << "Enter filename to get input from ('X' for manual input): ";
    getline(cin, filename);
    cout << endl;

    string filePath = "input/" + filename;

    if (!(filename == "X" || filename == "x")) {
        infile.open(filePath);

        if (!infile.is_open())
            cout << "File \"" << filename << "\" not found (is it in the input folder?). Switching to manual input." << endl << endl;
        else
            fileInput = true;
    }

    unsigned short int numRows;
    unsigned short int numCols;
    long double determinant; // long double to "brute force" reducing/getting rid of floating errors

    bool matrixIsSquare = false;
    while (!matrixIsSquare) {
        cout << "Please enter the number of rows: ";
        if (fileInput)
            infile >> numRows;
        else
            cin >> numRows;
        
        cout << "Please enter the number of columns: ";
        if (fileInput)
            infile >> numCols;
        else
            cin >> numCols;

        cout << endl;

        clearTerminal();

        if (numRows != numCols) {
            cout << "The matrix is not square, thus it does not have a determinant." << endl;
            continue;
        }
        else if (numRows <= 0) {
            cout << "The number of rows and columns cannot be " << numRows << " (must be 1 or more)." << endl;
            continue;
        }

        matrixIsSquare = true;
    }

    vector<vector<long double>> matrix(numRows, vector<long double> (numCols, 0));

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.at(i).size(); ++j) {
            cout << "Enter the matrix:" << endl << endl;
            printMatrix(matrix);

            cout << endl << "Enter value of the matrix at (" << i+1 << ", " << j+1 << "): ";
            long double val;

            if (fileInput)
                infile >> val;
            else
                cin >> val;

            matrix.at(i).at(j) = val;

            clearTerminal();
        }
    }

    if (fileInput)
        infile.close();

    printMatrix(matrix);

    auto start_time = chrono::high_resolution_clock::now();

    if (numRows == 1)
        determinant = matrix.at(0).at(0);
    else if (numRows == 2)
        determinant = det2x2(matrix);
    else {
        determinant = det(matrix);
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    auto time = end_time - start_time;
    
    cout << endl << "The determinant of the provided matrix is " << fixed << determinant << "." << endl;

    if (time/chrono::nanoseconds(1) < 100000)
        cout << "This calculation took " << time/chrono::nanoseconds(1) << " nanoseconds to run." << endl << endl;
    else if (time/chrono::nanoseconds(1) < 100000000){
        double milliseconds = time/chrono::nanoseconds(1) / double(1000000);
        cout << "This calculation took " << milliseconds << " milliseconds to run." << endl << endl;
    }
    else {
        double seconds = time/chrono::nanoseconds(1) / double(1000000000);
        cout << "This calculation took " << seconds << " seconds to run." << endl << endl;
    }
}