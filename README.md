# cinehub_pro
# 🎬 CineHub Pro - Single Screen Cinema Booking System

A modern, responsive web application for managing cinema bookings with **real-time seat selection**, **date+time collision detection**, and **dynamic occupancy tracking**.

## ✨ Features

### 🎫 Core Booking System
- **Movie Management**: Add unlimited movies with custom screening times
- **Smart Seat Layout**: 50-seat theater with Premium (A-B rows) and Classic (C-E rows) categories
- **Real-time Seat Selection**: Click to select/deselect seats with instant price calculation
- **Date & Time Selection**: Browse available dates and time slots
- **Price Categories**: Different pricing for premium and classic seats

### 🛡️ Collision Detection
- **Date+Time Specific**: Prevents double-booking of the same time slot on the same date
- **Smart Blocking**: Automatically blocks conflicting time slots
- **Single Screen**: Enforces single theater constraint - only one movie per time slot

### 📊 Analytics & Reporting
- **Live Occupancy Tracking**: Real-time occupancy percentage calculation
- **Slot Analytics**: View occupancy for each movie slot
- **Daily Bookings Chart**: Visual bar chart of bookings by day
- **System Report**: Total movies, bookings, revenue, and occupancy metrics

### 💳 Payment & Receipts
- **Multiple Payment Methods**: Card, UPI, Digital Wallet, Net Banking
- **Digital Receipts**: Full booking details with price breakdown
- **Print Option**: Print receipts directly from browser
- **Email Ready**: Receipt contains all necessary booking information

### 💾 Data Persistence
- **LocalStorage**: All data saves automatically
- **Auto-Load**: Previous bookings and movies load on refresh
- **No Backend Required**: Works completely offline after first load

### 🎨 Modern UI/UX
- **Dark Theme**: Eye-friendly dark interface
- **Responsive Design**: Works on desktop, tablet, and mobile
- **Smooth Animations**: Professional transitions and effects
- **Real-time Updates**: Instant feedback for all actions

---

## 🚀 Quick Start

### Installation

#### Method 1: Direct Download
1. Download `index.html` and `cpp-backend.js`
2. Place both files in same folder
3. Open `index.html` in browser
4. Done! ✅

#### Method 2: GitHub Pages
1. Fork this repository
2. Go to Settings → Pages
3. Enable GitHub Pages
4. Visit: `https://your-username.github.io/cinehub-pro/`

#### Method 3: Local Server
```bash
# Using Python
python -m http.server 8000

# Using Node.js
npx http-server

# Open: http://localhost:8000
