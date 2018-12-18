/*Program: 
Project4.cpp for CSC 230-02
Author: Miles Cumiskey 
---------------------------
Description: a city database using a BST. The data will be read in from a file and then it will be stored in local database 
implemented a BST. Each database record contains the name of the city (a string of arbitrary length) and the coordinates of 
the city expressed as integer x- and y-coordinates.

The BST should be organized by city name. The database should permit records to be inserted, deleted by name or coordinate 
and searched by name or coordinate. Operations to display the complete contents of the database as well as to clear the database 
completely should also be implemented. When records are purged or the BST cleared you must be sure to run destructor code to reclaim memory.

After the data is loaded from the file, display a menu of options and allow the user to select from the menu until they 
are finished. When the user is finished be sure to close the file and clear the database before exiting.
   
Input: an ASCII text file with formatted city names and GPS coordinates
Output: a menu that allows the user to choose different options for the BST
Preconditions: none
Postconditions: none
---------------------------*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace std;


class city_object {
    public:
    int xCoordinate;
    int yCoordinate; 
    string cityName;


    city_object(){
        xCoordinate = 0;
        yCoordinate = 0; 
        cityName = "default";
    }

    city_object(string n, int x, int y){
        xCoordinate = x;
        yCoordinate = y; 
        cityName = n;
    }

    void display(){
        cout << cityName << " x: " << xCoordinate << " y: " << yCoordinate << endl;
    }
};

class node {
    public:
    city_object city;
    node * left;
    node * right; 

    node (city_object data) {
        city = data;
        left = nullptr;
        right = nullptr; 
    }
};

class tree {
    public:
    node *root;
    int height = 0;

    tree(){
        root = nullptr;
        cout << "NULL TREE" << endl;
    }
    ~tree(){ }
    tree(city_object data){
        root = new node(data);
        cout << "ROOT: ";
        data.display();
        cout << " root val = " << root->city.cityName << endl; 
    }
    //search for a city name in the BST, recursive
    bool searchFor(node * recursiveNode, string name){
        if(recursiveNode == NULL){
            cout << "No record found for " << name << endl;
            return false;
        }
        if(recursiveNode->city.cityName == name){
            cout << "Record found for " << name << " at " << recursiveNode->city.xCoordinate << " " << recursiveNode->city.yCoordinate << endl;
            return true;
        } 
        if(recursiveNode->city.cityName < name){ //go right
            return searchFor(recursiveNode->right, name);
        }
        if(recursiveNode->city.cityName > name){    //go left
            return searchFor(recursiveNode->left, name);
        } 
    }

    //search for coordinates in the BST, recursive
    int searchFor(node * recursiveNode, int x, int y){
        if (recursiveNode == NULL){ //end case
            return false; 
        } 
        //go left
        searchFor(recursiveNode->left, x, y); 
    
        //found!
        if((recursiveNode->city.xCoordinate == x) && (recursiveNode->city.yCoordinate == y)){
            cout << "Record found for " << x << ", " << y << " at " << recursiveNode->city.cityName << endl;
            return true;
        }
    
        //go right
        searchFor(recursiveNode->right, x, y); 
    }

    node * returnSearch(node * recursiveNode, int x, int y){
        if (recursiveNode == NULL){ //end case
            return NULL; 
        } 
        //go left
        returnSearch(recursiveNode->left, x, y); 
    
        //found!
        if((recursiveNode->city.xCoordinate == x) && (recursiveNode->city.yCoordinate == y)){
            return recursiveNode;
        }
    
        //go right
        returnSearch(recursiveNode->right, x, y); 
    }

    void add (city_object data) {
        node *next = root;
        while(next != NULL){
            if(data.cityName == next->city.cityName){
                cout << "Duplicate: ";
                data.display();
                cout << " " << next->city.cityName << endl;            
                break;            
             }
             if(data.cityName > next->city.cityName){
                cout << "RIGHT = ";
                data.display();
                cout << " node val = " << next->city.cityName << endl;

                if (next->right == NULL) {
                    cout << "adding ";
                    data.display();
                    cout << " to the right of " << next->city.cityName << endl;
                    next->right = new node(data);
                    break;
                } else {
                    next = next->right;
                }
             }
            if(data.cityName < next->city.cityName){
                cout << "LEFT = ";
                data.display();
                cout << " node val = " << next->city.cityName << endl;
                if (next->left == NULL) {
                    cout << "adding ";
                    data.display();
                    cout << " to the left of " << next->city.cityName << endl;
                    next->left = new node(data);
                    break;
                } else {
                    next = next->left;
                }
            }
        }
    }
   //Given a binary search tree and a name, this function deletes the node and returns the new root
   node * deleteNode(node * recursiveNode, string name) { 
    if (recursiveNode == NULL){     // end case 
        return recursiveNode; 
    } 

    if(name < recursiveNode->city.cityName){                            //if the thing you want to delete is smaller than your root, go left
        recursiveNode->left = deleteNode(recursiveNode->left, name);
    } else if (name > recursiveNode->city.cityName){                     //thing is bigger than current, go right 
        recursiveNode->right = deleteNode(recursiveNode->right, name);
    } else {                                                             //if neither of the above... delete node!
         //node with only one child or no child 
        if (recursiveNode->left == NULL){
            node *temp = recursiveNode->right; 
            free(recursiveNode); 
            return temp; 
        } else if (recursiveNode->right == NULL) { 
            node *temp = recursiveNode->left; 
            free(recursiveNode); 
            return temp; 

        //node with two children
         } else {
             node * temp = smallestNode(recursiveNode->right); //find successor
             recursiveNode->city = temp->city;                 //copy successor's contents upward (keeping other links unchanged)
             recursiveNode->right = deleteNode(recursiveNode->right, name); //delete the unworthy 
         }
    }
    return recursiveNode;
   }

   //Given a binary search tree and coordinates, this function deletes the node with the matching coordinates
    void deleteNode(node * recursiveNode, int x, int y){
        //uses the search in order to find the name of the city, and deletes using existing code
        deleteNode(recursiveNode, returnSearch(recursiveNode, x, y)->city.cityName);
    }

        
    //finds the leftmost node, starting at the node given 
    node * smallestNode (node * recursiveNode){ 
        node * current = recursiveNode; 
    
        // loop down to find the leftmost child with no left children 
        while (current->left != NULL){
            current = current->left; 
        }  
        return current; 
    }  

    void clearTree(node *recursiveNode){
         if (recursiveNode == NULL){ //end case
            return; 
        } 
        //go left
        clearTree(recursiveNode->left); 
    
        //process current node
        deleteNode(recursiveNode, recursiveNode->city.cityName);
    
        //go right
        clearTree(recursiveNode->right); 
    }

    void printInorder(node * recursiveNode){ 
        if (recursiveNode == NULL){ //end case
            return; 
        } 
        //go left
        printInorder(recursiveNode->left); 
    
        //process current node
        recursiveNode->city.display();
    
        //go right
        printInorder(recursiveNode->right); 
    } 
};


//check if a given file opens
bool doesFileOpen(ifstream &infile) {
    if(infile.fail()) {
        return false;
    } else {
        return true;
    }
};
//checks if a string is a number - includes negative sign
bool isNumber(string ch){
    string numbers = "-0123456789";
    return !ch.empty() && ch.find_first_not_of(numbers) == std::string::npos;
};

tree run(ifstream &infile);     //takes everything from the file stream, parses it, and returns the BST.
bool isValid(string line);      //makes sure a line can be parsed
bool isNotDuplicate(vector <city_object> cities, city_object line); //makes sure a line is not a duplicate 
city_object parseLine(string line); //turns an input line into a city object

int main(int argc, char* argv[]){

    string filename;
    ifstream infile;
    tree cityDirectory; 
    
    if (argc == 1) {
        cout << "Please an input file: ";
        cin >> filename;
        ifstream infile(filename);  
        if (doesFileOpen(infile) == true) { 
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            cout << "File: " << filename << " opened sucessfully." << endl;
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            cityDirectory = run(infile);
        } else {
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            cout << "Error - couldn't process " << filename << endl;
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;

            return 1;
        }  
    } else {
        filename = argv[1];
        ifstream argfile(filename);  
         if (doesFileOpen(argfile) == true) { 
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            cout << "File: " << filename << " opened sucessfully." << endl;
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            cityDirectory = run(argfile);
        } else {
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            cout << "Error - couldn't process " << filename << endl;
            cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            return 1;
        }
    }    

    cout << "Database Contents:" << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    cityDirectory.printInorder(cityDirectory.root);
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;

    int commandInput = 0;
    int xSearch = 0;
    int ySearch = 0; 
    string nameSearch = "";
    while(commandInput != 3){
        cout << "Search options: 1 - by City Name, 2 - by Coordinates, 3 - EXIT" << endl;
        cin >> commandInput;
        if(commandInput == 1){
            cout << "Enter a city name: ";
            cin >> nameSearch;
            cityDirectory.searchFor(cityDirectory.root, nameSearch);
        } 
        if(commandInput == 2){
            cout << "Enter the city coordinates separated by a space: ";
            cin >> xSearch;
            cin >> ySearch;
            cityDirectory.searchFor(cityDirectory.root, xSearch, ySearch);
        }
        cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
    }
    cout << "End of run. " << endl;
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;


    return 0;
}

tree run(ifstream &infile){
    string tempString;
    vector <city_object> cities; 
    city_object tempCity; 
    while (!infile.eof()) {
        getline(infile, tempString);
        if(isValid(tempString)){
            cout << "Parsing: " << tempString << endl;
            tempCity = parseLine(tempString);
            if(isNotDuplicate(cities, tempCity)){
                cities.push_back(tempCity);
            } else {
                cout << tempString << " is a duplicate and will not be added to the tree." << endl; 
            }
        } 
    }

    tree cityDirectory(cities[0]);

    for(int i = 1; i < cities.size(); i++){
        cityDirectory.add(cities[i]);
    }

    return cityDirectory; 
}

//this function checks if a given line from the file is valid
bool isValid(string line) {
    char delimiter = ','; //seperates by comma 
    vector<string> name; //vector in case there are two words
    vector<int> coordinates; //for the ints
    string acc = ""; //accumulated bit of string
    
    for(int i = 0; i <= line.size(); i++) {                     //go through the line
        if(line[i] == delimiter || (i == line.size())) {        //if there is a comma,
            if(isNumber(acc)){                                  //check if number
                coordinates.push_back(stoi(acc));               //if it is, add it to the numbers and reset acc
                acc = "";
            } else {                                            //else add it to the name vector
                name.push_back(acc);
                acc = "";
            }  
        } else {                                                //not a comma? add it to acc. 
            acc += line[i];
        }
    }
    if((name.size() == 1) && (coordinates.size() == 2)){         //if there is one word and two ints, all good!
        return true;
    } else {
        cout << line << " could not be parsed." << endl;        //otherwise, error 
        return false;
    }
}

bool isNotDuplicate(vector <city_object> cities, city_object curCity){
    for(int i = 0; i < cities.size(); i++){
        if(cities[i].cityName == curCity.cityName){
            return false;
        }
    }
    return true;
}

//assumes that the string is correctly formatted, and creates a city_obj from it 
city_object parseLine(string line){
     char delimiter = ','; //seperates by comma 
    vector<string> name; //vector in case there are two words
    vector<int> coordinates; //for the ints
    string acc = ""; //accumulated bit of string
    
    for(int i = 0; i < line.size(); i++) {                      //go through the line
        if((line[i] == delimiter) || (i == line.size()-1)) {    //if there is a comma,
            if(isNumber(acc)){                                  //check if number
                if(i == line.size()-1){                         //if this is the end of the line
                    acc += line.substr(i, line.size());         //make sure to add everything!
                }
                coordinates.push_back(stoi(acc));               //if it is, add it to the numbers and reset acc
                acc = "";
            } else {                                            //else add it to the name vector
                name.push_back(acc);
                acc = "";
            }  
        } else {                                                //not a comma? add it to acc. 
            acc += line[i];
        }
    }

    city_object city(name[0], coordinates[0], coordinates[1]);
    return city; 
}
