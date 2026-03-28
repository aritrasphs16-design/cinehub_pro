// ══════════════════════════════════════════════════════════════
// C++ BACKEND SYSTEM - JavaScript Implementation
// Movie Theater Booking System
// ══════════════════════════════════════════════════════════════

// ──────────────────────────────────────────────────────────────
// CLASS 1: MOVIE
// ──────────────────────────────────────────────────────────────
class Movie {
    constructor(id, name, genre, rating, movieTime) {
        this.movieId = id;
        this.movieName = name;
        this.genre = genre;
        this.rating = rating;
        this.movieTime = movieTime || '00:00';
        this.bookedSeats = [];
        // Fixed 50 seats: 10 Premium (A,B) + 30 Classic (C,D,E)
        this.totalSeats = 50;
        this.availableSeats = 50;
        this.premiumPrice = 300;
        this.classicPrice = 150;
    }

    bookSeats(seatList) {
        if (seatList.length > this.availableSeats) {
            return false;
        }
        this.availableSeats -= seatList.length;
        return true;
    }

    cancelSeats(numSeats) {
        this.availableSeats += numSeats;
    }

    addBookedSeat(seatNum) {
        if (!this.bookedSeats.includes(seatNum)) {
            this.bookedSeats.push(seatNum);
        }
    }

    removeBookedSeat(seatNum) {
        const idx = this.bookedSeats.indexOf(seatNum);
        if (idx > -1) {
            this.bookedSeats.splice(idx, 1);
        }
    }

    getBookedSeats() {
        return [...this.bookedSeats];
    }

    getSeatPrice(seatNum) {
        const row = seatNum.charAt(0);
        if (row === 'A' || row === 'B') {
            return this.premiumPrice;
        }
        return this.classicPrice;
    }

    calculateTotal(seatList) {
        let total = 0;
        seatList.forEach(seat => {
            total += this.getSeatPrice(seat);
        });
        return total;
    }

    getInfo() {
        return `${this.movieId}|${this.movieName}|${this.genre}|${this.rating}|${this.movieTime}|${this.availableSeats}|${this.premiumPrice}|${this.classicPrice}|${this.bookedSeats.join(',')}`;
    }
}

// ──────────────────────────────────────────────────────────────
// CLASS 2: BOOKING
// ──────────────────────────────────────────────────────────────
class Booking {
    constructor(bookingId, movieId, name, email, seats, totalAmount, date) {
        this.id = bookingId;
        this.movieId = movieId;
        this.name = name;
        this.email = email;
        this.seats = seats;
        this.total = totalAmount;
        this.datetime = date;
        this.status = "CONFIRMED";
        this.paymentMethod = "card";
    }

    cancelBooking() {
        this.status = "CANCELLED";
    }

    getInfo() {
        const seatsStr = Array.isArray(this.seats) ? this.seats.join(',') : this.seats;
        return `${this.id}|${this.movieId}|${this.name}|${this.email}|${seatsStr}|${this.total}|${this.datetime}|${this.status}|${this.paymentMethod}`;
    }
}

// ──────────────────────────────────────────────────────────────
// BOOKING SYSTEM CLASS - MAIN LOGIC
// ──────────────────────────────────────────────────────────────
class BookingSystem {
    constructor() {
        this.movies = [];
        this.bookings = [];
        this.bookingIdCounter = 1000;
        this.TAX_RATE = 0.05;
        this.dailyBookings = {
            "Mon": 0,
            "Tue": 0,
            "Wed": 0,
            "Thu": 0,
            "Fri": 0,
            "Sat": 0,
            "Sun": 0
        };
        this.loadData();
    }

    // FUNCTION 1: Add Movie
    addMovie(id, name, genre, rating, movieTime) {
        if (this.findMovie(id) !== -1) {
            return { success: false, error: "Movie ID already exists!" };
        }

        if (rating < 0 || rating > 5) {
            return { success: false, error: "Rating must be between 0-5!" };
        }

        const movie = new Movie(id, name, genre, rating, movieTime);
        this.movies.push(movie);
        this.saveData();

        return { success: true, message: "✅ Movie added successfully! (50 Fixed Seats)" };
    }

    // FUNCTION 2: Book Tickets
    bookTickets(movieId, name, email, seats, paymentMethod) {
        const movieIndex = this.findMovie(movieId);
        if (movieIndex === -1) {
            return { success: false, error: "Movie not found!" };
        }

        if (!name || name.trim().length < 3) {
            return { success: false, error: "Name must be at least 3 characters!" };
        }

        if (!email || email.indexOf('@') === -1) {
            return { success: false, error: "Invalid email format!" };
        }

        const numSeats = seats.length || 0;
        if (numSeats <= 0 || numSeats > this.movies[movieIndex].availableSeats) {
            return { success: false, error: "Invalid number of seats!" };
        }

        if (this.movies[movieIndex].bookSeats(seats)) {
            const subtotal = this.movies[movieIndex].calculateTotal(seats);
            const tax = Math.round(subtotal * this.TAX_RATE);
            const total = subtotal + tax;

            const date = new Date().toLocaleString();
            const bookingId = ++this.bookingIdCounter;

            const booking = new Booking(bookingId, movieId, name, email, seats, total, date);
            booking.paymentMethod = paymentMethod || 'card';

            seats.forEach(seat => {
                this.movies[movieIndex].addBookedSeat(seat);
            });

            this.bookings.push(booking);
            
            const dayName = this.getDayName(new Date());
            if (this.dailyBookings.hasOwnProperty(dayName)) {
                this.dailyBookings[dayName]++;
            }

            this.saveData();

            return {
                success: true,
                booking: booking,
                subtotal: subtotal,
                tax: tax,
                message: "✅ Booking Confirmed!"
            };
        }

        return { success: false, error: "Booking failed!" };
    }

    // FUNCTION 3: View All Bookings
    viewBookings() {
        return this.bookings;
    }

    // FUNCTION 4: Cancel Booking
    cancelBooking(bookingId) {
        const bookingIndex = this.findBooking(bookingId);
        if (bookingIndex === -1) {
            return { success: false, error: "Booking not found!" };
        }

        const booking = this.bookings[bookingIndex];
        const movieIndex = this.findMovie(booking.movieId);

        if (movieIndex !== -1) {
            this.movies[movieIndex].cancelSeats(booking.seats.length);
            booking.seats.forEach(seat => {
                this.movies[movieIndex].removeBookedSeat(seat);
            });
        }

        const refundAmount = booking.total;
        booking.cancelBooking();
        this.saveData();

        return {
            success: true,
            refundAmount: refundAmount,
            message: "✅ Booking cancelled successfully!"
        };
    }

    // FUNCTION 5: Generate Report
    generateReport() {
        const totalMovies = this.movies.length;
        const totalBookings = this.bookings.length;
        const confirmedBookings = this.bookings.filter(b => b.status === "CONFIRMED").length;
        const cancelledBookings = this.bookings.filter(b => b.status === "CANCELLED").length;

        const totalRevenue = this.bookings
            .filter(b => b.status === "CONFIRMED")
            .reduce((sum, b) => sum + b.total, 0);

        const totalTicketsSold = this.bookings
            .filter(b => b.status === "CONFIRMED")
            .reduce((sum, b) => sum + b.seats.length, 0);

        const avgBookingValue = confirmedBookings > 0
            ? Math.round(totalRevenue / confirmedBookings)
            : 0;

        return {
            totalMovies,
            totalBookings,
            confirmedBookings,
            cancelledBookings,
            totalRevenue,
            totalTicketsSold,
            avgBookingValue,
            dailyBookings: this.dailyBookings
        };
    }

    // Helper Functions
    findMovie(movieId) {
        return this.movies.findIndex(m => m.movieId === movieId);
    }

    findBooking(bookingId) {
        return this.bookings.findIndex(b => b.id === bookingId);
    }

    getBookedSeatsForMovie(movieId) {
        const movieIndex = this.findMovie(movieId);
        if (movieIndex !== -1) {
            return this.movies[movieIndex].getBookedSeats();
        }
        return [];
    }

    getAllBookedSeats() {
        const bookedSeats = [];
        this.bookings.forEach(booking => {
            if (booking.status === "CONFIRMED") {
                booking.seats.forEach(seat => {
                    if (!bookedSeats.includes(seat)) {
                        bookedSeats.push(seat);
                    }
                });
            }
        });
        return bookedSeats;
    }

    getAllMovies() {
        return this.movies;
    }

    getDayName(date) {
        const days = ["Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"];
        return days[date.getDay()];
    }

    saveData() {
        try {
            const moviesData = this.movies.map(m => m.getInfo());
            localStorage.setItem('cinehub_movies', JSON.stringify(moviesData));

            const bookingsData = this.bookings.map(b => b.getInfo());
            localStorage.setItem('cinehub_bookings', JSON.stringify(bookingsData));

            localStorage.setItem('cinehub_counter', this.bookingIdCounter);
            localStorage.setItem('cinehub_daily_bookings', JSON.stringify(this.dailyBookings));
        } catch (error) {
            console.error("Error saving data:", error);
        }
    }

    loadData() {
        try {
            const moviesData = JSON.parse(localStorage.getItem('cinehub_movies') || '[]');
            moviesData.forEach(data => {
                const parts = data.split('|');
                const [id, name, genre, rating, movieTime, available, premiumPrice, classicPrice, bookedSeatsStr] = parts;
                const movie = new Movie(parseInt(id), name, genre, parseFloat(rating), movieTime);
                movie.availableSeats = parseInt(available);
                movie.premiumPrice = parseInt(premiumPrice);
                movie.classicPrice = parseInt(classicPrice);
                
                if (bookedSeatsStr) {
                    const bookedSeats = bookedSeatsStr.split(',').map(s => s.trim()).filter(s => s);
                    movie.bookedSeats = bookedSeats;
                }
                
                this.movies.push(movie);
            });

            const bookingsData = JSON.parse(localStorage.getItem('cinehub_bookings') || '[]');
            bookingsData.forEach(data => {
                const parts = data.split('|');
                const [id, movieId, name, email, seatsStr, total, datetime, status, paymentMethod] = parts;
                const seats = seatsStr.split(',').map(s => s.trim());

                const booking = new Booking(
                    parseInt(id),
                    parseInt(movieId),
                    name,
                    email,
                    seats,
                    parseFloat(total),
                    datetime
                );
                booking.status = status;
                booking.paymentMethod = paymentMethod || 'card';
                this.bookings.push(booking);
            });

            const counter = localStorage.getItem('cinehub_counter');
            if (counter) this.bookingIdCounter = parseInt(counter);

            const dailyData = localStorage.getItem('cinehub_daily_bookings');
            if (dailyData) this.dailyBookings = JSON.parse(dailyData);
        } catch (error) {
            console.error("Error loading data:", error);
        }
    }

    resetAllData() {
        this.movies = [];
        this.bookings = [];
        this.bookingIdCounter = 1000;
        this.dailyBookings = {
            "Mon": 0,
            "Tue": 0,
            "Wed": 0,
            "Thu": 0,
            "Fri": 0,
            "Sat": 0,
            "Sun": 0
        };
        localStorage.removeItem('cinehub_movies');
        localStorage.removeItem('cinehub_bookings');
        localStorage.removeItem('cinehub_counter');
        localStorage.removeItem('cinehub_daily_bookings');
    }

    executeCommand(command) {
        const cmd = command.toLowerCase().trim();
        
        if (cmd === 'occupancy') {
            const movies = this.getAllMovies();
            let totalSeats = 0;
            let bookedSeats = 0;
            movies.forEach(m => {
                totalSeats += m.totalSeats;
                bookedSeats += (m.totalSeats - m.availableSeats);
            });
            const occupancy = totalSeats > 0 ? Math.round((bookedSeats / totalSeats) * 100) : 0;
            return `📊 Current Occupancy: ${occupancy}% (${bookedSeats}/${totalSeats} seats booked)`;
        }
        
        if (cmd === 'movies') {
            if (this.movies.length === 0) {
                return '❌ No movies available';
            }
            let result = '🎬 MOVIES:\n';
            this.movies.forEach(m => {
                result += `├─ [${m.movieId}] ${m.movieName} (${m.genre}) ⭐${m.rating} at ${m.movieTime}\n`;
                result += `│  └─ Premium: ₹${m.premiumPrice} | Classic: ₹${m.classicPrice} - ${m.availableSeats}/${m.totalSeats} seats\n`;
            });
            return result;
        }

        if (cmd === 'bookings') {
            if (this.bookings.length === 0) {
                return '❌ No bookings yet';
            }
            let result = '📋 BOOKINGS:\n';
            this.bookings.forEach(b => {
                result += `├─ [${b.id}] ${b.name} - Seats: ${b.seats.join(',')} - ₹${b.total} - ${b.status}\n`;
            });
            return result;
        }

        if (cmd === 'revenue') {
            const total = this.bookings
                .filter(b => b.status === "CONFIRMED")
                .reduce((sum, b) => sum + b.total, 0);
            return `💰 Total Revenue: ₹${total}`;
        }

        if (cmd === 'stats') {
            const report = this.generateReport();
            return `📊 STATS:
├─ Total Movies: ${report.totalMovies}
├─ Total Bookings: ${report.totalBookings}
├─ Confirmed: ${report.confirmedBookings}
├─ Cancelled: ${report.cancelledBookings}
├─ Revenue: ₹${report.totalRevenue}
├─ Tickets Sold: ${report.totalTicketsSold}
└─ Avg Booking: ₹${report.avgBookingValue}`;
        }

        if (cmd === 'daily') {
            let result = '📅 DAILY BOOKINGS:\n';
            Object.entries(this.dailyBookings).forEach(([day, count]) => {
                result += `├─ ${day}: ${count} bookings\n`;
            });
            return result;
        }

        if (cmd === 'help') {
            return `📖 AVAILABLE COMMANDS:
├─ occupancy     - Show current occupancy %
├─ movies        - List all movies with times
├─ bookings      - Show all bookings
├─ revenue       - Total revenue
├─ stats         - System statistics
├─ daily         - Daily bookings breakdown
├─ clear         - Clear console
└─ help          - Show this help`;
        }

        if (cmd === 'clear') {
            return 'CLEAR';
        }

        return `❌ Unknown command: "${command}". Type "help" for available commands.`;
    }
}

// Initialize global system
let bookingSystem = new BookingSystem();
console.log("✅ C++ Backend System Loaded!");