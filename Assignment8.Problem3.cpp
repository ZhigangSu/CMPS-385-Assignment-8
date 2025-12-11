#include <iostream>
#include <string>
using namespace std;

struct TrafficLight {
    int id;
    string roadName;
    string color;
    TrafficLight* next;
};

TrafficLight* head = nullptr;
TrafficLight* current = nullptr;

void addLight();
void displaySequence();
void advanceLight();
void resetSystem();

int main() {
    int choice;
    do {
        cout << "\n****** Welcome to Smart Traffic Light Controller ******\n";
        cout << "Please choose one of the following operations:\n";
        cout << "1. Add Road and Light\n";
        cout << "2. Display Light Sequence\n";
        cout << "3. Advance to Next Light\n";
        cout << "4. Reset System\n";
        cout << "5. Exit\n";
        cout << "-> ";
        cin >> choice;

        switch (choice) {
        case 1:
            addLight();
            break;
        case 2:
            displaySequence();
            break;
        case 3:
            advanceLight();
            break;
        case 4:
            resetSystem();
            break;
        case 5:
            cout << "Traffic system saved. Goodbye and drive safe!" << endl;
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 5);

    resetSystem();

    return 0;
}

void addLight() {
    int id;
    string name;
    string color;

    cout << "Enter Road ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Road Name: ";
    getline(cin, name);

    cout << "Enter Initial Light Color (Green/Yellow/Red): ";
    getline(cin, color);

    TrafficLight* newLight = new TrafficLight;
    newLight->id = id;
    newLight->roadName = name;
    newLight->color = color;
    newLight->next = nullptr;

    if (head == nullptr) {
        head = newLight;
        newLight->next = newLight;
        current = newLight;
    }
    else {
        TrafficLight* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        temp->next = newLight;
        newLight->next = head;
    }

    cout << "Traffic light added for " << name << "." << endl;
}

void displaySequence() {
    if (head == nullptr) {
        cout << "No traffic lights in the system." << endl;
        return;
    }

    cout << "========== TRAFFIC LIGHT SEQUENCE ==========" << endl;
    cout << "ID\tRoad Name\t\tLight" << endl;
    cout << "-------------------------------------------" << endl;

    TrafficLight* temp = head;
    do {
        cout << temp->id << "\t" << temp->roadName << "\t\t" << temp->color << endl;
        temp = temp->next;
    } while (temp != head);
}

void advanceLight() {
    if (head == nullptr) {
        cout << "No traffic lights in the system." << endl;
        return;
    }

    if (current == nullptr) {
        current = head;
    }

    if (current->next == current) {
        current->color = "Green";
        cout << current->roadName << " -> Green" << endl;
        return;
    }

    TrafficLight* temp = head;
    do {
        temp->color = "Yellow";
        temp = temp->next;
    } while (temp != head);

    current->color = "Red";

    TrafficLight* nextLight = current->next;
    nextLight->color = "Green";

    temp = head;
    do {
        cout << temp->roadName << " -> " << temp->color << endl;
        temp = temp->next;
    } while (temp != head);

    current = nextLight;
}

void resetSystem() {
    if (head == nullptr) {
        cout << "System reset. All traffic lights cleared." << endl;
        current = nullptr;
        return;
    }

    TrafficLight* temp = head->next;
    while (temp != head) {
        TrafficLight* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }

    delete head;
    head = nullptr;
    current = nullptr;

    cout << "System reset. All traffic lights cleared." << endl;
}
