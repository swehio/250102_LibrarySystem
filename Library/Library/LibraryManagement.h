#pragma once
#pragma once
#include <iostream>
#include <vector>
#include<unordered_map>

using namespace std;

class Book {
public:
	string title;
	string author;

	Book(string title, string author) : title(title), author(author) {}
};

class BorrowManager {
private:
	unordered_map<string, int> stock;
	int quantity = 3; //BorrowManager 생성할 때 정할 수 있으면

public:
	void initializeStock(Book book);
	void borrowBook(string title);
	void returnBook(string title);
	void displayAllStock();

};

class BookManager {
private:
	vector<Book> books; //객체에 대한 벡터는 소멸될 때 각 객체의 소멸자를 호출
	BorrowManager* borrow;

	Book* findByTitle(string title);
	Book* findByAuthor(string title);

public:
	BookManager(BorrowManager* borrow) : borrow(borrow) {}
	void borrowBook();
	void returnBook(string title);
	void displayAllStock();
	void addBook(string title, string author);
	void displayAllBooks();
	void displayBook(const Book& book);
	Book* getBookByTitle(string title);
	Book* getBookByAuthor(string author);
	~BookManager() { delete borrow; }
};