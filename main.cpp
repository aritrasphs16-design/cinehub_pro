#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

// ══════════════════════════════════════════════════════════════
// CLASS 1: MOVIE
// ══════════════════════════════════��═══════════════════════════
class Movie {
private:
    int movieId;
    string movieName;
    string genre;
    float rating;
    int totalSeats;
    int availableSeats;

public:
    Movie() : movieId(0), movieName(""), genre(""), rating(0.0), totalSeats(0), availableSeats(0) {}
    
    Movie(int id, string name, string gen, float rate, int seats) 
        : movieId(id), movieName(name), genre(gen), rating(rate), totalSeats(seats), availableSeats(seats) {}

    int getMovieId() const { return movieId; }
    string getMovieName() const { return movieName; }
    string getGenre() const { return genre; }
    float getRating() const { return rating; }
    int getTotalSeats() const { return totalSeats; }
    int getAvailableSeats() const { return availableSeats; }

    void setAvailableSeats(int seats) { availableSeats = seats; }

    bool bookSeats(int numSeats) {
        if (numSeats > availableSeats) return false;
        availableSeats -= numSeats;
        return true;
    }

    void cancelSeats(int numSeats) {
        availableSeats += numSeats;
    }

    void displayMovie() const {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║ Movie ID: " << movieId << endl;
        cout << "║ Name: " << movieName << endl;
        cout << "║ Genre: " << genre << endl;
        cout << "║ Rating: ⭐ " << rating << "/5" << endl;
        cout << "║ Available Seats: " << availableSeats << "/" << totalSeats << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
    }
};

// ══════════════════════════════════════════════════════════════
// CLASS 2: BOOKING
// ══════���═══════════════════════════════════════════════════════
class Booking {
private:
    int bookingId;
    int movieId;
    string customerName;
    string email;
    int numSeats;
    float totalAmount;
    string bookingDate;
    string status;

public:
    Booking() : bookingId(0), movieId(0), customerName(""), email(""), numSeats(0), totalAmount(0.0), status("") {}
    
    Booking(int bId, int mId, string name, string mail, int seats, float amount, string date)
        : bookingId(bId), movieId(mId), customerName(name), email(mail), 
          numSeats(seats), totalAmount(amount), bookingDate(date), status("Confirmed") {}

    int getBookingId() const { return bookingId; }
    int getMovieId() const { return movieId; }
    string getCustomerName() const { return customerName; }
    string getEmail() const { return email; }
    int getNumSeats() const { return numSeats; }
    float getTotalAmount() const { return totalAmount; }
    string getBookingDate() const { return bookingDate; }
    string getStatus() const { return status; }

    void cancelBooking() {
        status = "Cancelled";
    }

    void displayBooking() const {
        cout << "\n┌─────────────────────────────────────┐" << endl;
        cout << "│ Booking ID: " << bookingId << endl;
        cout << "│ Movie ID: " << movieId << endl;
        cout << "│ Customer: " << customerName << endl;
        cout << "│ Email: " << email << endl;
        cout << "│ Seats: " << numSeats << endl;
        cout << "│ Amount: ₹" << totalAmount << endl;
        cout << "│ Date: " << bookingDate << endl;
        cout << "│ Status: " << status << endl;
        cout << "└─────────────────────────────────────┘" << endl;
    }

    string toFileFormat() const {
        return to_string(bookingId) + "|" + to_string(movieId) + "|" + customerName + "|" + 
               email + "|" + to_string(numSeats) + "|" + to_string(totalAmount) + "|" + bookingDate + "|" + status;
    }
};

// ══════════════════════════════════════════════════════════════
// BOOKING MANAGEMENT SYSTEM
// ══════════════════════════════════════════════════════════════
class BookingSystem {
private:
    vector<Movie> movies;
    vector<Booking> bookings;
    int bookingIdCounter;
    const string MOVIES_FILE = "movies.dat";
    const string BOOKINGS_FILE = "bookings.dat";
    const float TICKET_PRICE = 200.0f;

public:
    BookingSystem() : bookingIdCounter(1000) {
        loadMovies();
        loadBookings();
    }

    // FUNCTION 1: Add Movie
    void addMovie() {
        cout << "\n═══ ADD NEW MOVIE ═══" << endl;
        
        int id;
        cout << "Enter Movie ID: ";
        cin >> id;
        cin.ignore();

        if (findMovie(id) != -1) {
            cout << "❌ Movie ID already exists!" << endl;
            return;
        }

        string name, genre;
        float rating;
        int seats;

        cout << "Enter Movie Name: ";
        getline(cin, name);

        cout << "Enter Genre: ";
        getline(cin, genre);

        cout << "Enter Rating (0-5): ";
        cin >> rating;

        if (rating < 0 || rating > 5) {
            cout << "❌ Rating must be between 0-5!" << endl;
            return;
        }

        cout << "Enter Total Seats: ";
        cin >> seats;

        if (seats <= 0) {
            cout << "❌ Seats must be positive!" << endl;
            return;
        }

        Movie movie(id, name, genre, rating, seats);
        movies.push_back(movie);
        saveMovies();
        
        cout << "✅ Movie added successfully!" << endl;
    }

    // FUNCTION 2: Book Movie Tickets
    void bookTickets() {
        cout << "\n═══ BOOK TICKETS ═══" << endl;
        
        if (movies.empty()) {
            cout << "��� No movies available!" << endl;
            return;
        }

        displayAllMovies();

        int movieId;
        cout << "\nEnter Movie ID to book: ";
        cin >> movieId;
        cin.ignore();

        int movieIndex = findMovie(movieId);
        if (movieIndex == -1) {
            cout << "❌ Movie not found!" << endl;
            return;
        }

        string name, email;
        cout << "Enter Customer Name: ";
        getline(cin, name);

        if (name.empty() || name.length() < 3) {
            cout << "❌ Name must be at least 3 characters!" << endl;
            return;
        }

        cout << "Enter Email: ";
        getline(cin, email);

        if (email.find('@') == string::npos) {
            cout << "❌ Invalid email format!" << endl;
            return;
        }

        int numSeats;
        cout << "Enter Number of Seats: ";
        cin >> numSeats;

        if (numSeats <= 0 || numSeats > movies[movieIndex].getAvailableSeats()) {
            cout << "❌ Invalid number of seats!" << endl;
            return;
        }

        if (movies[movieIndex].bookSeats(numSeats)) {
            float totalAmount = numSeats * TICKET_PRICE;
            
            time_t now = time(0);
            struct tm* timeinfo = localtime(&now);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%d/%m/%Y", timeinfo);
            string date(buffer);

            Booking booking(++bookingIdCounter, movieId, name, email, numSeats, totalAmount, date);
            bookings.push_back(booking);
            
            saveMovies();
            saveBookings();

            cout << "\n✅ BOOKING CONFIRMED!" << endl;
            cout << "Booking ID: " << bookingIdCounter << endl;
            cout << "Total Amount: ₹" << totalAmount << endl;
            cout << "Confirmation sent to: " << email << endl;
        }
    }

    // FUNCTION 3: View All Bookings
    void viewBookings() {
        cout << "\n═══ ALL BOOKINGS ═══" << endl;
        
        if (bookings.empty()) {
            cout << "❌ No bookings yet!" << endl;
            return;
        }

        cout << "\nTotal Bookings: " << bookings.size() << endl;
        for (const auto& booking : bookings) {
            booking.displayBooking();
        }
    }

    // FUNCTION 4: Cancel Booking
    void cancelBooking() {
        cout << "\n═══ CANCEL BOOKING ═══" << endl;
        
        int bookingId;
        cout << "Enter Booking ID to cancel: ";
        cin >> bookingId;

        int bookingIndex = findBooking(bookingId);
        if (bookingIndex == -1) {
            cout << "❌ Booking not found!" << endl;
            return;
        }

        int movieId = bookings[bookingIndex].getMovieId();
        int movieIndex = findMovie(movieId);
        
        if (movieIndex != -1) {
            movies[movieIndex].cancelSeats(bookings[bookingIndex].getNumSeats());
        }

        bookings[bookingIndex].cancelBooking();
        saveMovies();
        saveBookings();

        cout << "✅ Booking cancelled successfully!" << endl;
        cout << "Refund Amount: ₹" << bookings[bookingIndex].getTotalAmount() << endl;
    }

    // FUNCTION 5: Generate Report
    void generateReport() {
        cout << "\n═══ SYSTEM REPORT ═══" << endl;

        int totalMovies = movies.size();
        int totalBookings = bookings.size();
        int confirmedBookings = 0;
        int cancelledBookings = 0;
        float totalRevenue = 0.0f;
        int totalTicketsSold = 0;

        for (const auto& booking : bookings) {
            if (booking.getStatus() == "Confirmed") {
                confirmedBookings++;
                totalRevenue += booking.getTotalAmount();
                totalTicketsSold += booking.getNumSeats();
            } else {
                cancelledBookings++;
            }
        }

        cout << "\n📊 SYSTEM STATISTICS" << endl;
        cout << "├─ Total Movies: " << totalMovies << endl;
        cout << "├─ Total Bookings: " << totalBookings << endl;
        cout << "├─ Confirmed Bookings: " << confirmedBookings << endl;
        cout << "├─ Cancelled Bookings: " << cancelledBookings << endl;
        cout << "├─ Total Tickets Sold: " << totalTicketsSold << endl;
        cout << "├─ Total Revenue: ₹" << totalRevenue << endl;
        cout << "└─ Average Booking Value: ₹" << (confirmedBookings > 0 ? totalRevenue/confirmedBookings : 0) << endl;

        saveReport(totalMovies, totalBookings, confirmedBookings, cancelledBookings, totalRevenue);
    }

    void displayAllMovies() const {
        cout << "\n═══ AVAILABLE MOVIES ═══" << endl;
        
        if (movies.empty()) {
            cout << "❌ No movies available!" << endl;
            return;
        }

        for (const auto& movie : movies) {
            movie.displayMovie();
        }
    }

    int findMovie(int movieId) const {
        for (int i = 0; i < movies.size(); i++) {
            if (movies[i].getMovieId() == movieId) {
                return i;
            }
        }
        return -1;
    }

    int findBooking(int bookingId) const {
        for (int i = 0; i < bookings.size(); i++) {
            if (bookings[i].getBookingId() == bookingId) {
                return i;
            }
        }
        return -1;
    }

    void saveMovies() {
        ofstream file(MOVIES_FILE);
        for (const auto& movie : movies) {
            file << movie.getMovieId() << "|"
                 << movie.getMovieName() << "|"
                 << movie.getGenre() << "|"
                 << movie.getRating() << "|"
                 << movie.getTotalSeats() << "|"
                 << movie.getAvailableSeats() << "\n";
        }
        file.close();
    }

    void loadMovies() {
        ifstream file(MOVIES_FILE);
        if (!file.is_open()) return;

        int id, total, available;
        string name, genre;
        float rating;
        char delimiter;

        while (file >> id >> delimiter) {
            getline(file, name, '|');
            getline(file, genre, '|');
            file >> rating >> delimiter;
            file >> total >> delimiter >> available;
            file.ignore();

            Movie movie(id, name, genre, rating, total);
            movie.setAvailableSeats(available);
            movies.push_back(movie);
        }
        file.close();
    }

    void saveBookings() {
        ofstream file(BOOKINGS_FILE);
        for (const auto& booking : bookings) {
            file << booking.toFileFormat() << "\n";
        }
        file.close();
    }

    void loadBookings() {
        ifstream file(BOOKINGS_FILE);
        if (!file.is_open()) return;

        int bookingId, movieId, numSeats;
        float amount;
        string name, email, date, status;
        char delimiter;

        while (file >> bookingId >> delimiter) {
            file >> movieId >> delimiter;
            getline(file, name, '|');
            getline(file, email, '|');
            file >> numSeats >> delimiter >> amount >> delimiter;
            getline(file, date, '|');
            getline(file, status);

            Booking booking(bookingId, movieId, name, email, numSeats, amount, date);
            if (status == "Cancelled") booking.cancelBooking();
            bookings.push_back(booking);

            if (bookingId >= bookingIdCounter) {
                bookingIdCounter = bookingId + 1;
            }
        }
        file.close();
    }

    void saveReport(int totalMovies, int totalBookings, int confirmed, int cancelled, float revenue) {
        ofstream file("report.txt");
        file << "═══════════════════════════════════════\n";
        file << "     BOOKING SYSTEM REPORT\n";
        file << "═══════════════════════════════════════\n\n";
        file << "Total Movies: " << totalMovies << "\n";
        file << "Total Bookings: " << totalBookings << "\n";
        file << "Confirmed Bookings: " << confirmed << "\n";
        file << "Cancelled Bookings: " << cancelled << "\n";
        file << "Total Revenue: ₹" << revenue << "\n";
        file.close();
    }
};

// ══════════════════════════════════════════════════════════════
// MAIN FUNCTION - MENU-DRIVEN SYSTEM
// ══════════════════════════════════════════════════════════════
int main() {
    BookingSystem system;
    int choice;

    cout << "\n╔════════════════════════════════════════════════════════╗" << endl;
    cout << "║    🎬 MOVIE THEATER BOOKING SYSTEM 🎬               ║" << endl;
    cout << "║    OOP-Based C++ Project                            ║" << endl;
    cout << "╚════════════════════════════════════════════════════════╝" << endl;

    do {
        cout << "\n┌────────────────────────────────────┐" << endl;
        cout << "│  MAIN MENU                         │" << endl;
        cout << "├────────────────────────────────────┤" << endl;
        cout << "│ 1. Add Movie                       │" << endl;
        cout << "│ 2. View All Movies                 │" << endl;
        cout << "│ 3. Book Tickets                    │" << endl;
        cout << "│ 4. View My Bookings                │" << endl;
        cout << "│ 5. Cancel Booking                  │" << endl;
        cout << "│ 6. Generate Report                 │" << endl;
        cout << "│ 7. Exit                            │" << endl;
        cout << "└────────────────────────────────────┘" << endl;

        cout << "\nEnter your choice (1-7): ";
        cin >> choice;

        switch (choice) {
            case 1:
                system.addMovie();
                break;
            case 2:
                system.displayAllMovies();
                break;
            case 3:
                system.bookTickets();
                break;
            case 4:
                system.viewBookings();
                break;
            case 5:
                system.cancelBooking();
                break;
            case 6:
                system.generateReport();
                break;
            case 7:
                cout << "\n✅ Thank you for using Movie Theater Booking System!" << endl;
                cout << "Goodbye! 🎬" << endl;
                return 0;
            default:
                cout << "\n❌ Invalid choice! Please try again." << endl;
        }
    } while (choice != 7);

    return 0;
}