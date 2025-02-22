# 🏪 Inventory Management System

## 📌 Overview
This project is a **simple Inventory Management System** using **POSIX message queues** for **inter-process communication (IPC)**. It allows customers to place orders, updates inventory automatically, and enables shopkeepers to restock or modify item details in real-time.

## ✨ Features
- ✅ Customers can place orders and receive real-time approval or rejection.
- 🔄 Inventory updates dynamically when stock is low.
- 📦 Shopkeepers can update stock quantities and modify prices.
- 📜 Logs all orders and inventory updates.
- 💬 Uses **POSIX message queues (IPC)** for seamless communication between processes.

---

## 📂 Project Structure
```
├── inventory_manager.c   # Handles inventory and order processing
├── customer.c            # Customer interface for ordering items
├── shopkeeper.c          # Shopkeeper interface for updating inventory
├── common.h              # Common definitions and structures
├── inventory.txt         # Stores inventory data
├── orders.log            # Logs processed orders
├── README.md             # Project documentation
```

---

## ⚙️ Installation & Compilation
### 📌 Prerequisites
- 🖥️ Linux-based system
- 🔧 GCC compiler
- 📨 POSIX message queues support

### 🛠️ Compilation
Run the following commands to compile the project:
```bash
gcc -o inventory_manager inventory_manager.c -lrt -pthread
gcc -o customer customer.c -lrt -pthread
gcc -o shopkeeper shopkeeper.c -lrt -pthread
```

---

## 🚀 Usage
### 1️⃣ Running the **Inventory Manager (Server)**
```bash
./inventory_manager
```
This starts the inventory system, processing orders and handling stock updates.

### 2️⃣ Running a **Customer Process**
```bash
./customer
```
This allows a customer to **view inventory and place orders**.

### 3️⃣ Running a **Shopkeeper Process**
```bash
./inventory_managment
```
The shopkeeper can **update stock levels and change prices**.

---

## 📜 Logs & Inventory Management
- **Inventory Data** is stored in `inventory.txt`
- **Orders** are logged in `orders.log`

To manually clean message queues:
```bash
mq_unlink("/customer_queue");
mq_unlink("/shop_queue");
```

---

## 📝 Example Outputs
### 📌 Viewing Inventory
**Command:**
```bash
./customer
```
**Output:**
```
🛒 Available Items in Shop:
---------------------------------
| Item            | Stock | Price |
---------------------------------
| T-Shirts       |   10   | $15.00  |
| Jeans         |    5   | $40.00  |
| Jackets       |   20   | $60.00  |
---------------------------------
```

### 📌 Placing an Order (Success)
**Command:**
```bash
./customer
```
**Input:**
```
🛒 Enter item name (or type 'exit' to quit): T-Shirts
📦 Enter quantity: 3
```
**Output:**
```
✅ Order Approved: 3 x T-Shirts
```

### 📌 Placing an Order (Out of Stock)
**Command:**
```bash
./customer
```
**Input:**
```
🛒 Enter item name (or type 'exit' to quit): Jeans
📦 Enter quantity: 10
```
**Output:**
```
❌ Order Rejected: Jeans (Not enough stock)
```

### 📌 Restocking Inventory
**Command:**
```bash
./shopkeeper
```
**Input:**
```
📦 Enter item name to update (or 'exit' to quit): Jeans
📈 Enter new quantity: 15
💰 Enter new price: 45.00
```
**Output:**
```
✅ Inventory Updated!
```

### 📌 Auto-Restock Triggered
**Scenario:** A customer orders an item that is **low in stock**.
**Command:**
```bash
./customer
```
**Input:**
```
🛒 Enter item name (or type 'exit' to quit): Jackets
📦 Enter quantity: 18
```
**Output:**
```
✅ Order Processed: 18 x Jackets
🔴 Low stock warning! Auto-restocking
```

### 📌 Viewing Order Logs
**Command:**
```bash
cat orders.log
```
**Output:**
```
T-Shirts | Ordered: 3 | ✅ Approved
Jeans | Ordered: 10 | ❌ Rejected
Jackets | Ordered: 18 | ✅ Approved
```

---

## 📝 License
This project is **open-source** and can be modified as needed.

---

🎯 **Contributions are welcome!** Feel free to improve and expand this project. 🚀

