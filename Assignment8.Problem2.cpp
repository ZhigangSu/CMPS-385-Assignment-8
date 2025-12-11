#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

struct Song {
    int id;
    string title;
    string artist;
    float duration;
    Song* prev;
    Song* next;
};

Song* head = nullptr;
Song* tail = nullptr;
Song* current = nullptr;

void loadPlaylist();
void savePlaylist();
void addSong();
void displayPlaylist();
void playNext();
void playPrevious();
void removeSong();

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void appendSong(int id, const string& title, const string& artist, float duration) {
    Song* newSong = new Song;
    newSong->id = id;
    newSong->title = title;
    newSong->artist = artist;
    newSong->duration = duration;
    newSong->prev = nullptr;
    newSong->next = nullptr;

    if (head == nullptr) {
        head = newSong;
        tail = newSong;
        current = newSong;
    }
    else {
        tail->next = newSong;
        newSong->prev = tail;
        tail = newSong;
    }
}

int main() {
    loadPlaylist();
    int choice;

    do {
        cout << "\n****** Welcome to Music Playlist Manager ******\n";
        cout << "Please choose one of the following operations:\n";
        cout << "1. Add Song to Playlist\n";
        cout << "2. Display Playlist\n";
        cout << "3. Play Next Song\n";
        cout << "4. Play Previous Song\n";
        cout << "5. Remove Song\n";
        cout << "6. Save and Exit\n";
        cout << "-> ";
        cin >> choice;

        switch (choice) {
        case 1:
            addSong();
            break;
        case 2:
            displayPlaylist();
            break;
        case 3:
            playNext();
            break;
        case 4:
            playPrevious();
            break;
        case 5:
            removeSong();
            break;
        case 6:
            savePlaylist();
            cout << "Playlist saved. See you next jam!" << endl;
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }
    } while (choice != 6);

    Song* temp = head;
    while (temp != nullptr) {
        Song* toDelete = temp;
        temp = temp->next;
        delete toDelete;
    }

    return 0;
}

void loadPlaylist() {
    ifstream inFile("playlist.txt");
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

        string idStr = line.substr(0, p1);
        string title = line.substr(p1 + 1, p2 - p1 - 1);
        string artist = line.substr(p2 + 1, p3 - p2 - 1);
        string durationStr = line.substr(p3 + 1);

        int id = 0;
        float duration = 0.0f;

        try {
            id = stoi(idStr);
        }
        catch (...) {
            id = 0;
        }

        try {
            duration = static_cast<float>(stod(durationStr));
        }
        catch (...) {
            duration = 0.0f;
        }

        appendSong(id, title, artist, duration);
    }

    inFile.close();
}

void savePlaylist() {
    ofstream outFile("playlist.txt");
    if (!outFile) {
        cout << "Error: could not open playlist.txt for writing." << endl;
        return;
    }

    Song* temp = head;
    while (temp != nullptr) {
        outFile << temp->id << ","
            << temp->title << ","
            << temp->artist << ","
            << temp->duration << "\n";
        temp = temp->next;
    }

    outFile.close();
}

void addSong() {
    int id;
    string title;
    string artist;
    float duration;

    cout << "Enter Song ID: ";
    cin >> id;

    clearInputBuffer();

    cout << "Enter Title: ";
    getline(cin, title);

    cout << "Enter Artist: ";
    getline(cin, artist);

    cout << "Enter Duration (in minutes): ";
    cin >> duration;

    while (cin.fail() || duration < 0.0f) {
        cin.clear();
        clearInputBuffer();
        cout << "Invalid duration. Please enter a non-negative number: ";
        cin >> duration;
    }

    appendSong(id, title, artist, duration);

    cout << "Song added to playlist!" << endl;
}

void displayPlaylist() {
    if (head == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }

    cout << "========== CURRENT PLAYLIST ==========" << endl;
    cout << left << setw(8) << "ID"
        << left << setw(25) << "Title"
        << left << setw(20) << "Artist"
        << left << setw(10) << "Duration" << endl;
    cout << "----------------------------------------------------" << endl;

    cout << fixed << showpoint << setprecision(1);

    Song* temp = head;
    while (temp != nullptr) {
        cout << left << setw(8) << temp->id
            << left << setw(25) << temp->title
            << left << setw(20) << temp->artist
            << left << setw(10) << temp->duration << endl;
        temp = temp->next;
    }
}

void playNext() {
    if (current == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }

    if (current->next == nullptr) {
        cout << "You are at the last song." << endl;
        return;
    }

    current = current->next;

    cout << fixed << showpoint << setprecision(1);
    cout << "Now playing: " << current->title
        << " by " << current->artist
        << " (" << current->duration << " min)" << endl;
}

void playPrevious() {
    if (current == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }

    if (current->prev == nullptr) {
        cout << "You are at the first song." << endl;
        return;
    }

    current = current->prev;

    cout << fixed << showpoint << setprecision(1);
    cout << "Now playing: " << current->title
        << " by " << current->artist
        << " (" << current->duration << " min)" << endl;
}

void removeSong() {
    if (head == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }

    int targetId;
    cout << "Enter Song ID to remove: ";
    cin >> targetId;

    Song* temp = head;
    while (temp != nullptr && temp->id != targetId) {
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "Song with ID " << targetId << " not found in playlist." << endl;
        return;
    }

    string removedTitle = temp->title;

    if (temp == current) {
        if (temp->next != nullptr) {
            current = temp->next;
        }
        else if (temp->prev != nullptr) {
            current = temp->prev;
        }
        else {
            current = nullptr;
        }
    }

    if (temp == head && temp == tail) {
        head = nullptr;
        tail = nullptr;
    }
    else if (temp == head) {
        head = temp->next;
        head->prev = nullptr;
    }
    else if (temp == tail) {
        tail = temp->prev;
        tail->next = nullptr;
    }
    else {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    delete temp;

    cout << "Song '" << removedTitle << "' removed from playlist." << endl;
}
