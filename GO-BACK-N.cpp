#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cmath>

#define TOT_FRAMES 500
#define FRAMES_SEND 10

class gobkn {
private:
    int fr_send_at_instance;  
    int arr[TOT_FRAMES];       
    int arr1[FRAMES_SEND];     
    int sw;                    
    int rw;                    

public:
    gobkn();
    void input();
    void sender(int m);
    void receiver(int m);
};

// Constructor to initialize sender and receiver windows
gobkn::gobkn() {
    sw = 0;
    rw = 0;
}


void gobkn::input() {
    int n; 
    int m; 

    std::cout << "Enter the number of bits for the sequence number: ";
    std::cin >> n;

    m = pow(2, n); // Calculate the maximum sequence number (2^n)
    fr_send_at_instance = m / 2; // Half of the sequence number for Go-Back-N

    int t = 0;
    for (int i = 0; i < TOT_FRAMES; i++) {
        arr[i] = t;
        t = (t + 1) % m; 
    }

    srand(static_cast<unsigned>(time(0)));

    sender(m);
}

void gobkn::sender(int m) {
    int j = 0;
    for (int i = sw; i < sw + fr_send_at_instance; i++) {
        arr1[j] = arr[i];
        j++;
    }
    for (int i = 0; i < j; i++)
        std::cout << "SENDER: Frame " << arr1[i] << " is sent\n";

    receiver(m); 
}

void gobkn::receiver(int m) {
    int f = rand() % 10;  // Random number to simulate frame loss
    int f1, a1;
    char ch;

    // If frame is not lost (f != 5)
    if (f != 5) {
        for (int i = 0; i < fr_send_at_instance; i++) {
            if (rw == arr1[i]) {
                std::cout << "RECEIVER: Frame " << arr1[i] << " received correctly\n";
                rw = (rw + 1) % m; // Update expected frame number
            } else {
                std::cout << "RECEIVER: Duplicate frame " << arr1[i] << " discarded\n";
            }
        }

        a1 = rand() % 15;  // Random number to simulate ACK loss
        if (a1 >= 0 && a1 <= 3) {
            std::cout << "(Acknowledgment for Frame " << arr1[a1] << " and all after this lost)\n";
            sw = arr1[a1]; // Set the sender window to the lost frame
        } else {
            sw = (sw + fr_send_at_instance) % m;
        }
    } 
    / Simulate frame loss
    else {
        f1 = rand() % fr_send_at_instance; 
        for (int i = 0; i < f1; i++) {
            if (rw == arr1[i]) {
                std::cout << "RECEIVER: Frame " << arr1[i] << " received correctly\n";
                rw = (rw + 1) % m;
            } else {
                std::cout << "RECEIVER: Duplicate frame " << arr1[i] << " discarded\n";
            }
        }

        int Id = rand() % 2; // Randomly decide whether the frame is lost or damaged
        if (Id == 0)
            std::cout << "RECEIVER: Frame " << arr1[f1] << " damaged\n";
        else
            std::cout << "(Frame " << arr1[f1] << " lost)\n";

        for (int i = f1 + 1; i < fr_send_at_instance; i++)
            std::cout << "RECEIVER: Frame " << arr1[i] << " discarded\n";

        std::cout << "(SENDER TIMEOUT --> Resending frames from " << arr1[f1] << ")\n";
        sw = arr1[f1]; // Resend from the lost frame
    }
    std::cout << "Do you want to continue (y/n)? ";
    std::cin >> ch;

    if (ch == 'y')
        sender(m);
    else
        exit(0);
}

int main() {
    gobkn gb;
    gb.input();
    return 0;
}

