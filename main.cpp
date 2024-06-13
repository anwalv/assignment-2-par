#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
class TextEditor{
private:
    char *text;
    int arraySize;
    char clipboard[100];
    char *states[3];
    int numStates;
    int undoNum;
    int redoNum;
    int currentStateIndex;

public:
    TextEditor(int size = 100) {
        text = (char*) calloc(size, sizeof(char));
        arraySize = size;
        numStates = 0;
        undoNum = -1;
        redoNum = -1;
        currentStateIndex = -1;

        // Allocate memory for the states
        for (int i = 0; i < 3; ++i) {
            states[i] = (char*) calloc(arraySize, sizeof(char));
        }
    }

    ~TextEditor() {
        free(text);
        for (int i = 0; i < 3; ++i) {
            if (states[i] != nullptr) {
                free(states[i]);
            }
        }
    }

    void SaveState(){
        if (numStates == 3) {
            free(states[0]);
            for (int i = 0; i < 2; ++i) {
                states[i] = states[i + 1];
            }
            states[2] = nullptr;
            numStates = 2;
        }
        states[numStates] = (char*) calloc(arraySize, sizeof(char));
        strncpy(states[numStates], text, arraySize);
        currentStateIndex = numStates;
        numStates++;
        undoNum = numStates-1;
        redoNum= -1;
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
        int inputLength;
        int currentSize;
        char answer;

        while (answer!='y') {
            std::cout << "Enter the line number: ";
            std::cin >> lineNumber;
            std::cin.ignore();

            std::cout << "Enter position number: ";
            std::cin >> position;
            std::cin.ignore();

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
            displayTextWithCursor(position);
            std::cout << "Is this position correct?(y/n): ";
            std::cin >> answer;
            std::cin.ignore();
        }
        std::cout << "Please, enter text to insert:\n";
        std::cin.getline(userInput, 1000);

        inputLength = strlen(userInput);
        currentSize = strlen(text);
        if (currentSize + inputLength >= arraySize) {
            arraySize = currentSize + inputLength + 2;
            text = (char *) realloc(text, arraySize * sizeof(char));
            if (text == NULL) {
                std::cout << "Memory reallocation failed.\n";
                return;
            }
        }

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
        int lineNumber;
        int position;
        int numChar;
        char answer;
        while (answer!='y') {
            std::cout << "Enter the line number: ";
            std::cin >> lineNumber;
            std::cin.ignore();

            std::cout << "Enter position number: ";
            std::cin >> position;
            std::cin.ignore();

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
            displayTextWithCursor(position);
            std::cout << "Is this position correct?(y/n): ";
            std::cin >> answer;
            std::cin.ignore();
        }
        std::cout << "Please, enter number of characters: \n";
        std::cin >> numChar;
        std::cin.ignore();
        int currentPosition = 0;
        int textLength = strlen(text);
        position += currentPosition;
        if(position + numChar > textLength ){
            numChar = textLength -position;
        }
        memmove(text+ position, text + position + numChar, textLength - position- numChar + 1);
        std::cout << "Text was deleted successfully\n";
    }

    void Undo(){
        if (undoNum<0){
            std::cout<< "No commands to undo";
        }
        if(undoNum == 0 && numStates < 3){
            strncpy(text, states[undoNum], arraySize);
            currentStateIndex = undoNum;
            undoNum = -1;
        }
        else{
            strncpy(text, states[undoNum-1], arraySize);
            currentStateIndex = undoNum;
            undoNum --;
        }
        redoNum = std::min(redoNum + 1, numStates - 1);
        std::cout << "Undo performed successfully\n";
    }

    void Redo() {
        if (redoNum < 0 || redoNum >= numStates) {
            std::cout << "No states to redo\n";
            return;
        }

        strncpy(text, states[redoNum], arraySize);
        redoNum++;
        currentStateIndex = redoNum;

        undoNum = std::min(undoNum + 1, numStates - 1);

        std::cout << "Redo performed successfully\n";
    }
    void ReplaceInsert(){
        int lineNumber;
        int position;
        char userInput[1000];
        int inputLength;
        int currentSize;
        char answer;

        while (answer!='y') {
            std::cout << "Enter the line number: ";
            std::cin >> lineNumber;
            std::cin.ignore();

            std::cout << "Enter position number: ";
            std::cin >> position;
            std::cin.ignore();

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
            displayTextWithCursor(position);
            std::cout << "Is this position correct?(y/n): ";
            std::cin >> answer;
            std::cin.ignore();
        }
        std::cout << "Please, enter text to insert:\n";
        std::cin.getline(userInput, 1000);

        inputLength = strlen(userInput);
        currentSize = strlen(text);

        if (position + inputLength > currentSize) {
            std::cout << "The position and length of input exceed text size.\n";
            return;
        }

        strncpy(text + position, userInput, inputLength);
        std::cout << "Text was inserted with replacement successfully\n";
    }

    void Cut (){
        int lineNumber;
        int position;
        int numChar;
        char answer;
        while (answer!='y') {
            std::cout << "Enter the line number: ";
            std::cin >> lineNumber;
            std::cin.ignore();

            std::cout << "Enter position number: ";
            std::cin >> position;
            std::cin.ignore();

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
            displayTextWithCursor(position);
            std::cout << "Is this position correct?(y/n): ";
            std::cin >> answer;
            std::cin.ignore();
        }
        std::cout << "Please, enter number of characters: \n";
        std::cin >> numChar;
        std::cin.ignore();

        int textLength = strlen(text);
        if(position + numChar > textLength ){
            numChar = textLength -position;
        }
        strncpy(clipboard, text+position, numChar);
        clipboard[numChar] = '\0';
        memmove(text+ position, text + position + numChar, textLength - position- numChar + 1);
        std::cout<< "Text was cut successfully!";
    }

    void PasteText(){
        int lineNumber;
        int position;

        char answer;
        while (answer!='y') {
            std::cout << "Enter the line number: ";
            std::cin >> lineNumber;
            std::cin.ignore();

            std::cout << "Enter position number: ";
            std::cin >> position;
            std::cin.ignore();

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
            displayTextWithCursor(position);
            std::cout << "Is this position correct?(y/n): ";
            std::cin >> answer;
            std::cin.ignore();
        }
        int bufferLength = strlen(clipboard);
        int currentSize = strlen(text);
        if (currentSize + bufferLength >= arraySize) {
            arraySize = currentSize + bufferLength + 2;
            text = (char *) realloc(text, arraySize * sizeof(char));
            if (text == NULL) {
                std::cout << "Memory reallocation failed.\n";
                return;
            }
        }
        memmove(text + position + bufferLength, text + position, currentSize - position + 1);
        strncpy(text + position, clipboard, bufferLength);
        memset(clipboard, 0, sizeof(clipboard));
        std::cout << "Text was pasted successfully\n";
    }
    void CopyText(){
        int lineNumber;
        int position;
        int numChar;
        char answer;
        while (answer!='y') {
            std::cout << "Enter the line number: ";
            std::cin >> lineNumber;
            std::cin.ignore();

            std::cout << "Enter position number: ";
            std::cin >> position;
            std::cin.ignore();

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
            displayTextWithCursor(position);
            std::cout << "Is this position correct?(y/n): ";
            std::cin >> answer;
            std::cin.ignore();
        }
        std::cout << "Please, enter number of characters: \n";
        std::cin >> numChar;
        std::cin.ignore();

        int textLength = strlen(text);
        if(position + numChar > textLength ){
            numChar = textLength -position;
        }
        strncpy(clipboard, text + position, numChar);
        clipboard[numChar] = '\0';
    }
    void displayTextWithCursor(int position) {
        for (int i = 0; i < strlen(text); ++i) {
            if (i == position) {
                std::cout << "|"; // показуємо курсор
            }
            std::cout << text[i];
        }
        if (position == strlen(text)) {
            std::cout << "|"; // курсор в кінці тексту
        }
        std::cout << std::endl;
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
    editor.SaveState();
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
                editor.SaveState();
                break;
            case 2:
                editor.StartNewLine();
                editor.SaveState();
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
                editor.SaveState();
                break;
            case 7:
                editor.SearchSubstring();
                break;
            case 8:
                editor.DeleteText();
                editor.SaveState();
                break;
            case 9:
                editor.Undo();
                break;
            case 10:
                editor.Redo();
                break;
            case 11:
                editor.Cut();
                editor.SaveState();
                break;
            case 12:
                editor.CopyText();
                break;
            case 13:
                editor.PasteText();
                editor.SaveState();
                break;
            case 14:
                editor.ReplaceInsert();
                editor.SaveState();
                break;
            case 15:
                std::cout << "Ciao!";
        }
    }
    return 0;
}
