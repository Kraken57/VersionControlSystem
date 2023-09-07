#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    // Create a file object to store the terminal output
    fstream file;
    // Open the file in append mode
    file.open("terminal.txt", ios::app);
    // Check if the file exists and is opened successfully
    if (file.is_open())
    {
        // Create a string variable to store the user input
        string input;
        // Create a boolean variable to store the user choice
        bool resume = false;
        // Ask the user if they want to resume from where they left
        cout << "Do you want to start from where you left? (y/n)" << endl;
        // Get the user input
        cin >> input;
        // Check if the user input is yes
        if (input == "y" || input == "Y")
        {
            // Set the resume flag to true
            resume = true;
            // Close the file in append mode
            file.close();
            // Open the file in input mode
            file.open("terminal.txt", ios::in);
            // Check if the file is opened successfully
            if (file.is_open())
            {
                // Read each line from the file and print it on the terminal
                while (getline(file, input))
                {
                    cout << input << endl;
                }
                // Close the file in input mode
                file.close();
                // Open the file in append mode again
                file.open("terminal.txt", ios::app);
            }
            else
            {
                // Print an error message if the file cannot be opened in input mode
                cout << "Error: Cannot open the file in input mode." << endl;
            }
        }
        // Loop until the user enters exit
        while (input != "exit")
        {
            // Get the user input
            cin >> input;
            // Write the user input to the file with a newline character
            file << input << "\n";
            // Print the user input on the terminal
            cout << input << endl;
        }
        // Close the file in append mode
        file.close();
    }
    else
    {
        // Print an error message if the file cannot be opened in append mode
        cout << "Error: Cannot open the file in append mode." << endl;
    }
    return 0;
}
