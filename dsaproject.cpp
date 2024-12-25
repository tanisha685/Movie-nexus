#include<bits/stdc++.h>
using namespace std;
class Movies {
public:
    string title;
    string releaseDate;
    double rating;
    string genre;
    string duration;
    Movies(string title, string releaseDate, double rating, string genre, string duration)
        : title(title), releaseDate(releaseDate), rating(rating), genre(genre), duration(duration) {}

};
class User {
public:
    string username;
    string password;
    User* next;

    User() : username(""), password(""), next(nullptr) {}
    User(string username, string password, User* head) {
        this->username = username;
        this->password = password;
        this->next = head;
    }
};
struct Booking {
    string seatCategory;
    int numSeats;
};


struct User1 {
    int userId;
    vector<Booking> bookings;
};


void displaySeatPrices(const unordered_map<string, double>& seatPrices) {
    cout << "Available seat categories and prices:\n";
    for (const auto& category : seatPrices) {
        cout << category.first << ": Rs " << category.second << " per seat\n";
    }
    cout << endl;
}


double calculateUserAmount(const vector<Booking>& bookings, unordered_map<string, double>& seatPrices) {
    double totalAmount = 0;

    for (const Booking& booking : bookings) {
        if (seatPrices.find(booking.seatCategory) != seatPrices.end()) {
            totalAmount += booking.numSeats * seatPrices[booking.seatCategory];
        } else {
            cout << "Error: Seat category " << booking.seatCategory << " not found.\n";
        }
    }

    int totalSeats = 0;
    for (const Booking& booking : bookings) {
        totalSeats += booking.numSeats;
    }

    if (totalSeats > 5) {
        totalAmount *= 0.9; // Apply 10% discount
        cout << "Discount applied! You booked " << totalSeats << " seats.\n";
    }


    double tax = totalAmount * 0.18;
    totalAmount += tax;
    cout << "GST (18%) applied: $" << tax << endl;

    return totalAmount;
}


double readRevenueFromFile(const string& filename) {
    ifstream file(filename);
    double totalRevenue = 0;

    if (file.is_open()) {
        file >> totalRevenue;
        file.close();
    }

    return totalRevenue;
}


void writeRevenueToFile(const string& filename, double totalRevenue) {
    ofstream file(filename);


    if (file.is_open()) {
        file << totalRevenue;
        file.close();
    } else {
        cout << "Error: Unable to open the file for writing.\n";
    }
}

struct Node {
    int seat;
    Node* left;
    Node* right;

    Node(int s) : seat(s), left(nullptr), right(nullptr) {}
};

// Class for managing booked seats using BST
class SeatTree {
public:
    Node* root;

    SeatTree() : root(nullptr) {}

    // Insert a seat into the BST
    Node* insert(Node* node, int seat) {
        if (node == nullptr) return new Node(seat);
        if (seat < node->seat)
            node->left = insert(node->left, seat);
        else if (seat > node->seat)
            node->right = insert(node->right, seat);
        return node;
    }

    // Search a seat in the BST
    bool search(Node* node, int seat) {
        if (node == nullptr) return false;
        if (node->seat == seat) return true;
        if (seat < node->seat)
            return search(node->left, seat);
        else
            return search(node->right, seat);
    }

    // Function to book a seat
    void bookSeat(int seat) {
        if (!search(root, seat)) {
            root = insert(root, seat);
            cout << "Seat " << seat << " has been booked." << endl;
        } else {
            cout << "Seat " << seat << " is already booked!" << endl;
        }
    }
};

// Class to manage the theater seats
class Theater {
    int rows, cols;
    int** seats;
    int goldenPrice, silverPrice;
    vector<User1> allUsers;  // Store all users and their bookings

public:
    Theater(int r, int c, int goldPrice, int silverPrice)
        : rows(r), cols(c), goldenPrice(goldPrice), silverPrice(silverPrice) {
        seats = new int*[rows];
        for (int i = 0; i < rows; ++i) {
            seats[i] = new int[cols];
            for (int j = 0; j < cols; ++j) {
                seats[i][j] = 1;  // 1 represents available seat
            }
        }
    }

    // Display the current seat availability with seat labels (O = Available, X = Booked)
    void displaySeats() {
        cout << "Seating arrangement (O = Available, X = Booked):\n";
        cout << "   ";  // Formatting for column headers
        for (int j = 0; j < cols; ++j) {
            cout << " " << char('A' + j) << " ";  // Display column labels (A, B, C, ...)
        }
        cout << endl;

        for (int i = 0; i < rows; ++i) {
            cout << setw(2) << (i + 1) << " ";  // Display row number
            for (int j = 0; j < cols; ++j) {
                cout << (seats[i][j] == 1 ? " O " : " X ");  // O = available, X = booked
            }
            cout << endl;
        }
    }

    // Book a seat for a user, with a seat category and pricing
    void bookSeat(int row, int col) {
        if (row >= rows || col >= cols || row < 0 || col < 0) {
            cout << "Invalid seat!" << endl;
            return;
        }

        if (seats[row][col] == 0) {  // Seat is already booked
            cout << "Seat " << (row + 1) << char('A' + col) << " is already booked!" << endl;
            return;
        }

        seats[row][col] = 0;  // Mark seat as booked
        unordered_map<string, double> seatPrices = {
            {"VIP", 2000.0},
            {"Regular", 980.0},
            {"Economy", 450.0},
            {"Balcony", 750.0}
        };

        char continueBooking = 'y';
        int userId = allUsers.size() + 1;

        User1 currentUser;
        currentUser.userId = userId;
        vector<Booking> currentBookings;

        while (continueBooking == 'y' || continueBooking == 'Y') {
            string seatCategory;
            int numSeats;

            cout << "Enter seat category you want to book (VIP/Regular/Economy/Balcony): ";
            cin >> seatCategory;
            cout << "Enter number of seats: ";
            cin >> numSeats;

            currentBookings.push_back({seatCategory, numSeats});

            cout << "Do you want to book more seats? (y/n): ";
            cin >> continueBooking;
        }

        currentUser.bookings = currentBookings;
        allUsers.push_back(currentUser);  // Add this user to the list of all users
        double amountToPay = calculateUserAmount(currentUser.bookings, seatPrices);
        cout << "\nAmount to be paid by User " << currentUser.userId << ": $" << amountToPay << endl;

    }

    // Calculate total price for the given bookings
    double calculateUserAmount(const vector<Booking>& bookings, const unordered_map<string, double>& seatPrices) {
        double total = 0.0;
        for (const auto& booking : bookings) {
            if (seatPrices.find(booking.seatCategory) != seatPrices.end()) {
                total += booking.numSeats * seatPrices.at(booking.seatCategory);
            }
        }
        return total;
    }

    // Display the total price of all booked seats across all users
    void showTotalPrice() {
        unordered_map<string, double> seatPrices = {
            {"VIP", 2000.0},
            {"Regular", 980.0},
            {"Economy", 450.0},
            {"Balcony", 750.0}
        };

        double totalRevenue = 0.0;
        for (const auto& user : allUsers) {
            totalRevenue += calculateUserAmount(user.bookings, seatPrices);
        }
        cout << "\nTotal Price of booked seats: $" << totalRevenue << endl;
    }

    // Destructor to clean up memory
    ~Theater() {
        for (int i = 0; i < rows; ++i) {
            delete[] seats[i];
        }
        delete[] seats;
    }
};

int book_ticket()
{
    int rows = 5, cols = 5;
    int goldenPrice = 150, silverPrice = 100;

    Theater theater(rows, cols, goldenPrice, silverPrice);
    int choice, row;
    char colChar;
    int col;

    while (true) {
        cout << "\n1. View Seats\n2. Book Seat\n3. Show Total Price\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                theater.displaySeats();
                break;
            case 2:
                cout << "Enter row (1-" << rows << ") and column (A-" << char('A' + cols - 1) << ") to book: ";
                cin >> row >> colChar;
                col = colChar - 'A';  // Convert column letter to index
                theater.bookSeat(row - 1, col);

                break;
            case 3:
                theater.showTotalPrice();
                break;
            case 4:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
void welcomeMessage() {
  cout << "" << endl;
  cout << "       Welcome to the Movie Ticket Booking System!" << endl;
  cout << "" << endl;
  cout << endl;
  cout << "Experience seamless movie ticket booking with our platform!" << endl;
  cout << endl;
  cout << "Features:" << endl;
  cout << "- Browse and Search Movies with Ease" << endl;
  cout << "- Quick and Secure User Registration and Login" << endl;
  cout << "- Real-time Seat Selection and Booking" << endl;
  cout << "- Efficient Booking History and Ticket Cancellation" << endl;
  cout << "- Get Personalized Movie Recommendations" << endl;
  cout << endl;
  cout << "Get ready to enjoy your movie experience like never before!" << endl;
  cout << endl;
  }
class movie
{
    public:
    string name;
    vector<string> showtimes;

    movie( string& name, vector<string> &showtimes)
    {
        this->name=name;
        this->showtimes=showtimes;
    }
    string getname() const
    {
        return name;
    }
    vector<string> getshowtime() const
    {
        return showtimes;
    }
};
class theatre
{
    public:
    string name;
    vector<movie> moviesobj;
    theatre(string& name, vector<movie>& moviesobj)
    {
        this->name=name;
        this->moviesobj=moviesobj;
    }
    string getname() const
    {
        return name;
    }
    vector<movie> getmovies() const
    {
        return moviesobj;
    }
};
class cities
{
    public:
    string name;
    vector<theatre> theatresobj;
    cities* left;
    cities* right;
    cities( string& name,vector<theatre>& theatresobj)
    {
         this->name=name;
         this->theatresobj=theatresobj;
         left=right=nullptr;
    }
    string getcityname() const
    {
        return name;
    }
    vector<theatre> gettheatres() const
    {
        return theatresobj;
    }
    cities* getleft() const
    {
        return left;
    }
    cities* getright() const
    {
        return right;
    }
    void setleft(cities* leftnode)
    {
        left = leftnode;
    }
    void setright(cities* rightnode)
    {
        right = rightnode;
    }
};
cities* insertcity(cities* root, string& name, vector<theatre>& theatresobj)
{
    if (root == nullptr)
    {
        return new cities(name, theatresobj);
    }
    if (name < root->getcityname())
    {
        root->setleft(insertcity(root->getleft(), name, theatresobj));
    }
    else if (name > root->getcityname())
    {
        root->setright(insertcity(root->getright(), name, theatresobj));
    }
    return root;
}
cities* searchcity(cities* root,  string& name)
{
    if (root==nullptr || root->getcityname()==name)
    {
        return root;
    }
    if (name<root->getcityname())
    {
        return searchcity(root->getleft(),name);
    }
    else
    {
        return searchcity(root->getright(),name);
    }
}
void displaytheatres( cities* root)
{
    if (root==nullptr)
    {
        cout << "City not found.\n";
        return;
    }
    cout << "----------------------------\n";
    cout << "     Welcome to " << root->getcityname();
    cout<<endl;
    cout << "----------------------------\n\n";
    for (const theatre& theatresobj : root->gettheatres())
    {
        cout << "🎬 Theatre: " << theatresobj.getname() << "\n";
        cout << "----------------------------\n";
        for (const movie& moviesobj : theatresobj.getmovies())
        {
            cout << "  🎥 Movie: " << moviesobj.getname() << "\n";
            cout << "  🕒 Showtimes:";
            vector<string> showtimes = moviesobj.getshowtime();
            for (size_t i = 0; i < showtimes.size(); ++i)
            {
                cout << showtimes[i];
                if (i < showtimes.size() - 1)
                {
                    cout << " | ";
                }
            }
            cout << "\n";
        }
        cout << "----------------------------\n\n";
    }
}
void showmovies(){
cout<<"1.Zindagi Na Milegi Dobara\t6.Dune\n2.Stree 2\t\t\t7.Pathaan\n3.The Dark Knight\t\t8.Jawan\n4.Interstellar\t\t\t9.Avengers: Endgame\n5.Inception\t\t\t10.Spider-Man: No Way Home"<<endl;
}
class UserList {
    User* head;
    User* loggedInUser;  // Track the currently logged-in user

public:
    UserList() : head(nullptr), loggedInUser(nullptr) {}

    // Register a new user
    void registerUser(string username, string password,vector<Movies> &movieList) {
        if (findUser(username)) {
            cout << "Username already taken. Please choose another one.\n";
            return;
        }

        User* newUser = new User(username, password, head);
        head = newUser;
        cout << "User " << username << " registered successfully!\n";
        userDashboard(username,movieList); // Redirect to user dashboard after registration
    }

    bool findUser(string username) {
        User* temp = head;
        while (temp != nullptr) {
            if (temp->username == username) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    bool loginUser(string username, string password) {
        User* temp = head;
        while (temp != nullptr) {
            if (temp->username == username && temp->password == password) {
                cout << "Login successful! Welcome, " << username << "!\n";
                loggedInUser = temp;  // Set the logged-in user
                return true;
            }
            temp = temp->next;
        }
        cout << "Login failed! Incorrect username or password.\n";
        return false;
    }

    void changePassword() {
        if (!isLoggedIn()) {
            cout << "You must log in first to change your password.\n";
            return;
        }

        string newPassword;
        cout << "Enter your new password: ";
        cin >> newPassword;
        loggedInUser->password = newPassword;
        cout << "Password changed successfully!\n";
    }

    void logoutUser() {
        if (!isLoggedIn()) {
            cout << "No user is currently logged in.\n";
            return;
        }
        cout << "User " << loggedInUser->username << " has logged out.\n";
        loggedInUser = nullptr;
    }

    void deleteAccount() {
        if (!isLoggedIn()) {
            cout << "You must log in first to delete your account.\n";
            return;
        }

        string confirm;
        cout << "Are you sure you want to delete your account? (yes/no): ";
        cin >> confirm;
        if (confirm == "yes") {
            User* temp = head;
            User* prev = nullptr;

            // Traverse to find the user in the linked list
            while (temp != nullptr && temp != loggedInUser) {
                prev = temp;
                temp = temp->next;
            }

            // Delete user node
            if (prev == nullptr) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }

            delete temp;
            loggedInUser = nullptr;
            cout << "Account deleted! We will miss you...\n";
        } else {
            cout << "Account deletion canceled.\n";
        }
    }

    void displayUsers() {
        User* temp = head;
        cout << "\nRegistered Users:\n";
        while (temp != nullptr) {
            cout << "Username: " << temp->username << endl;
            temp = temp->next;
        }
    }

    bool isLoggedIn() {
        return loggedInUser != nullptr;
    }

    bool validateInput(string username, string password) {
        if (username.length() < 3) {
            cout << "Username should be at least 3 characters long.\n";
            return false;
        }
        if (password.length() < 5) {
            cout << "Password should be at least 5 characters long.\n";
            return false;
        }
        return true;
    }

    void userDashboard(string username,vector<Movies> &movieList) {
        int choice;
        do {
            cout << "\n--- User Dashboard ---\n";
            cout << "1. Change Password\n";
            cout << "2. Search Movies\n";
            cout << "3. Book Ticket\n";
            cout << "4. Logout\n";
            cout << "5. Exit Dashboard\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    changePassword();
                    break;
                case 2:
                    cout<<"MOVIES SHOWING RIGHT NOW:"<<endl;
                    showmovies();
                    searchMovies(movieList);
                    break;
                case 3:
                    bookTicket(movieList);
                    book_ticket();
                    break;
                case 4:
                    logoutUser();
                    return; // Exit dashboard on logout
                case 5:
                    return; // Exit dashboard
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (true);
    }

   void searchMovies(vector<Movies>& movieList) {
    string movieName;
    cout << "Enter movie name to search: ";
    getline(cin >> ws, movieName); // Using ws to ignore leading whitespace

    cout << "\n--- Search Results ---\n";
    bool found = false; // Flag to check if any movies were found
    for (const auto& movie : movieList) {
        if (movie.title.find(movieName) != string::npos) {
            cout << movie.title << " - " << movie.genre
                 << " - " << movie.releaseDate
                 << " - Rating: " << movie.rating << "\n";
            found = true; // Set found to true if a match is found
        }
    }
    if (!found) {
        cout << "No movies found matching your search.\n"; // Message if no movies found
    }
}


    void bookTicket(vector<Movies>& movieList) {
        string movieName;
        cout << "Enter the name of the movie you want to book a ticket for: ";
        cin.ignore(); // Ignore any leftover newline characters
        getline(cin, movieName);

        // Check if the movie exists in the list
        for (const auto& m : movieList) {
            if (m.title == movieName) {
                cout << "Ticket booking for " << movieName ;
                return;
            }
        }
        cout << "Movie not found. Please check the name and try again.\n";
    }

    void showRecommendedMovies() {
        vector<string> movies = {
            "Zindagi Na Milegi Dobara",
            "Stree 2",
            "The Dark Knight",
            "Interstellar",
            "Inception",
            "Dune",
            "Jawan",
            "Avengers: Endgame",
            "Spider-Man: No Way Home",
            "Pathaan"
        };

        for (const auto& movie : movies) {
            cout << "- " << movie << endl;
        }
    }
};

void showMenu() {
    cout << "\n--- User Login and Registration System ---\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Change Password\n";
    cout << "4. Delete Account\n";
    cout << "5. Show All Users (Admin Feature)\n";
    cout << "6. Logout\n";
    cout << "7. Exit\n";
    cout << "Enter your choice: ";
}
void addmovies(vector<Movies> &movieList){
      movieList.push_back(Movies("Zindagi Na Milegi Dobara", "2011-07-15", 8.2, "Drama/Adventure", "2h 30m"));
    movieList.push_back(Movies("Stree 2", "2023-08-31", 7.5, "Horror/Comedy", "2h 10m"));
    movieList.push_back(Movies("The Dark Knight", "2008-07-18", 9.0, "Action/Crime", "2h 32m"));
    movieList.push_back(Movies("Interstellar", "2014-11-07", 8.6, "Sci-Fi/Drama", "2h 49m"));
    movieList.push_back(Movies("Inception", "2010-07-16", 8.8, "Sci-Fi/Thriller", "2h 28m"));
    movieList.push_back(Movies("Dune", "2021-10-22", 8.0, "Sci-Fi/Adventure", "2h 35m"));
    movieList.push_back(Movies("Pathaan", "2023-01-25", 6.5, "Action/Thriller", "2h 26m"));
    movieList.push_back(Movies("Jawan", "2023-09-07", 7.8, "Action/Drama", "2h 25m"));
    movieList.push_back(Movies("Avengers: Endgame", "2019-04-26", 8.4, "Action/Sci-Fi", "3h 1m"));
    movieList.push_back(Movies("Spider-Man: No Way Home", "2021-12-17", 8.6, "Action/Adventure", "2h 28m"));


}
int main() {

    UserList userList;  // Create UserList object to manage users
vector<Movies> movieList;
    int choice;
    string username, password;
addmovies(movieList);

    do {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                do {
                    cout << "Enter username to register: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                     welcomeMessage();
                } while (!userList.validateInput(username, password));
                userList.registerUser(username, password,movieList);
                break;

            case 2:
                cout << "Enter username to login: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                userList.loginUser(username, password);
                break;

            case 3:
                userList.changePassword();
                book_ticket();
                break;

            case 4:
                userList.deleteAccount();
                break;

            case 5:
                userList.displayUsers();
                break;

            case 6:
                userList.logoutUser();
                break;

            case 7:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Choice went out of bounds. Try again!\n";
        }

    } while (choice != 7);

    return 0;
}
