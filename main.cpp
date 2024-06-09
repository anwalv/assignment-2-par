#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
class TextEditor{
private:
    char *text;
    int arraySize;
    char clipboard[100];

public:
    TextEditor(int size = 100) {
        text = (char*) calloc(size, sizeof(char));
        arraySize = size;
    }

    ~TextEditor() {
        free(text);
    }

    void AppendText(){
        char userInput[1000];
        std::cout << "Please, enter text:\n";
        std::cin.getline(userInput, 1000);
        int inputLength = strlen(userInput);
        int currentSize = strlen(text);
        if (currentSize + inputLength >= arraySize) {
            text = (char *) realloc(text, (currentSize + inputLength + 2) * sizeof(char));
            arraySize = currentSize + inputLength + 2;
        }
        strcat(text, userInput);
        std::cout << "Text was added successfully\n";
    }

    void StartNewLine() {
        strcat(text, "\n");
        std::cout << "A new line has started!\n";
    }

    void WriteToFile() {
        FILE *out_file = fopen("C:\\Users\\User\\CLionProjects\\untitled2\\my_text.txt", "w");
        if (out_file == NULL) {
            std::cout << "Error! Could not open file.\n";
            return;
        }
        fprintf(out_file, "%s", text);
        fclose(out_file);
        std::cout << "The data was successfully written to the file.\n";
    }

    void LoadFromFile() {
        FILE *in_file = fopen("C:\\Users\\User\\CLionProjects\\untitled2\\my_text.txt", "r");
        if (in_file == NULL) {
            std::cout << "Error! Could not open file.\n";
            return;
        }
        char myText[1000];
        while (fgets(myText, sizeof(myText), in_file) != NULL) {
            std::cout << myText;
        }
        fclose(in_file);
    }

    void PrintText() {
        std::cout << "Your text: " << text << "\n";
    }

    void InsertText() {
        int lineNumber;
        int position;
        char userInput[1000];

        std::cout << "Enter the line number: ";
        std::cin >> lineNumber;
        std::cin.ignore();

        std::cout << "Enter position number: ";
        std::cin >> position;
        std::cin.ignore();

        std::cout << "Please, enter text to insert:\n";
        std::cin.getline(userInput, 1000);

        int inputLength = strlen(userInput);
        int currentSize = strlen(text);
        if (currentSize + inputLength >= arraySize) {
            arraySize = currentSize + inputLength + 2;
            text = (char *) realloc(text, arraySize * sizeof(char));
            if (text == NULL) {
                std::cout << "Memory reallocation failed.\n";
                return;
            }
        }
        int currentLine = 1;
        int currentPosition = 0;
        char *ptr = text;
        while (currentLine < lineNumber) {
            if (*ptr == '\n') {
                currentLine++;
            }
            ptr++;
            currentPosition++;
        }
        position += currentPosition;
        memmove(text + position + inputLength, text + position, currentSize - position + 1);
        strncpy(text + position, userInput, inputLength);
        std::cout << "Text was added successfully\n";
    }

    void SearchSubstring() {
        char substring[1000];
        std::cout << "Enter the substring to search for: ";
        std::cin >> substring;
        char *ptr = text;
        int position = 0;
        while ((ptr = strstr(ptr, substring))) {
            std::cout << "Substring found at position " << ptr - text + position << "\n";
            position = ptr - text + 1;
            ptr++;
        }
    }

    void DeleteText(){

    }

    void Undo(){

    }

    void Redo(){
    }

    void ReplaceInsert(){

    }

    void Cut (){

    }

    void PasteText(){

    }

    void CopyText(){

    }
};
int main() {
    TextEditor editor;
    std::cout << "Commands:\n"
              << "1. Append text.\n"
              << "2. Start a new line.\n"
              << "3. Save information to a file.\n"
              << "4. Load information from a file.\n"
              << "5. Print current text.\n"
              << "6. Insert text by line and symbol index.\n"
              << "7. Search text.\n"
              << "8. Delete text.\n"
              << "9. Undo.\n"
              << "10. Redo.\n"
              << "11. Cut text.\n"
              << "12. Copy text.\n"
              << "13. Paste text.\n"
              << "14. Insert with replacement.\n"
              << "15. Exit.\n";

    int command = 0;
    while (command != 15) {
        std::cout << "\nPlease, enter a number of command: ";
        std::cin >> command;
        std::cin.ignore();
        if (command < 1 || command > 15) {
            std::cout << "Invalid command.\n";
            continue;
        }
        switch (command) {
            case 1:
                editor.AppendText();
                break;
            case 2:
                editor.StartNewLine();
                break;
            case 3:
                editor.WriteToFile();
                break;
            case 4:
                editor.LoadFromFile();
                break;
            case 5:
                editor.PrintText();
                break;
            case 6:
                editor.InsertText();
                break;
            case 7:
                editor.SearchSubstring();
                break;
            case 8:
                editor.DeleteText();
                break;
            case 9:
                editor.Undo();
                break;
            case 10:
                editor.Redo();
                break;
            case 11:
                editor.Cut();
                break;
            case 12:
                editor.CopyText();
                break;
            case 13:
                editor.PasteText();
                break;
            case 14:
                editor.ReplaceInsert();
                break;
            case 15:
                std::cout << "Ciao!";
        }
    }
    return 0;
}
