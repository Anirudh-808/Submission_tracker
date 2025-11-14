# Submission Tracker Project

This project is a **C-based submission tracking system** designed to log student submissions, set deadlines, check for late submissions, calculate penalties (extended version), and maintain a persistent record using a text file (`subs.txt`).

It includes two versions:
- **main.c** – Base version with late submission detection.
- **main2.c** – Extended version with penalty calculation.

---

## 📂 Files Included

### **1. main.c**
Implements:
- Uploading submissions
- Setting a deadline
- Checking submission status
- Displaying late submissions
- Storing updates to `subs.txt`
- Clearing stored submissions

### **2. main2.c**
Extends `main.c` with:
- Calculation of penalty based on delay duration
- Differentiated penalty types (minutes, hours, days, months, years)

### **3. subs.txt**
Contains previously logged submissions in the format:
```
ID,timestamp
```
Example:
```
001,09/09/25 08:06
```

### **4. test.exe**
Compiled executable for testing the project.

---

## 🧠 How the Program Works

### **1. Submission Logging**
Each submission has:
- Student ID
- Timestamp (`dd/mm/yy hh:mm`)
- A flag indicating whether it is late

Submissions are stored in an in-memory stack and also saved to `subs.txt`.

### **2. Deadline Setting**
The user may set a deadline at any time.
- All stored submissions are rechecked.
- Future submissions are marked late or on-time immediately.

### **3. Late Submission Detection**
The program compares:
- Submission timestamp
- Deadline timestamp

Depending on which component differs (minute/hour/day etc.), the submission is marked late.

### **4. Penalty Calculation (main2.c)**
Late duration is categorized by:
- Minutes
- Hours
- Days
- Months
- Years

Penalties are applied accordingly.

---

## 📜 Menu Options
Both versions present a menu with common operations.

### **main.c Menu**
1. Upload Submission
2. Set Deadline
3. Check Status of a Submission
4. Display All Late Submissions
5. Save & Exit
6. Clear File & Exit

### **main2.c Additional Option**
7. Calculate Penalty for a Submission

---

## ▶️ How to Run
1. Compile the program (GCC example):
```
gcc main.c -o tracker
```
or for extended version:
```
gcc main2.c -o tracker
```

2. Ensure `subs.txt` exists in the same directory.
3. Run:
```
./tracker
```

---

## 📌 Important Notes
- **Always set the deadline first**, otherwise all submissions are considered on time.
- Memory for dynamically allocated entries is freed when exiting via options 5 or 6.
- The timestamp must strictly follow: `dd/mm/yy hh:mm`.

---

## 🖼️ Output Screenshots
*(Place your screenshots below these headings)*

### **1. Program Startup:**
_[Add screenshot here]_

### **2. Uploading a Submission:**
_[Add screenshot here]_

### **3. Setting Deadline:**
_[Add screenshot here]_

### **4. Displaying Late Submissions:**
_[Add screenshot here]_

### **5. Penalty Calculation (main2.c):**
_[Add screenshot here]_

---

## ✔️ Conclusion
This submission tracker is a complete utility for managing time-based student submissions, identifying late entries, and applying penalties where necessary. It also provides persistent storage and efficient memory cleanup.

Feel free to extend the system further with sorting, enhanced UI, or database integration!

