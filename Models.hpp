#pragma once
#include <string>
#include <vector>
#include <iostream>

class User;
class Book;
class Borrowing;

// Base User class
enum class UserRole {
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
        id(id), email(email), firstName(firstName), lastName(lastName), role(role) {
    }
    User() : id(-1), email(""), firstName(""), lastName(""), role(UserRole::MEMBER) {}

    virtual ~User() = default;

    virtual bool canViewBooks() const { return true; }
    virtual bool canReserveBooks() const { return role == UserRole::MEMBER; }
    virtual bool canManageBooks() const { return role >= UserRole::LIBRARIAN; }
    virtual bool canManageMembers() const { return role >= UserRole::LIBRARIAN; }
    virtual bool canManageAllUsers() const { return role == UserRole::ADMIN; }
    virtual bool canViewStatistics() const { return role == UserRole::ADMIN; }

	int getId() const { return id; }
    std::string getEmail() const { return email; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getFullName() const { return firstName + " " + lastName; }
    UserRole getRole() const { return role; }

    static UserRole stringToUserRole(const std::string& role) {
        if (role == "member") return UserRole::MEMBER;
        if (role == "librarian") return UserRole::LIBRARIAN;
        if (role == "admin") return UserRole::ADMIN;
        throw std::invalid_argument("Invalid role");
    }

    static std::string userRoleToString(UserRole role) {
        switch (role) {
        case UserRole::MEMBER: return "member";
        case UserRole::LIBRARIAN: return "librarian";
        case UserRole::ADMIN: return "admin";
        default: throw std::invalid_argument("Invalid role");
        }
    }

    virtual void display() const {
        std::cout << "User Information:\n"
            << "ID: " << id << "\n"
            << "Email: " << email << "\n"
            << "First Name: " << firstName << "\n"
            << "Last Name: " << lastName << "\n"
            << "Role: " << userRoleToString(role) << "\n";
    }
};

class Author {
    int id;
    std::string firstName;
    std::string lastName;
    std::string dateOfBirth;
public:
    Author() : id(0), firstName(""), lastName(""), dateOfBirth("") {}
    Author(int id, const std::string& firstName, const std::string& lastName, const std::string& dateOfBirth)
        : id(id), firstName(firstName), lastName(lastName), dateOfBirth(dateOfBirth) {
    }

    void setFirstName(const std::string& firstName) { this->firstName = firstName; }
    void setLastName(const std::string& lastName) { this->lastName = lastName; }
    void setDateOfBirth(const std::string& dateOfBirth) { this->dateOfBirth = dateOfBirth; }

    int getId() const { return id; }
    std::string getFirstName() const { return firstName; }
    std::string getLastName() const { return lastName; }
    std::string getDateOfBirth() const { return dateOfBirth; }
    std::string getFullName() const { return firstName + " " + lastName; }

    void display() const {
        std::cout << "Author ID: " << id << "\nFirst Name: " << firstName
            << "\nLast Name: " << lastName << "\nDate of Birth: " << dateOfBirth << "\n";
    }
};

class Editor {
    int id;
    std::string name;
public:
    Editor() : id(0), name("") {}
    Editor(int id, const std::string& name) : id(id), name(name) {}

    void setId(int id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }

    int getId() const { return id; }
    std::string getName() const { return name; }

    void display() const {
        std::cout << "Editor ID: " << id << "\nName: " << name << "\n";
    }
};

class Language {
    int id;
    std::string name;
    char code[3];
public:
    Language() : id(0), name(""), code{ ' ', '\0' } {}
    Language(int id, const std::string& name, const char code[2]) : id(id), name(name) {
        this->code[0] = code[0];
        this->code[1] = code[1];
        this->code[2] = '\0';
    }

    void setId(int id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }
    void setCode(const char code[2]) {
        this->code[0] = code[0];
        this->code[1] = code[1];
        this->code[2] = '\0';
    }

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getCode() const { return std::string(code); }

    void display() const {
        std::cout << "Language ID: " << id << "\nName: " << name << "\nCode: " << code << "\n";
    }
};

class Category {
    int id;
    std::string name;
    std::string type;
public:
    Category() : id(0), name(""), type("") {}
    Category(int id, const std::string& name, const std::string& type)
        : id(id), name(name), type(type) {
    }

    void setId(int id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }
    void setType(const std::string& type) { this->type = type; }

    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }

    void display() const {
        std::cout << "Category ID: " << id << "\nName: " << name << "\nType: " << type << "\n";
    }
};

class Book {
    int id;
    std::string isbn;
    std::string title;
    int pubYear;
    int numAvailableCopies;
    int nbrPages;
    Language language;
    Editor editor;
    std::vector<Author> authors;
    std::vector<Category> categories;
    std::vector<Category> genres;
public:
    Book() : id(0), isbn(""), title(""), pubYear(0), numAvailableCopies(0), nbrPages(0) {}
    Book(int id, const std::string& isbn, const std::string& title, int pubYear,
        int numAvailableCopies, int nbrPages, const Language& language,
        const Editor& editor, const std::vector<Author>& authors,
        const std::vector<Category>& categories, const std::vector<Category>& genres)
        : id(id), isbn(isbn), title(title), pubYear(pubYear), numAvailableCopies(numAvailableCopies),
        nbrPages(nbrPages), language(language), editor(editor),
        authors(authors), categories(categories), genres(genres) {
    }

    void setId(int id) { this->id = id; }
    void setIsbn(const std::string& isbn) { this->isbn = isbn; }
    void setTitle(const std::string& title) { this->title = title; }
    void setPubYear(int pubYear) { this->pubYear = pubYear; }
    void setNumAvailableCopies(int num) { this->numAvailableCopies = num; }
    void setNbrPages(int nbr) { this->nbrPages = nbr; }
    void setLanguage(const Language& language) { this->language = language; }
    void setEditor(const Editor& editor) { this->editor = editor; }
    void setAuthors(const std::vector<Author>& authors) { this->authors = authors; }
    void setCategories(const std::vector<Category>& categories) { this->categories = categories; }
    void setGenres(const std::vector<Category>& genres) { this->genres = genres; }

    int getId() const { return id; }
    std::string getIsbn() const { return isbn; }
    std::string getTitle() const { return title; }
    int getPubYear() const { return pubYear; }
    int getNumAvailableCopies() const { return numAvailableCopies; }
    int getNbrPages() const { return nbrPages; }
    Language getLanguage() const { return language; }
    Editor getEditor() const { return editor; }
    std::vector<Author> getAuthors() const { return authors; }
    std::vector<Category> getCategories() const { return categories; }
    std::vector<Category> getGenres() const { return genres; }

    void display() const {
        std::cout << "Book ID: " << id << "\nISBN: " << isbn << "\nTitle: " << title
            << "\nPublication Year: " << pubYear << "\nAvailable Copies: "
            << numAvailableCopies << "\nNumber of Pages: " << nbrPages << "\n";
        language.display();
        editor.display();
        std::cout << "Authors: \n";
        for (const auto& author : authors) author.display();
        std::cout << "Categories: \n";
        for (const auto& category : categories) category.display();
        std::cout << "Genres: \n";
        for (const auto& genre : genres) genre.display();
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
    Borrowing(int id, const std::string& dateBorrowed, const std::string& dateIntendedReturn,
        const std::string& dateActualReturn, const std::string& status, const User& client,
        const User& librarian, const Book& book)
        : id(id), dateBorrowed(dateBorrowed), dateIntendedReturn(dateIntendedReturn),
        dateActualReturn(dateActualReturn), status(status), client(client),
        librarian(librarian), book(book) {
    }

    void setId(int id) { this->id = id; }
    void setDateBorrowed(const std::string& date) { this->dateBorrowed = date; }
    void setDateIntendedReturn(const std::string& date) { this->dateIntendedReturn = date; }
    void setDateActualReturn(const std::string& date) { this->dateActualReturn = date; }
    void setStatus(const std::string& status) { this->status = status; }
    void setClient(const User& client) { this->client = client; }
    void setLibrarian(const User& librarian) { this->librarian = librarian; }
    void setBook(const Book& book) { this->book = book; }

    int getId() const { return id; }
    std::string getDateBorrowed() { return dateBorrowed; }
    std::string getDateIntendedReturn() const { return dateIntendedReturn; }
    std::string getDateActualReturn() const { return dateActualReturn; }
    std::string getStatus() const { return status; }
    User getClient() const { return client; }
    User getLibrarian() const { return librarian; }
    Book getBook() const { return book; }

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

public:
    // Méthodes pour gérer les membres
    void addMember(const User& member) { members.push_back(member); }
    void removeMember(int memberId) {
        members.erase(std::remove_if(members.begin(), members.end(), [memberId](const User& member) {
            return member.getId() == memberId;
            }), members.end());
    }
    std::vector<User> getMembers() const { return members; }

    // Méthodes pour gérer les bibliothécaires
    void addLibrarian(const User& librarian) { librarians.push_back(librarian); }
    void removeLibrarian(int librarianId) {
        librarians.erase(std::remove_if(librarians.begin(), librarians.end(), [librarianId](const User& librarian) {
            return librarian.getId() == librarianId;
            }), librarians.end());
    }
    std::vector<User> getLibrarians() const { return librarians; }

    // Méthodes pour gérer les administrateurs
    void addAdmin(const User& admin) { admins.push_back(admin); }
    void removeAdmin(int adminId) {
        admins.erase(std::remove_if(admins.begin(), admins.end(), [adminId](const User& admin) {
            return admin.getId() == adminId;
            }), admins.end());
    }
    std::vector<User> getAdmins() const { return admins; }

    // Méthodes pour gérer les livres
    void addBook(const Book& book) { books.push_back(book); }
    void removeBook(int bookId) {
        books.erase(std::remove_if(books.begin(), books.end(), [bookId](const Book& book) {
            return book.getId() == bookId;
            }), books.end());
    }
    std::vector<Book> getBooks() const { return books; }

    // Méthodes pour gérer les emprunts
    void addBorrowing(const Borrowing& borrowing) { borrowings.push_back(borrowing); }
    void removeBorrowing(int borrowingId) {
        borrowings.erase(std::remove_if(borrowings.begin(), borrowings.end(), [borrowingId](const Borrowing& borrowing) {
            return borrowing.getId() == borrowingId;
            }), borrowings.end());
    }
    std::vector<Borrowing> getBorrowings() const { return borrowings; }

    // Méthode d'affichage
    void display() const {
        std::cout << "Library Members:\n";
        for (const auto& member : members) member.display();
        std::cout << "Library Librarians:\n";
        for (const auto& librarian : librarians) librarian.display();
        std::cout << "Library Admins:\n";
        for (const auto& admin : admins) admin.display();
        std::cout << "Library Books:\n";
        for (const auto& book : books) book.display();
        std::cout << "Library Borrowings:\n";
        for (const auto& borrowing : borrowings) borrowing.display();
    }
};
