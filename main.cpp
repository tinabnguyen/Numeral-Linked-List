#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cctype>
using namespace std;

// struct definition
struct node
{
   int arabic;
   string roman;
   node *next = nullptr;
};

// This function checks if the roman numeral is valid
bool romanVal (string rNum) {
    string valid = "IVXLCDM";                                   // This string contains all valid roman letters
    for (long unsigned int i = 0; i < rNum.length(); i++){
        if (valid.find(rNum[i]) == string::npos) {              // a character in the roman numeral input isn't one of the valid, then it's invalid
            return false;
        }
    }
    return true;
}

// This function checks if the arabic numeral is valid
bool arabicVal(string aNum) {
    for (long unsigned int i = 0; i < aNum.length(); i++) {
        if (!isdigit(aNum[i]) && aNum[i] != ' ') {              // checks to see if the arabic character is a digit or space
            return false;
        }
    }
    
    int arabic = stoi(aNum);
    if (arabic < 1 || arabic > 4999) {                          // checks to see if the arabic numeral is in the range
        return false;
    }
    
    return true;
}

// This function returns the arabic equivalent of each roman letter
int romanValues(char letter) {
    if (letter == 'I') {
        return 1;
    }
    if (letter == 'V') {
        return 5;
    }
    if (letter == 'X') {
        return 10;
    }
    if (letter == 'L') {
        return 50;
    }
    if (letter == 'C') {
        return 100;
    }
    if (letter == 'D') {
        return 500;
    }
    if (letter == 'M') {
        return 1000;
    }
    else {
        return 0;
    }
}

// Convert from roman numeral to arabic numeral
int romanToArabic(string romanNumeral) {
    
    int sum = 0;

    for (long unsigned int i = 0; i < romanNumeral.length(); i++) {

        int num1 = romanValues(romanNumeral[i]);            // value of the first roman letter       

        if (i + 1 < romanNumeral.length()) {                
            int num2 = romanValues(romanNumeral[i+1]);      // get the next roman letter value
            
            if (num1 >= num2) {                         
                sum += num1;                                // normal adding values
            }
            else {

                sum += (num2 - num1);                       // for special cases like 4s and 9s
                i++;
            }
        }
        else {
            sum += num1;
        }
    }
    return sum;
}

// convert from arabic numeral to roman numeral
string arabicToRoman(int arabicNumeral) {
    
    // These 2 arrays correspond with each other in value
    string roman[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    int arabic[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    string final;
    
    for (int i =0; i < 13; i++) {
        while (arabicNumeral - arabic[i] >= 0) {     // determine the first value of digit       
            final += roman[i];                       // adds the roman letters to the final string       
            arabicNumeral -= arabic[i];              // determine the remaining value       
        }
    }
    return final;
}

// This function reads in the data from the file and puts them into a node, and then add the node to a linked list
void readFile(node *&head, ifstream &infile) {

    string row;
    string araNum, romanNum;
    int ara;
    long unsigned int first = 0;
    
    while (infile.good()) {
        while (getline(infile, row)) {               
            node *newNode = new node;                // dynamically allocated node for each line of file
            newNode->next = nullptr;                 
            
            first = row.find(' ');                   // find the first space in the row
            
            if (first == 0) {                        // if the first space is at position 0, then that means that there is an arabic numeral given
                araNum = row.substr(17,4);           // get arabic substring from row
                if (arabicVal(araNum)) {             // validate arabic numeral
                    ara = stoi(araNum);              // convert string arabic to int arabic
                    newNode->arabic = ara;           // add int arabic to dynamic node
                    romanNum = arabicToRoman(ara);   // convert int arabic to string roman
                    newNode->roman = romanNum;       // add string roman to dynamic node
                    
                    if (head == nullptr) {           // empty list
                        head = newNode;
                    }
                    else {                           // add to beginning of list
                        newNode->next = head;
                        head = newNode;
                    }
                }
                else  {                             // if not valid arabic numeral, skip
                    continue;
                }
            }
            else {                                  // given the roman numeral in line
                romanNum = row.substr(0, first);    // get roman substring from row
                if (romanVal(romanNum)) {           // validate roman numeral
                    newNode->roman = romanNum;      // add string roman to dynamic node
                    ara = romanToArabic(romanNum);  // convert string roman to int arabic
                    newNode->arabic = ara;          // add int arabic to dynamic node
                    
                    if (head == nullptr) {          // empty list
                        head = newNode;
                    }
                    else {                          // add to beginning of list
                        newNode->next = head;
                        head = newNode;
                    }
                }
                else {                              // if not valid roman numeral, skip
                    continue;
                }
            }

        }
    }
    
    infile.close(); 
}

// This function searches the linked list for either roman or arabic numerals using linear search algorithm
void searchList (node *head, string searchValue) {
    node *ptr = head;
    bool found = {false};
    
    int arabicSearchVal;
    
    if (isdigit(searchValue[0])) {                                  // if the first position of the search value is a digit, then searching for arabic numeral
        arabicSearchVal = stoi(searchValue);                        // convert the string searchVal to int
        
        while (ptr != nullptr) {                                    // linear search algorithm
            if (ptr->arabic == arabicSearchVal) {                   // if the current arabic equals the search value, then found
                cout << arabicSearchVal << " " << "found\n\n";
                found = true;
                break;
            }
            ptr = ptr->next;
        }
        
        if (!found) {
            cout << arabicSearchVal << " " << "not found\n\n";
        }
    }
    else {                                                          // if the first position is not a digit, then searching for a roman numeral
        while (ptr != nullptr) {                                    // linear search algorithm
            if (ptr->roman == searchValue) {                        // if current roman equals search value, then found
                cout << searchValue << " " << "found\n\n";
                found = true;
                break;
            }
            ptr = ptr->next;
        }
        
        if (!found) {
            cout << searchValue << " " << "not found\n\n";
        }
    }
}

// This function sorts the linked list by roman or arabic using bubble sort algorithm
void sortList (node *&head, string numeralType) {
 
   bool swapped;
   node *ptr = nullptr;
   node *ptr2 = nullptr;
   
   if (head == nullptr) {           // empty list, no sorting
       return;
   }
   
   do 
   {
       swapped = false;
       ptr = head;
       
       while (ptr->next != ptr2){     
           if ((numeralType == "Roman" && ptr->roman > ptr->next->roman) ||        //compare the numeral type and the numeral value of current and next in list
           (numeralType == "Arabic" && ptr->arabic > ptr->next->arabic)) {         // if current data is greater than next data
               node *temp = ptr->next;
               ptr->next = ptr->next->next;                                        // swapping nodes
               temp->next = ptr;
               
               if (ptr == head) {                   // swap to the beginning
                   head = temp;
               }
               else {                              // swapping in middle/end
                   node *prev = head;
                   while (prev->next != ptr) {
                       prev = prev->next;
                   }
                   prev->next = temp;
               }
               
               ptr = temp;
               swapped = true;
           }
           else {
               ptr = ptr->next;
           }
       }
       ptr2 = ptr;
       
   }while(swapped);
   
   ptr = head;
   while(ptr != nullptr) {                           
       if (numeralType == "Roman") {            // printing out the sorted list of roman numerals
           cout << ptr->roman << endl;
       }
       else if (numeralType == "Arabic") {      // printing out the sorted list of arabic numerals
           cout << ptr->arabic << endl;
       }
       ptr = ptr->next;
   }
   
   cout << endl;
}

// this function prints out the linked list recursively to the output file
void printList(node *head, ofstream &outfile) {
    
    if (head == nullptr) {          // base case, if at the end of the list, stop
        return;
    }
    
    outfile << left << setw(17) << setfill(' ') << head->roman;            // output roman 
    outfile << left << setw(4) << setfill(' ') << head->arabic << endl;    // output arabic
        
    printList(head->next, outfile);    // recursive function call
    
    outfile.close();
}

int main()
{
    node *head = nullptr;
    
    string name;
    cout << "Please enter file name of input file: ";
    cin >> name;
    
    ifstream infile;
    infile.open(name);
    if (!infile) {                                          // checks to see if infile opened properly
        cout << "Error opening input file." << endl;
        return -1;
    }
    
    readFile(head, infile);                                 // call readfile function
    
    int menu;
    do                                                      // driver menu
    {
        cout << "1. Search\n"
            << "2. Sort\n"
            << "3. Exit\n\n"
            << "Please make a selection: ";
        cin >> menu;
        
        if (menu == 1) {                                    // search list
            string numeral;
            cout << "Please enter a numeral to find: ";
            cin >> numeral;
            searchList(head, numeral);
        }
        else if (menu == 2) {                               // sort list
            int submenu;
            cout << "1. Sort by Roman numeral\n"
                << "2. Sort by Arabic numeral\n\n"
                << "Please make a sub selection: ";
            cin >> submenu;
            
            if (submenu == 1) {
                sortList(head, "Roman");
            }
            else if (submenu == 2) {
                sortList(head, "Arabic");
            }
        }
        
    } while (menu != 3);
    
    ofstream outfile;                   
    outfile.open("numbers.txt");
    if (!outfile) {                                      // checks to see if output file opened properly
        cout << "error opening output file." << endl;
        return -1;
    }
    
    printList(head, outfile);       // call print function at the end of the program

   cout << "Hello, world" << endl;
    
    return 0;        
}
