//Michaela Isaacs

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <math.h>
using namespace std;

class Node
{
public:
    int value;
    int numZerosAfter = 0; //used for keeping track of zeros in sparse matrix
    int col;
    int row;
    Node* next = NULL;
};

class Matrix {
public:
    int size;
    Node* head;
    Matrix();
};

Matrix::Matrix() {
    size=0;
    head = NULL;
}

int findValue(int row,int col, Matrix &matrix) { //find value at specific position in matrix
    Node* currentNode=matrix.head;
    while (currentNode != NULL){ //until the end of the matrix

        if (currentNode->col == col && currentNode->row == row) { //if this is the node we want
            return currentNode->value;

        }
        else {
            currentNode = currentNode->next;
        }
    }
    return 0; //if we don't find the node we want, it must be a 0 that was ommitted in creating the sparse matrix
}

void addNode(Matrix &matrix, int input, int colLocated, int rowLocated) { //creating matrix
    Node* userNode = new Node;
    userNode->value = input;
    Node* currentNode=matrix.head;

    if (matrix.head == NULL) { //if there's not yet anything in the matrix
        matrix.head = userNode;
        userNode->col=colLocated; //assigning indices to each node
        userNode->row=rowLocated;
    }
    else {
        while (currentNode->next != NULL) { //until the end of the list
            currentNode=currentNode->next;
        }
        if (input == 0) { //if it's a zero, don't create a node, but note it down in the existing node's information
            currentNode->numZerosAfter++;
        }
        else { //otherwise, add the node to the list
            currentNode->next=userNode;
            userNode->col=colLocated;
            userNode->row=rowLocated;
        }

    }
}

Matrix minorMatrix(Matrix &matrix, int row, int col) { //creating a minor of a matrix
    Matrix newMatrix;
    Node* currentNode = matrix.head;
    int currentCol=0;
    int currentRow=0;
    bool zeroEntry = false; //creates a condition to avoid skipping over appropriate values
    int sizeAdjusted = sqrt((matrix.size-((2*sqrt(matrix.size))-1))); //a general formula for the size of a minor of a matrix

    while (currentNode != NULL && (currentNode->col != (sizeAdjusted-1) || currentNode->row != sizeAdjusted)) { //until the node before the list ends (to account for 0 being final value)
        if (zeroEntry) { //if a zero was found last time around
            zeroEntry=false;
            addNode(newMatrix,currentNode->value,currentCol,currentRow); //place the appropriate value directly after the 0
            if (currentCol < sizeAdjusted-1) { //if col is not max of the minor matrix
                currentCol++;
            }
            else { //if it is max of the minor matrix, hop down a row
                currentCol=0;
                if (currentRow < sizeAdjusted-1) {
                    currentRow++;
                }
            }
        }
        if (currentNode->next->row!=row && currentNode->next->col != col) { //if the next node is valid for our minor
            if (currentNode->numZerosAfter > 0 && currentNode->col != sizeAdjusted) { //if the next valid option is a 0, but that 0 isn't on the next row
                currentNode=currentNode->next;
                zeroEntry = true;
            }
            else { //otherwise add node to the new matrix at indicated position
                addNode(newMatrix,currentNode->next->value,currentCol,currentRow);
                currentNode = currentNode->next;
            }


            if (currentCol < sizeAdjusted-1) { //if col is not max of minor matrix, move over a col
                currentCol++;
            }
            else { //if it is the max of the minor matrix, hop down a row
                currentCol=0;
                if (currentRow < sizeAdjusted-1) {
                    currentRow++;
                }
            }
        }
        else {
            currentNode = currentNode->next;
        }
    }
    if (currentNode->numZerosAfter > 0) { //if final appropriate value is a 0
        zeroEntry=true;
    }
    else { //otherwise, make this node the last in our minor
        addNode(newMatrix,currentNode->next->value,currentCol,currentRow);
    }
    newMatrix.size=(matrix.size-((2 * sqrt(matrix.size))-1));
    return newMatrix;
}

int det(Matrix matrix) { //calculates determinant
    int determinant=0;
    if (matrix.head==NULL) { // base case: if no matrix was created, it was all 0s
        return 0;
    }
    else if (matrix.size==4) { //base case: if we can just take the cross product

        int leftTop = findValue(0,0,matrix);
        int leftBottom = findValue(0,1,matrix);
        int rightTop = findValue(1,0,matrix);
        int rightBottom = findValue(1,1,matrix);

        return ((leftTop*rightBottom)-(rightTop*leftBottom));
    }
    else { //otherwise, recursive case
        for (int i = 0; i < (sqrt(matrix.size)); i++) {
            determinant += pow(-1,i) * findValue(i,0,matrix) * det(minorMatrix(matrix,i,0));
        }
    }
    return determinant;
}

int main()
{
    Matrix sparse;
    string line;
    int num;
    vector<int> numPerLine;
    bool squareMatrix;

    while(getline(cin,line)) { //take each line of input matrix
        stringstream ss(line);
        stringstream keepInCheck(line); //stops stream from progressing before i want it to
        int numEachLine = 0;
        while (ss >> num) { //while there's an integer there
            keepInCheck >> num;
            numEachLine++; //keeps track of columns of matrix
            addNode(sparse,num,(numEachLine-1),(sparse.size));
        }
        numPerLine.push_back(numEachLine); //stores columns for later use
        sparse.size++; //keeps track of rows of matrix to determine if square
    }

    sparse.size = pow(sparse.size,2); //how many individual values in this matrix

    for (int i = 0; i < numPerLine.size(); i++) {
        if (numPerLine[i] != sqrt(sparse.size)) { //if cols != rows
            squareMatrix = false;
        }
        else {
            squareMatrix = true;
        }
    }

    if (!squareMatrix) { //if the matrix is not square at all
        cout << "Error! Non-square matrix!";
    }
    else { //get the determinant
        cout << det(sparse);
    }

}