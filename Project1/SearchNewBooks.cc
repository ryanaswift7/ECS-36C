#include<iostream>
#include<stdio.h>
#include<fstream>
#include<string>
#include<vector>
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




// Overload << to print Book objects nicely
ostream& operator<< (ostream& os, Book book){
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








int main(int argc, char* argv[]){

    //-------------- MAKE SURE THE ARGUMENTS AREN'T BORKED -----------------------------
    
    if (argc != 3 && argc != 4){
        cerr << "Usage: .SearchNewBooks <newBooks.dat> <request.dat> <result_file.dat>" << endl;
    }

    
    // Read first file from cli as the book list
    ifstream bookListFile(argv[1]);


    // Catch exception if file not found
    if (!bookListFile){
        cerr << "Error opening file" << endl;
    }

    //--------------------- READ IN DATA FROM FILES -------------------------

    // Create a Book vector to store the all of the Books
    vector<Book> bookList = parse_data(bookListFile);


    bookListFile.close();


    ifstream requestFile(argv[2]);

    // Create a Book vector to store the requested books
    vector<Book> requestList = parse_data(requestFile);

    requestFile.close();

        
    cout << "Book List: " << endl << bookList << endl;
    cout << "Requested Books: " << endl << requestList << endl;

    //--------------- SORT THE BOOK LIST -----------------------------
    




    


    
    return 0;
}



/* Need to:
    - read files into a vector (or one vector per line?)
    - call Book constructor on each line and append it to the vector?*/