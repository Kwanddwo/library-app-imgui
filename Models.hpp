#pragma once
#include <string>
#include <vector>

class User;
class Book;
class Borrowing;

// Base User class
enum class UserRole
{
    MEMBER,
    LIBRARIAN,
    ADMIN
};


class User {
protected:
    int id;
    std::string email;
    std::string firstName;
    std::string lastName;
    UserRole role;

public:
    User(const int id, const std::string& email, const std::string& firstName, const std::string& lastName, const UserRole& role) :
        id(id), email(email), firstName(firstName), lastName(lastName), role(role) {};
    User() {
        id = -1; email = firstName = lastName = ""; role = UserRole::MEMBER;
    }
    // No need for a copy constructor as cpp gives a default
    
    virtual ~User() = default;

    virtual bool canViewBooks() const { return true; }
    virtual bool canReserveBooks() const { return role == UserRole::MEMBER; }
    virtual bool canManageBooks() const { return role >= UserRole::LIBRARIAN; }
    virtual bool canManageMembers() const { return role >= UserRole::LIBRARIAN; }
    virtual bool canManageAllUsers() const { return role == UserRole::ADMIN; }
    virtual bool canViewStatistics() const { return role == UserRole::ADMIN; }

    std::string getEmail() { return email; };
    std::string getFirstName() { return firstName; };
    std::string getLastName() { return lastName; };
    UserRole getRole() const { return role; }
    static UserRole stringToUserRole(std::string role) {
        if (role == "member") {
            return UserRole::MEMBER;
        }
        if (role == "librarian") {
            return UserRole::LIBRARIAN;
        }
        if (role == "admin") {
            return UserRole::ADMIN;
        }
    }
    static std::string userRoleToString(UserRole role) {
        if (role == UserRole::MEMBER) {
            return "member";
        }
        if (role == UserRole::LIBRARIAN) {
            return "librarian";
        }
        if (role == UserRole::ADMIN) {
            return "admin";
        }
    }
    // Méthode d'affichage
    virtual void display() const {
        std::cout << "User Information:\n"
            << "ID: " << id << "\n"
            << "Email: " << email << "\n"
            << "First Name: " << firstName << "\n"
            << "Last Name: " << lastName << "\n"
            << "Role: " << userRoleToString(role) << "\n";
    }
};

class author {
    int id;
    std::string firstname;
    std::string lastname;
    std::string dateofbirth;
public:
    author() : id(0), firstname(""), lastname(""), dateofbirth("") {}
    author(int _id, const std::string& _firstname, const std::string& _lastname, const std::string& _dateofbirth)
        : id(_id), firstname(_firstname), lastname(_lastname), dateofbirth(_dateofbirth) {
    }
    // Setters
    void setFirstname(const std::string& _firstname) { firstname = _firstname; }
    void setLastname(const std::string& _lastname) { lastname = _lastname; }
    void setDateofbirth(const std::string& _dateofbirth) { dateofbirth = _dateofbirth; }
    // Getters
    int getId() const { return id; }
    std::string getFirstname() const { return firstname; }
    std::string getLastname() const { return lastname; }
    std::string getDateOfBirth() const { return dateofbirth; }
    // Méthode d'affichage 
    void display() const {
        std::cout << "Author ID: " << id << "\nFirstname: " << firstname
            << "\nLastname: " << lastname << "\nDate of Birth: " << dateofbirth << "\n";
    }
};




class editor {
    int id;
    std::string name;
public:
    editor() : id(0), name("") {}
    editor(int _id, const std::string& _name) : id(_id), name(_name) {}
    // Setters
    void setId(int _id) { id = _id; }
    void setName(const std::string& _name) { name = _name; }
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    // Méthode d'affichage
    void display() const { std::cout << "Editor ID: " << id << "\nName: " << name << "\n";}
};

class language {
    int id; 
    std::string name;
    char code[2];
public:

    language() : id(0), name(""), code{ ' ', '\0' } {}
    language(int _id, const std::string& _name, const char _code[2]) : id(_id), name(_name) {
        code[0] = _code[0];
        code[1] = _code[1];
        code[2] = '\0';
    }
    // Setters
    void setId(int _id) { id = _id; }
    void setName(const std::string& _name) { name = _name; }
    void setCode(const char _code[2]) {
        code[0] = _code[0];
        code[1] = _code[1];
        code[2] = '\0';
    }
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCode() const { return std::string(code); }
    // Méthode d'affichage
    void display() const {std::cout << "Language ID: " << id << "\nName: " << name << "\nCode: " << code << "\n";}
};

class categories {
    int id;
    std::string name;
    std::string type;
public:
    categories() : id(0), name(""), type("") {}
    categories(int _id, const std::string& _name, const std::string& _type)
        : id(_id), name(_name), type(_type) {
    }
    // Setters
    void setId(int _id) { id = _id; }
    void setName(const std::string& _name) { name = _name; }
    void setType(const std::string& _type) { type = _type; }
    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    // Méthode d'affichage
    void display() const {std::cout << "Category ID: " << id << "\nName: " << name << "\nType: " << type << "\n";}
};

class Book {
    int id;
    std::string isbn;
    std::string title;
    int pubYear;
    int numAvailableCopies;
    int nbrPages;
    language lang_instance;
    editor editor_instance;
    std::vector<author> authors;
    std::vector<categories> categor;
    std::vector<categories> genre;
public:
    Book() : id(0), isbn(""), title(""), pubYear(0), numAvailableCopies(0), nbrPages(0) {}
    Book(int _id, const std::string& _isbn, const std::string& _title, int _pubYear,
        int _numAvailableCopies, int _nbrPages, const language& _lang_instance,
        const editor& _editor_instance, const std::vector<author>& _authors,
        const std::vector<categories>& _categor, const std::vector<categories>& _genre)
        : id(_id), isbn(_isbn), title(_title), pubYear(_pubYear), numAvailableCopies(_numAvailableCopies),
        nbrPages(_nbrPages), lang_instance(_lang_instance), editor_instance(_editor_instance),
        authors(_authors), categor(_categor), genre(_genre) {
    }

    // Setters
    void setId(int _id) { id = _id; }
    void setIsbn(const std::string& _isbn) { isbn = _isbn; }
    void setTitle(const std::string& _title) { title = _title; }
    void setPubYear(int _pubYear) { pubYear = _pubYear; }
    void setNumAvailableCopies(int _num) { numAvailableCopies = _num; }
    void setNbrPages(int _nbr) { nbrPages = _nbr; }
    void setLanguage(const language& _lang) { lang_instance = _lang; }
    void setEditor(const editor& _editor) { editor_instance = _editor; }
    void setAuthors(const std::vector<author>& _authors) { authors = _authors; }
    void setCategories(const std::vector<categories>& _categor) { categor = _categor; }
    void setGenre(const std::vector<categories>& _genre) { genre = _genre; }

    // Getters
    int getId() const { return id; }
    std::string getIsbn() const { return isbn; }
    std::string getTitle() const { return title; }
    int getPubYear() const { return pubYear; }
    int getNumAvailableCopies() const { return numAvailableCopies; }
    int getNbrPages() const { return nbrPages; }
    language getLanguage() const { return lang_instance; }
    editor getEditor() const { return editor_instance; }
    std::vector<author> getAuthors() const { return authors; }
    std::vector<categories> getCategories() const { return categor; }
    std::vector<categories> getGenre() const { return genre; }

    // Méthode d'affichage
    void display() const {
        std::cout << "Book ID: " << id << "\nISBN: " << isbn << "\nTitle: " << title
            << "\nPublication Year: " << pubYear << "\nAvailable Copies: "
            << numAvailableCopies << "\nNumber of Pages: " << nbrPages << "\n";
        lang_instance.display();
        editor_instance.display();
        std::cout << "Authors: \n";
        for (const auto& author : authors) author.display();
        std::cout << "Categories: \n";
        for (const auto& cat : categor) cat.display();
        std::cout << "Genres: \n";
        for (const auto& gen : genre) gen.display();
    }
};

class Borrowing {
    int id;
    std::string dateBorrowed;
    std::string dateIntendedReturn;
    std::string dateActualReturn;
    std::string status;
    User client;
    User librarian;
    Book book;
public:
    Borrowing() : id(0), dateBorrowed(""), dateIntendedReturn(""), dateActualReturn(""), status("") {}
    Borrowing(int _id, const std::string& _dateBorrowed, const std::string& _dateIntendedReturn,
        const std::string& _dateActualReturn, const std::string& _status, const User& _client,
        const User& _librarian, const Book& _book)
        : id(_id), dateBorrowed(_dateBorrowed), dateIntendedReturn(_dateIntendedReturn),
        dateActualReturn(_dateActualReturn), status(_status), client(_client),
        librarian(_librarian), book(_book) {
    }
    // Setters
    void setId(int _id) { id = _id; }
    void setDateBorrowed(const std::string& _date) { dateBorrowed = _date; }
    void setDateIntendedReturn(const std::string& _date) { dateIntendedReturn = _date; }
    void setDateActualReturn(const std::string& _date) { dateActualReturn = _date; }
    void setStatus(const std::string& _status) { status = _status; }
    void setClient(const User& _client) { client = _client; }
    void setLibrarian(const User& _librarian) { librarian = _librarian; }
    void setBook(const Book& _book) { book = _book; }
    // Getters
    int getId() const { return id; }
    std::string getDateBorrowed() const { return dateBorrowed; }
    std::string getDateIntendedReturn() const { return dateIntendedReturn; }
    std::string getDateActualReturn() const { return dateActualReturn; }
    std::string getStatus() const { return status; }
    User getClient() const { return client; }
    User getLibrarian() const { return librarian; }
    Book getBook() const { return book; }
    // Méthode d'affichage
    void display() const {
        std::cout << "Borrowing ID: " << id << "\nDate Borrowed: " << dateBorrowed
            << "\nIntended Return Date: " << dateIntendedReturn
            << "\nActual Return Date: " << dateActualReturn
            << "\nStatus: " << status << "\n";
        std::cout << "Client:\n";
        client.display();
        std::cout << "Librarian:\n";
        librarian.display();
        std::cout << "Book:\n";
        book.display();
    }
};

class Library {
    std::vector<User> members;
    std::vector<User> librarians;
    std::vector<User> admins;
    std::vector<Book> books;
    std::vector<Borrowing> borrowings;
};