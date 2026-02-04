

# 🛡️ Elite Car Rental Management System

A high-performance, **Object-Oriented (OOP)** fleet management solution built with **C++17** and **SQLite3**. This system provides a robust infrastructure for managing car inventories, processing rentals with automated billing, and generating financial analytics.

---

## 🚀 Key Features

* **Relational Persistence:** Uses **SQLite3** for ACID-compliant data storage. No more losing data in JSON files.
* **Modular Architecture:** Strictly follows the **MVC (Model-View-Controller)** pattern across a multi-file structure.
* **Smart Billing Engine:** Automatically calculates rental durations and total bills using `std::chrono` and `ctime` logic.
* **Transaction Safety:** Implements `BEGIN TRANSACTION` and `COMMIT` logic to ensure data integrity during rentals.
* **Business Analytics:** Built-in reporting module to track revenue and fleet utilization by category.
* **Input Validation:** Robust Regex-based sanitization for license plates, phone numbers, and dates.

---

## 🏗️ Project Structure

The project is organized into logical directories to separate concerns:

```text
.
├── codes/               # Core Implementation (.cpp files)
│   ├── DatabaseManager.cpp
│   ├── RentalService.cpp
│   └── ...
├── include/             # Header Definitions (.h files)
│   ├── Global.h
│   ├── RentalService.h
│   └── ...

```

---

## 🛠️ Installation & Setup

### 1. Prerequisites

Ensure you have the SQLite3 development library installed on your system.

**For Ubuntu/Debian:**

```bash
sudo apt-get install libsqlite3-dev

```

**For MacOS (using Homebrew):**

```bash
brew install sqlite

```

### 2. Compilation

Use the provided `Makefile` for a clean, modular build:

```bash
make

```

### 3. Execution

Run the system directly from the binary folder:

```bash
make run

```

---

## 📊 Database Schema

The system manages two primary relational tables:

1. **Cars Table:** Tracks `Plate`, `Brand`, `Model`, `DailyRate`, `Status` (Available/Rented), and `Category`.
2. **Rents Table:** Logs `CustomerName`, `Contact`, `StartDate`, `EndDate`, and `TotalBill`, linked via the `Plate` foreign key.

-Engine" for car recommendations based on user history. This links your current code to your more advanced expertise!

**Would you like me to create the `.gitignore` file for you to ensure you don't accidentally push your `.db` or binary files to GitHub?**
