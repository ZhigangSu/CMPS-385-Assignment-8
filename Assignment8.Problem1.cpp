#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

struct Patient {
    string id;
    string name;
    string condition;
    int severity;
    Patient* next;
};

Patient* head = nullptr;
int totalServed = 0;

void loadPatients();
void savePatients();
void addPatient();
void displayQueue();
void servePatient();
void showSummary();

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void appendPatient(const string& id, const string& name,
    const string& condition, int severity) {
    Patient* newNode = new Patient;
    newNode->id = id;
    newNode->name = name;
    newNode->condition = condition;
    newNode->severity = severity;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    }
    else {
        Patient* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}

int main() {
    loadPatients();
    int choice;

    do {
        cout << "\n****** Welcome to ER Patient Queue Manager ******\n";
        cout << "Please choose one of the following operations:\n";
        cout << "1. Add New Patient\n";
        cout << "2. Display Patient Queue\n";
        cout << "3. Serve Next Patient\n";
        cout << "4. View ER Summary\n";
        cout << "5. Exit\n";
        cout << "-> ";
        cin >> choice;

        switch (choice) {
        case 1:
            addPatient();
            break;
        case 2:
            displayQueue();
            break;
        case 3:
            servePatient();
            break;
        case 4:
            showSummary();
            break;
        case 5:
            savePatients();
            cout << "Patient queue saved to patients.txt. Stay safe!" << endl;
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);

    while (head != nullptr) {
        Patient* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}

void loadPatients() {
    ifstream inFile("patients.txt");
    if (!inFile) {
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        size_t p1 = line.find(',');
        size_t p2 = line.find(',', p1 + 1);
        size_t p3 = line.find(',', p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) {
            continue;
        }

        string id = line.substr(0, p1);
        string name = line.substr(p1 + 1, p2 - p1 - 1);
        string condition = line.substr(p2 + 1, p3 - p2 - 1);
        string severityStr = line.substr(p3 + 1);

        int severity = 0;
        try {
            severity = stoi(severityStr);
        }
        catch (...) {
            severity = 0;
        }

        appendPatient(id, name, condition, severity);
    }

    inFile.close();
}

void savePatients() {
    ofstream outFile("patients.txt");
    if (!outFile) {
        cout << "Error: could not open patients.txt for writing." << endl;
        return;
    }

    Patient* current = head;
    while (current != nullptr) {
        outFile << current->id << ","
            << current->name << ","
            << current->condition << ","
            << current->severity << "\n";
        current = current->next;
    }

    outFile.close();
}

void addPatient() {
    string id;
    string name;
    string condition;
    int severity;

    clearInputBuffer();

    cout << "Enter Patient ID: ";
    getline(cin, id);

    cout << "Enter patient name: ";
    getline(cin, name);

    cout << "Enter condition: ";
    getline(cin, condition);

    cout << "Enter severity (1-5): ";
    cin >> severity;

    while (severity < 1 || severity > 5 || cin.fail()) {
        cin.clear();
        clearInputBuffer();
        cout << "Invalid severity. Please enter a value from 1 to 5: ";
        cin >> severity;
    }

    appendPatient(id, name, condition, severity);

    cout << "Patient added to the queue." << endl;
}

void displayQueue() {
    if (head == nullptr) {
        cout << "No patients in queue." << endl;
        return;
    }

    cout << "=========== CURRENT QUEUE ===========" << endl;
    cout << left << setw(10) << "ID"
        << left << setw(20) << "Name"
        << left << setw(20) << "Condition"
        << left << setw(10) << "Severity" << endl;
    cout << "-----------------------------------------" << endl;

    Patient* current = head;
    while (current != nullptr) {
        cout << left << setw(10) << current->id
            << left << setw(20) << current->name
            << left << setw(20) << current->condition
            << left << setw(10) << current->severity << endl;
        current = current->next;
    }
}

void servePatient() {
    if (head == nullptr) {
        cout << "No patients in queue." << endl;
        return;
    }

    Patient* temp = head;
    head = head->next;

    cout << "Serving Patient: " << temp->id << " - " << temp->name << endl;

    totalServed++;
    delete temp;
}

void showSummary() {
    int count = 0;
    int severitySum = 0;

    Patient* current = head;
    while (current != nullptr) {
        count++;
        severitySum += current->severity;
        current = current->next;
    }

    double averageSeverity = 0.0;
    if (count > 0) {
        averageSeverity = static_cast<double>(severitySum) / count;
    }

    cout << "========== ER SUMMARY ==========" << endl;
    cout << "Total Patients Served: " << totalServed << endl;
    cout << "Patients in Queue: " << count << endl;
    cout << fixed << showpoint << setprecision(2);
    cout << "Average Severity of Waiting Patients: " << averageSeverity << endl;
    cout << "================================" << endl;
}
