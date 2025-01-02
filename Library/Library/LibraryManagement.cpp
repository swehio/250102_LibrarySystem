#include <iostream>
#include <vector>
#include<unordered_map>

using namespace std;

enum Choice {
	Quit,
	DisplayAllBook,
	SearchByTitle,
	SearchByAuthor,
	DisplayAllStock,
	Borrow,
	Return,
};

enum BorrowChoice {
	Title = 1,
	Author = 2,
};

class Book {
public:
	string title;
	string author;

	Book(string title, string author) : title(title), author(author) {
	}
};

class BorrowManager {
private:
	unordered_map<string, int> stock;
	int quantity = 3; //BorrowManager 생성할 때 정할 수 있으면
public:
	void initializeStock(Book book) {
		stock[book.title] = quantity;
	}

	void borrowBook(string title) {
		unordered_map<string, int>::iterator it = stock.find(title);
		if (it != stock.end()) {
			if (stock[title] > 0) {
				cout << title << "을 대여했습니다." << endl;
				--stock[title];
			}
			else
				cout << "재고가 없어 대여가 불가능합니다." << endl;
		}
	}

	void returnBook(string title) {
		unordered_map<string, int>::iterator it = stock.find(title);
		if (it != stock.end()) {
			if (stock[title] < quantity) {
				cout << title << "을 반납했습니다." << endl;
				++stock[title];
			}
			else
				cout << "재고가 가득 찼습니다." << endl;
		}
		else
			cout << "존재하지 않는 책입니다." << endl;
	}

	void displayAllStock() {
		cout << "\n도서관 내 모든 책의 재고입니다." << endl;
		for (auto it = stock.begin(); it != stock.end(); ++it) {
			cout << it->first << " : " << it->second << endl;
		}
		cout << endl;
	}
};

class BookManager {
private:
	vector<Book> books; //객체에 대한 벡터는 소멸될 때 각 객체의 소멸자를 호출
	BorrowManager* borrow;

	Book* findByTitle(string title) { //굳이 private으로 넣은 이유가 뭘까?
		//cout << "\nFinding By Title..." << endl;
		for (int i = 0; i < books.size(); ++i) {
			if (books[i].title == title) {
				return &books[i];
			}
		}
		return nullptr;
	}
	Book* findByAuthor(string author) {
		//cout << "\nFinding By Author..." << endl;
		for (int i = 0; i < books.size(); ++i) {
			if (books[i].author == author) {
				return &books[i];
				//auto book으로 반복하고 &book 반환했더니 에러 -> 지역 변수라 사라짐
			}
		}
		return nullptr;
	}

public:
	BookManager(BorrowManager* borrow) : borrow(borrow) {}

	void borrowBook() {
		int isTitle = 0;
		string input = "";

		while (isTitle != 1 && isTitle != 2) {
			cout << "제목으로 대여 시 1, 작가로 대여 시 2를 눌러주세요.";
			cin >> isTitle;
			switch ((BorrowChoice)isTitle) {
			case BorrowChoice::Title:
				cout << "대여할 책 제목을 입력해주세요.";
				cin >> input;
				if (findByTitle(input) != nullptr)
					borrow->borrowBook((*findByTitle(input)).title);
				else
					cout << "도서관에 해당 제목의 책이 없습니다." << endl;
				break;
			case BorrowChoice::Author:
				cout << "대여할 책 작가를 입력해주세요.";
				cin >> input;
				if (findByAuthor(input) != nullptr)
					borrow->borrowBook(getBookByAuthor(input)->title);
				else
					cout << "도서관에 해당 작가에 대한 책이 없습니다." << endl;
				break;
			default:
				cout << "잘못된 입력입니다." << endl;
				break;
			}
		}
	}

	void returnBook(string title) {
		borrow->returnBook(title);
	}

	void displayAllStock() {
		borrow->displayAllStock();
	}

	void addBook(string title, string author) {

		books.push_back(Book(title, author)); // new Book은 왜 안됨? -> 포인터로 받아야 하니까(벡터를 클래스로 선언했잖아)
		borrow->initializeStock(books.back());
	}

	void displayAllBooks() {
		cout << "도서관에 배치되어 있는 책입니다." << endl;
		for (auto book : books) {
			displayBook(book);
		}
	}

	void displayBook(const Book& book) {
		cout << "|| Title: " << book.title << " || Author: " << book.author << " ||" << endl;
	}

	Book* getBookByTitle(string title) {
		return findByTitle(title);
	}

	Book* getBookByAuthor(string author) {
		return findByAuthor(author);
	}

	~BookManager() {
		delete borrow;
	}
};

int main() {
	BookManager bookManager(new BorrowManager());
	int actionType = -1;
	string input = "";

	bookManager.addBook("책2", "작가1");
	bookManager.addBook("책3", "작가2");
	bookManager.addBook("책4", "작가3");

	while (actionType != 0) {
		cout << "\n무엇을 하시나요? \n\n0 : 종료하기 \n1 : 도서관이 소지한 책 전부 보기 \n2 : 책 제목으로 검색하기 \n3 : 작가로 검색하기\n4 : 전체 책 재고 확인하기 \n5 : 대여하기 \n6 : 반납하기 \n\n입력: ";
		cin >> actionType;
		switch ((Choice)actionType) {
		case Choice::Quit:
			break;
		case Choice::DisplayAllBook:
			bookManager.displayAllBooks();
			break;
		case Choice::SearchByTitle:
			cout << "제목을 입력해주세요. ";
			cin >> input;
			if (bookManager.getBookByTitle(input) != nullptr)
				bookManager.displayBook(*bookManager.getBookByTitle(input));
			else
				cout << "존재하지 않습니다." << endl;
			break;
		case Choice::SearchByAuthor:
			cout << "작가를 입력해주세요. ";
			cin >> input;
			if (bookManager.getBookByAuthor(input) != nullptr)
				bookManager.displayBook(*bookManager.getBookByAuthor(input));
			else
				cout << "존재하지 않습니다." << endl;
			break;
		case Choice::DisplayAllStock:
			bookManager.displayAllStock();
			break;
		case Choice::Borrow:
			bookManager.borrowBook();
			break;
		case Choice::Return:
			cout << "반납할 책의 제목을 입력해주세요.";
			cin >> input;
			bookManager.returnBook(input);
			break;
		default:
			cout << "잘못된 입력입니다." << endl;
			break;
		}
	}
	return 0;
}