#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ELEMENTS 118
#define MAX_FORMULA 100

// Structure definitions
typedef struct {
    char symbol[5];
    int atomicNumber;
    float atomicWeight;
    char classification[20];
} Element;

typedef struct {
    char formula[MAX_FORMULA];
    Element elements[10];  // Increased from 5 to 10
    int elementCount[10];  // Increased from 5 to 10
    int numElements;
    double molecularWeight;
} Molecule;

// Stack for molecular weight calculations
typedef struct StackNode {
    Molecule molecule;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

typedef struct {
    int top;
    float weights[100];
} Stackelement;

// Queue for molecule processing
typedef struct QueueNode {
    Molecule molecule;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

// BST for organizing molecules by weight
typedef struct BSTNode {
    Molecule molecule;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

// Linked List for molecule storage
typedef struct ListNode {
    Molecule molecule;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
} LinkedList;

// Periodic Table (Corrected)
Element periodicTable[MAX_ELEMENTS] = {
    {"H", 1, 1.008, "nonmetal"},
    {"He", 2, 4.0026, "noble gas"},
    {"Li", 3, 6.94, "metal"}, 
    {"Be", 4, 9.0122, "metal"}, 
    {"B", 5, 10.81, "metalloid"}, 
    {"C", 6, 12.011, "nonmetal"}, 
    {"N", 7, 14.007, "nonmetal"}, 
    {"O", 8, 15.999, "nonmetal"}, 
    {"F", 9, 18.998, "nonmetal"}, 
    {"Ne", 10, 20.180, "noble gas"},
    {"Na", 11, 22.990, "metal"}, 
    {"Mg", 12, 24.305, "metal"}, 
    {"Al", 13, 26.982, "metal"}, 
    {"Si", 14, 28.085, "metalloid"}, 
    {"P", 15, 30.974, "nonmetal"}, 
    {"S", 16, 32.06, "nonmetal"}, 
    {"Cl", 17, 35.45, "nonmetal"}, 
    {"Ar", 18, 39.948, "noble gas"},
    {"K", 19, 39.098, "metal"}, 
    {"Ca", 20, 40.078, "metal"},
    {"Sc", 21, 44.956, "metal"}, 
    {"Ti", 22, 47.867, "metal"}, 
    {"V", 23, 50.942, "metal"}, 
    {"Cr", 24, 52.04, "metal"}, 
    {"Mn", 25, 54.938, "metal"}, 
    {"Fe", 26, 55.845, "metal"}, 
    {"Co", 27, 58.933, "metal"}, 
    {"Ni", 28, 58.693, "metal"},
    {"Cu", 29, 63.546, "metal"}, 
    {"Zn", 30, 65.38, "metal"},
    {"Ga", 31, 69.723, "metal"}, 
    {"Ge", 32, 72.63, "metalloid"}, 
    {"As", 33, 74.922, "metalloid"}, 
    {"Se", 34, 78.971, "nonmetal"}, 
    {"Br", 35, 79.904, "nonmetal"}, 
    {"Kr", 36, 83.798, "noble gas"},
    {"Rb", 37, 85.468, "metal"}, 
    {"Sr", 38, 87.62, "metal"}, 
    {"Y", 39, 88.906, "metal"}, 
    {"Zr", 40, 91.224, "metal"},
    {"Nb", 41, 92.906, "metal"}, 
    {"Mo", 42, 95.95, "metal"}, 
    {"Tc", 43, 98, "metal"}, 
    {"Ru", 44, 101.07, "metal"}, 
    {"Rh", 45, 102.91, "metal"}, 
    {"Pd", 46, 106.42, "metal"},
    {"Ag", 47, 107.87, "metal"}, 
    {"Cd", 48, 112.41, "metal"},
    {"In", 49, 114.82, "metal"}, 
    {"Sn", 50, 118.71, "metal"},
    {"Sb", 51, 121.76, "metalloid"}, 
    {"Te", 52, 127.60, "metalloid"}, 
    {"I", 53, 126.90, "nonmetal"}, 
    {"Xe", 54, 131.29, "noble gas"}, 
    {"Cs", 55, 132.91, "metal"}, 
    {"Ba", 56, 137.33, "metal"},
    {"La", 57, 138.91, "metal"}, 
    {"Ce", 58, 140.12, "metal"},
    {"Pr", 59, 140.91, "metal"}, 
    {"Nd", 60, 144.24, "metal"}, 
    {"Pm", 61, 145, "metal"}, 
    {"Sm", 62, 150.36, "metal"}, 
    {"Eu", 63, 152.00, "metal"}, 
    {"Gd", 64, 157.25, "metal"},
    {"Tb", 65, 158.93, "metal"}, 
    {"Dy", 66, 162.50, "metal"},
    {"Ho", 67, 164.93, "metal"}, 
    {"Er", 68, 167.26, "metal"},
    {"Tm", 69, 168.93, "metal"}, 
    {"Yb", 70, 173.04, "metal"}, 
    {"Lu", 71, 175.00, "metal"}, 
    {"Hf", 72, 178.49, "metal"},
    {"Ta", 73, 180.95, "metal"}, 
    {"W", 74, 183.84, "metal"}, 
    {"Re", 75, 186.21, "metal"}, 
    {"Os", 76, 190.23, "metal"},
    {"Ir", 77, 192.22, "metal"}, 
    {"Pt", 78, 195.08, "metal"},
    {"Au", 79, 196.97, "metal"}, 
    {"Hg", 80, 200.59, "metal"},
    {"Tl", 81, 204.38, "metal"}, 
    {"Pb", 82, 207.2, "metal"}, 
    {"Bi", 83, 208.98, "metal"}, 
    {"Po", 84, 209, "metalloid"},
    {"At", 85, 210, "nonmetal"}, 
    {"Rn", 86, 222, "noble gas"},
    {"Fr", 87, 223, "metal"}, 
    {"Ra", 88, 226, "metal"},
    {"Ac", 89, 227, "metal"}, 
    {"Th", 90, 232.04, "metal"},
    {"Pa", 91, 231.04, "metal"}, 
    {"U", 92, 238.03, "metal"}, 
    {"Np", 93, 237, "metal"}, 
    {"Pu", 94, 244, "metal"},
    {"Am", 95, 243, "metal"}, 
    {"Cm", 96, 247, "metal"},
    {"Bk", 97, 247, "metal"}, 
    {"Cf", 98, 251, "metal"},
    {"Es", 99, 252, "metal"}, 
    {"Fm", 100, 257, "metal"},
    {"Md", 101, 258, "metal"}, 
    {"No", 102, 259, "metal"},
    {"Lr", 103, 262, "metal"}, 
    {"Rf", 104, 267, "metal"},
    {"Db", 105, 270, "metal"}, 
    {"Sg", 106, 271, "metal"},
    {"Bh", 107, 270, "metal"}, 
    {"Hs", 108, 277, "metal"},
    {"Mt", 109, 276, "metal"}, 
    {"Ds", 110, 281, "metal"},
    {"Rg", 111, 280, "metal"}, 
    {"Cn", 112, 285, "metal"},
    {"Nh", 113, 284, "metal"}, 
    {"Fl", 114, 289, "metal"},
    {"Mc", 115, 288, "metal"}, 
    {"Lv", 116, 293, "metal"},
    {"Ts", 117, 294, "metal"}, 
    {"Og", 118, 294, "metal"}
};

int totalElements = 118;

// Function declarations
void searchElementByNumber(int number);
void searchElementBySymbol(char symbol[]);
void displayElementsByClassification(char classification[]);
void sortElementsByWeight(int order);
Molecule parseMolecularFormula(char* formula);
void displayMolecule(Molecule molecule);
const char* classifyAcidity(Molecule molecule);
const char* detectFunctionalGroups(Molecule molecule);

// Stack operations
void push(Stack* stack, Molecule molecule) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->molecule = molecule;
    newNode->next = stack->top;
    stack->top = newNode;
}

void pushelement(Stackelement *stack, float value) {
    if (stack->top < 99) {
        stack->weights[++stack->top] = value;
    } else {
        printf("Stack overflow!\n");
    }
}

Molecule pop(Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty!\n");
        Molecule emptyMolecule = {0};
        return emptyMolecule;
    }
    StackNode* temp = stack->top;
    Molecule molecule = temp->molecule;
    stack->top = stack->top->next;
    free(temp);
    return molecule;
}

float popelement(Stackelement *stack) {
    if (stack->top >= 0) {
        return stack->weights[stack->top--];
    } else {
        printf("Stack underflow!\n");
        return 0.0;
    }
}

// Queue operations
void enqueue(Queue* queue, Molecule molecule) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->molecule = molecule;
    newNode->next = NULL;
    
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}

Molecule dequeue(Queue* queue) {
    if (queue->front == NULL) {
        printf("Queue is empty!\n");
        Molecule emptyMolecule = {0};
        return emptyMolecule;
    }
    QueueNode* temp = queue->front;
    Molecule molecule = temp->molecule;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    return molecule;
}

// BST operations
BSTNode* createBSTNode(Molecule molecule) {
    BSTNode* newNode = (BSTNode*)malloc(sizeof(BSTNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newNode->molecule = molecule;
    newNode->left = newNode->right = NULL;
    return newNode;
}

BSTNode* insertBST(BSTNode* root, Molecule molecule) {
    if (root == NULL) {
        return createBSTNode(molecule);
    }
    
    if (molecule.molecularWeight < root->molecule.molecularWeight) {
        root->left = insertBST(root->left, molecule);
    } else {
        root->right = insertBST(root->right, molecule);
    }
    return root;
}

// Linked List operations
void insertInList(LinkedList* list, Molecule molecule) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    newNode->molecule = molecule;
    newNode->next = list->head;
    list->head = newNode;
}

// Function to search element by atomic number
void searchElementByNumber(int number) {
    for (int i = 0; i < totalElements; i++) {
        if (periodicTable[i].atomicNumber == number) {
            printf("Element Found: %s, Atomic Weight: %.2f, Classification: %s\n",
                   periodicTable[i].symbol,
                   periodicTable[i].atomicWeight, 
                   periodicTable[i].classification);
            return;
        }
    }
    printf("Element with atomic number %d not found.\n", number);
}

// Function to search element by symbol
void searchElementBySymbol(char symbol[]) {
    for (int i = 0; i < totalElements; i++) {
        if (strcmp(periodicTable[i].symbol, symbol) == 0) {
            printf("Element Found: %s, Atomic Weight: %.2f, Classification: %s\n",
                   periodicTable[i].symbol,
                   periodicTable[i].atomicWeight,
                   periodicTable[i].classification);
            return;
        }
    }
    printf("Element with symbol %s not found.\n", symbol);
}

// Function to display elements by classification
void displayElementsByClassification(char classification[]) {
    int found = 0;
    printf("Elements classified as %s:\n", classification);
    for (int i = 0; i < totalElements; i++) {
        if (strcmp(periodicTable[i].classification, classification) == 0) {
            printf("%d: %s, Atomic Weight: %.2f, Classification: %s\n",
                   periodicTable[i].atomicNumber,
                   periodicTable[i].symbol,
                   periodicTable[i].atomicWeight,
                   periodicTable[i].classification);
            found = 1;
        }
    }
    if (!found) {
        printf("No elements found in the classification %s.\n", classification);
    }
}

void sortElementsByWeight(int order) {
    // Create a copy to avoid modifying the original array
    Element tempTable[MAX_ELEMENTS];
    for (int i = 0; i < totalElements; i++) {
        tempTable[i] = periodicTable[i];
    }
    
    // Sort the copy
    for (int i = 0; i < totalElements - 1; i++) {
        for (int j = 0; j < totalElements - i - 1; j++) {
            if ((order == 1 && tempTable[j].atomicWeight > tempTable[j + 1].atomicWeight) ||
                (order == 2 && tempTable[j].atomicWeight < tempTable[j + 1].atomicWeight)) {
                Element temp = tempTable[j];
                tempTable[j] = tempTable[j + 1];
                tempTable[j + 1] = temp;
            }
        }
    }
    
    printf("Elements sorted by atomic weight:\n");
    for (int i = 0; i < totalElements; i++) {
        printf("%d: %s, Weight: %.2f, Classification: %s\n",
               tempTable[i].atomicNumber,
               tempTable[i].symbol,
               tempTable[i].atomicWeight,
               tempTable[i].classification);
    }
}

// Parse molecular formula and calculate weight
Molecule parseMolecularFormula(char* formula) {
    Molecule molecule = {0};
    strcpy(molecule.formula, formula);
    
    int i = 0;
    int elementIndex = 0;
    
    while (formula[i] != '\0' && elementIndex < 10) {
        char elementSymbol[3] = {0};
        int count = 0;
        
        // Skip parentheses for now (simplified parsing)
        if (formula[i] == '(' || formula[i] == ')') {
            i++;
            continue;
        }
        
        // Get element symbol
        if (isupper(formula[i])) {
            elementSymbol[0] = formula[i++];
            if (islower(formula[i])) {
                elementSymbol[1] = formula[i++];
            }
        } else {
            i++;
            continue;
        }
        
        // Get element count
        while (isdigit(formula[i])) {
            count = count * 10 + (formula[i] - '0');
            i++;
        }
        if (count == 0) count = 1;
        
        // Find element in periodic table
        for (int j = 0; j < totalElements; j++) {
            if (strcmp(periodicTable[j].symbol, elementSymbol) == 0) {
                // Check if element already exists in molecule
                int found = 0;
                for (int k = 0; k < elementIndex; k++) {
                    if (strcmp(molecule.elements[k].symbol, elementSymbol) == 0) {
                        molecule.elementCount[k] += count;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    molecule.elements[elementIndex] = periodicTable[j];
                    molecule.elementCount[elementIndex] = count;
                    elementIndex++;
                }
                break;
            }
        }
    }
    
    molecule.numElements = elementIndex;
    
    // Calculate molecular weight
    double weight = 0;
    for (int i = 0; i < molecule.numElements; i++) {
        weight += molecule.elements[i].atomicWeight * molecule.elementCount[i];
    }
    molecule.molecularWeight = weight;
    
    return molecule;
}

// Classify acidity/basicity (simplified)
const char* classifyAcidity(Molecule molecule) {
    int countH = 0;
    int countO = 0;
    int countN = 0;
    
    for (int i = 0; i < molecule.numElements; i++) {
        if (strcmp(molecule.elements[i].symbol, "H") == 0) {
            countH += molecule.elementCount[i];
        } else if (strcmp(molecule.elements[i].symbol, "O") == 0) {
            countO += molecule.elementCount[i];
        } else if (strcmp(molecule.elements[i].symbol, "N") == 0) {
            countN += molecule.elementCount[i];
        }
    }
    
    // Simplified classification
    if (countH > 0 && countO > 0) {
        return "Potentially Acidic/Basic";
    } else if (countN > 0 && countH > 0) {
        return "Potentially Basic";
    } else {
        return "Neutral";
    }
}

// Detect functional groups (simplified)
const char* detectFunctionalGroups(Molecule molecule) {
    int hasC = 0, hasO = 0, hasN = 0, hasH = 0;
    
    for (int i = 0; i < molecule.numElements; i++) {
        if (strcmp(molecule.elements[i].symbol, "C") == 0) hasC = 1;
        if (strcmp(molecule.elements[i].symbol, "O") == 0) hasO = 1;
        if (strcmp(molecule.elements[i].symbol, "N") == 0) hasN = 1;
        if (strcmp(molecule.elements[i].symbol, "H") == 0) hasH = 1;
    }
    
    if (hasC && hasO && hasH) {
        return "Possible Hydroxyl/Carbonyl group";
    } else if (hasN && hasH) {
        return "Possible Amino group";
    } else if (hasO && hasH) {
        return "Possible Hydroxyl group";
    } else {
        return "No obvious functional group";
    }
}

// Display molecule information
void displayMolecule(Molecule molecule) {
    printf("\nMolecule: %s\n", molecule.formula);
    printf("Molecular Weight: %.3f\n", molecule.molecularWeight);
    printf("Composition:\n");
    for (int i = 0; i < molecule.numElements; i++) {
        printf("  %s: %d (%.2f)\n", 
               molecule.elements[i].symbol, 
               molecule.elementCount[i],
               molecule.elements[i].atomicWeight);
    }
    
    printf("Metallic Nature:\n");
    for (int i = 0; i < molecule.numElements; i++) {
        printf("  %s: %s\n", 
               molecule.elements[i].symbol, 
               molecule.elements[i].classification);
    }
    
    printf("Classification: %s\n", classifyAcidity(molecule));
    printf("Functional Group: %s\n", detectFunctionalGroups(molecule));
}

// Inorder traversal of BST
void inorderBST(BSTNode* root) {
    if (root != NULL) {
        inorderBST(root->left);
        displayMolecule(root->molecule);
        inorderBST(root->right);
    }
}

int main() {
    Stack stack = {NULL};
    Queue queue = {NULL, NULL};
    LinkedList list = {NULL};
    BSTNode* bstRoot = NULL;

    int choice;
    char formula[MAX_FORMULA];
    
    do {
        printf("\n--- Smart Periodic Table ---\n");
        printf("\n--- Element Operator ---\n");
        printf("1. Search Element by Atomic Number\n");
        printf("2. Search Element by Symbol\n");
        printf("3. Display Elements by Classification\n");
        printf("4. Sort Elements by Atomic Weight\n");
        printf("\n--- Molecular Calculator ---\n");
        printf("5. Add Molecule\n");
        printf("6. Display All Molecules (Sorted by Weight)\n");
        printf("7. Process Molecular Weights (Using Stack)\n");
        printf("8. Process Molecules in Queue\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                int number;
                printf("Enter atomic number: ");
                scanf("%d", &number);
                searchElementByNumber(number);
                break;
            }
            case 2: {
                char symbol[5];
                printf("Enter element symbol: ");
                scanf("%s", symbol);
                searchElementBySymbol(symbol);
                break;
            }
            case 3: {
                char classification[20];
                printf("Enter classification (metal/nonmetal/noble gas/metalloid): ");
                scanf("%s", classification);
                displayElementsByClassification(classification);
                break;
            }
            case 4: {
                int order;
                printf("Enter sorting order (1 for ascending, 2 for descending): ");
                scanf("%d", &order);
                sortElementsByWeight(order);
                break;
            }
            case 5: {
                printf("Enter molecular formula (e.g., H2O, NH3, C6H12O6): ");
                scanf("%s", formula);
                
                Molecule molecule = parseMolecularFormula(formula);
                
                // Add to all data structures
                push(&stack, molecule);
                enqueue(&queue, molecule);
                insertInList(&list, molecule);
                bstRoot = insertBST(bstRoot, molecule);
                
                printf("Molecule added successfully!\n");
                displayMolecule(molecule);
                break;
            }
            
            case 6: {
                printf("\nMolecules sorted by molecular weight:\n");
                if (bstRoot == NULL) {
                    printf("No molecules added yet.\n");
                } else {
                    inorderBST(bstRoot);
                }
                break;
            }
            
            case 7: {
                printf("\nProcessing molecular weights from stack:\n");
                if (stack.top == NULL) {
                    printf("No molecules in stack.\n");
                } else {
                    while (stack.top != NULL) {
                        Molecule molecule = pop(&stack);
                        displayMolecule(molecule);
                    }
                }
                break;
            }
            
            case 8: {
                printf("\nProcessing molecules from queue:\n");
                if (queue.front == NULL) {
                    printf("No molecules in queue.\n");
                } else {
                    while (queue.front != NULL) {
                        Molecule molecule = dequeue(&queue);
                        displayMolecule(molecule);
                    }
                }
                break;
            }
            
            case 9:
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 9);
    
    return 0;
}