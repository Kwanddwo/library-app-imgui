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
};

class Book {
    int id;
    std::string isbn;
    std::string title;
    int pubYear;
    int numAvailableCopies;
    int nbrPages;
    int languageId;
    int editorId;
};

class Borrowing {
    int id;
    std::string dateBorrowed;
    std::string dateIntendedReturn;
    std::string dateActualReturn;
    std::string status;
    int clientId;
    int librarianId;
    int bookId;
};

class Library {
    std::vector<User> members;
    std::vector<User> librarians;
    std::vector<User> admins;
    std::vector<Book> books;
    std::vector<Borrowing> borrowings;
};