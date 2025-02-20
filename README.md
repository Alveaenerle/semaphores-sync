# FIFO Buffer with Threads and Semaphores

## 📌 Project Overview
This project implements a **9-element FIFO buffer** using **threads and semaphores**. It was created as part of the **Operating Systems course** during the **3rd semester**. The program follows a producer-consumer model with synchronization.

## 🔧 Features
- Two **producers**:
  - **Producer A** produces the letter 'A'.
  - **Producer B** produces the letter 'B'.
- Two **consumers**.
- **Buffer access rules**:
  - Producer A **can only write** when the buffer has **more than 5 elements**.
  - An item **can only be read** when the buffer has **more than 3 elements**.
- Implemented using **threads** and **semaphores** (Semaphore class from `monitor.h`).

---

## 🚀 Installation & Running the Program
### **1️⃣ Prerequisites**
- **GCC Compiler** (for C++ compilation)
- **Make** (for building the program)

### **2️⃣ Compiling the Program**
Run the following command in the project directory:
```bash
make
```
This will compile the `buffer.cpp` file and generate an executable.

### **3️⃣ Running the Program**
After compiling, execute the program using:
```bash
./buffer
```

### **4️⃣ Cleaning the Build**
To remove compiled files, run:
```bash
make clean
```

---

## 🏗️ Implementation Details
- **Synchronization**: Uses semaphores to control producer-consumer access.
- **Thread Management**: Multiple threads run simultaneously to produce and consume elements from the buffer.
- **Buffer Structure**: FIFO queue ensuring correct order of data processing.

---
