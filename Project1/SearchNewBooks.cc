#include<iostream>
#include<stdio.h>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<sstream>
#include <chrono> //NOLINT (build/c++11)
using namespace std;



class Book {
    public:
        string isbn, lang, type;

        Book (string isbn, string lang, string type){
            this->isbn = isbn;
            this->lang = lang;
            this->type = type;
        }

};


// Overload == for searching comparisons
bool operator== (Book& b1, Book& b2){
    if (b1.isbn == b2.isbn && b1.lang == b2.lang && b1.type == b2.type)
        return true;

    else
        return false;
}



// Function to find the index of the last element in a Book vector
int lastIndex (vector<Book>& list){
    int counter = 0;
    for (Book& b : list){
        if (b == b){        // would not compile if b was not used in some way
            counter++;
        }
    }
    return counter - 1; // the first Book is at index 0
}




// Overload << to print Book objects nicely
ostream& operator<< (ostream& os, Book& book){
    os << book.isbn << ", " << book.lang << ", " << book.type << endl;
    return os;
}





// Overload << to print Book vectors nicely
ostream& operator<< (ostream& os, vector<Book> list){
    for (Book b : list)
        os << b.isbn << ", " << b.lang << ", " << b.type << endl;

    return os;
}




// Create a function to split the lines read in from files into a a string vector of substrings
// delimited by commas so that the substrings can be passed as arguments into
// the Book constructor
vector<string> split (const string& str, char delimiter){
    
    // The vector we will return
    vector<string> substrings;

    // Starting indices for substring
    string::size_type start = 0;                    // first element
    string::size_type end = str.find(delimiter);    // index of first instance of delimiter

    while (end != string::npos){
        substrings.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    // Include the final substring separately, as it won't be included by the
    // while loop because if it does not end in the delimiter
    substrings.push_back(str.substr(start, end));
    return substrings;
}





// Function to get data from .dat file inputs
vector<Book> parse_data (ifstream& file){

    // Create an string variable to store each line
    string line;

    // Create a Book vector to return
    vector<Book> list;

    while(getline(file,line)){

            
        // Create a vector to store substrings containing
        // each of the parameters for the Book constructor
        vector<string> substr = split(line, ',');               
        
        // Create a new Book object with the data from the current line
        Book b = Book(substr[0], substr[1], substr[2]);

        // Append the new book in the booklist
        list.push_back(b);
    }

    return list;
}





// Overload `<` for sorting
bool operator< (Book& b1, Book& b2){

    // Convert string ISBN to integers (otherwise)
    // will sort only by first digit
    int isbn1, isbn2;
    istringstream(b1.isbn) >> isbn1;
    istringstream(b2.isbn) >> isbn2;
    
    if (isbn1 < isbn2){
        return true;
    }
    else if (b1.isbn == b2.isbn){

        if (b1.type == "new" && b2.type == "old")
            return true;
        else if(b1.type == "new" && b2.type == "digital")
            return true;
        

        else if (b1.type == "old" && b2.type == "digital")
            return true;


        else if (b2.type == "new" && b1.type == "old")
            return false;
        else if(b2.type == "new" && b1.type == "digital")
            return false;
        else if (b2.type == "old" && b1.type == "digital")
            return false;
        else if (b2.type == b1.type)
            return b1.lang < b2.lang;
        else
            return false;
        
    }
    else
        return false;
}









// Function for writing result of searches to found.dat
void writeResultFile (int num, string& filename){
    ofstream outputFile;
    outputFile.open(filename);
    outputFile << num;
    outputFile.close();
}






int linearSearch (vector<Book>& available, vector<Book>& requested){

   //----------------- START THE CLOCK --------------------------
    std::chrono::high_resolution_clock::time_point start;
    start = std::chrono::high_resolution_clock::now();

    // Counter to hold number of books found
    int found = 0;

    for (Book& b_req : requested){    
        for (Book& b_avail : available){
            if (b_req == b_avail){
                found++;
                break;
            }
        }
    }
//--------------------- STOP THE CLOCK ------------------------
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed_us = std::chrono::duration<double, std::micro>(end - start).count();
    std::cout << "CPU time: " << elapsed_us << " microseconds" << endl;


    return found;
}




int binarySearch(vector<Book>& available, vector<Book>& requested) {

//----------------- START THE CLOCK --------------------------
    std::chrono::high_resolution_clock::time_point start;
    start = std::chrono::high_resolution_clock::now();


    // First, sort the list of available books
    sort(available.begin(), available.end());     
    
    
    // Now implement binary search algo
    int found = 0;     // counter

    for (Book& b : requested){
        int left = 0;
        int right = lastIndex(available);
        while (left <= right){

            int mid = (left + right) / 2;

            
            if (b == available[mid]){
                found++;
                break;
            }

            else if ((stoi(b.isbn) < stoi(available[mid].isbn)) || (b.lang < available[mid].lang) || (b.type < available[mid].type)){
                right = mid - 1;
            }

            else if ((stoi(b.isbn) > stoi(available[mid].isbn)) || (b.lang > available[mid].lang) || (b.type > available[mid].type)){
                left = mid + 1;
            }
            else {
                cout << "i brokey" << endl;
                break;
            }

        }
    }


    //--------------------- STOP THE CLOCK ------------------------
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed_us = std::chrono::duration<double, std::micro>(end - start).count();
    std::cout << "CPU time: " << elapsed_us << " microseconds" << endl;




    return found;

}




bool recursiveBinarySearch(vector<Book>& available, Book& b, int l, int r) {

    //int found = counter;
    int left = l;
    int right = r;

    if (left <= right){
        int mid = (left + right) / 2;

        if (b == available[mid]){
            return true;
        }

        else if ((stoi(b.isbn) < stoi(available[mid].isbn)) || (b.lang < available[mid].lang) || (b.type < available[mid].type)){
            return recursiveBinarySearch(available, b, left, mid - 1);
        }

        else if ((stoi(b.isbn) > stoi(available[mid].isbn)) || (b.lang > available[mid].lang) || (b.type > available[mid].type)){
            return recursiveBinarySearch(available, b, mid + 1, right);
        }

        else {
            cout << "i brokey" << endl;
            return false;
        }

    }

    return false;
}







void search(vector<Book>& available, vector<Book>& requested, string& outputFile){

    char searchType;
    cout << "Choice of search method ([l]inear, [b]inary, [r]ecursiveBinary)?" << endl;
    cin >> searchType;

    while (!((searchType == 'l') || (searchType == 'b') || (searchType == 'r'))){
        // Inform user of invalid response and reprompt
        cout << "Incorrect choice" << endl;
        cin >> searchType;
    }

    if (searchType == 'l'){
            // Implement linear search and write results to found.dat
            writeResultFile(linearSearch(available, requested), outputFile);
            return;
    }

    else if (searchType == 'b'){
            // Implement non-recursive binary search and write results to found.dat
            writeResultFile(binarySearch(available, requested), outputFile);
            return;
    }
    else if (searchType == 'r'){

            //----------------- START THE CLOCK --------------------------
            std::chrono::high_resolution_clock::time_point start;
            start = std::chrono::high_resolution_clock::now();


            // Need to first sort the available list outside of the
            // recursive function, or it will waste time trying
            // to sort it at each subsequent call
            sort(available.begin(), available.end());     
    
            // Initialize counter
            int found = 0;

            // Then need to store the size of the available vector to find its
            // rightmost index
            int right = lastIndex(available);


            // Need to implement recursion for each requested book
            // If a book is found, increment the counter
            for (Book& b : requested){
                if (recursiveBinarySearch(available, b, 0, right) == true){
                    found++;
                }
            }


            //--------------------- STOP THE CLOCK ------------------------
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed_us = std::chrono::duration<double, std::micro>(end - start).count();
            std::cout << "CPU time: " << elapsed_us << " microseconds" << endl;



            // Implement recursive binary search and write results to found.dat
            writeResultFile(found, outputFile);

            return;
    }
    

}





//------------------- BEGINNING OF MAIN FUNCTION ------------------------------------------------------




int main(int argc, char* argv[]){

    //-------------- MAKE SURE THE ARGUMENTS AREN'T BORKED -----------------------------
    
    if (argc != 3 && argc != 4){
        cerr << "Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>" << endl;
    }

    // Change default output file, if specified
    string outputFile = "found.dat";
    if (argc == 4){
        outputFile = argv[3];
    }

    //--------------------- READ IN DATA FROM FILES -------------------------

    // Read first file from cli as the book list
    ifstream bookListFile(argv[1]);


    // Catch error if file not found
    if (!bookListFile){
        cerr << "Error: cannot open file " << argv[1] << endl;
    }


    // Create a Book vector to store the all of the Books
    vector<Book> bookList = parse_data(bookListFile);


    bookListFile.close();


    ifstream requestFile(argv[2]);

    // Catch error if file not found
    if (!requestFile){
        cerr << "Error: cannot open file " << argv[2] << endl;
    }

    // Create a Book vector to store the requested books
    vector<Book> requestList = parse_data(requestFile);

    requestFile.close();

     
    //------- DO ALL THE THINGS ------
   
    search(bookList, requestList, outputFile);


    return 0;
}


