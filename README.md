# C-Programming
Collection of low-level C projects including Address Book, MP3 Tag Reader, Steganography, Lexical Analyzer, Source to HTML converter.

## Projects Included

| Project | Description | Key Tech / Concepts |
| :--- | :--- | :--- |
| **Address Book** | A menu-driven C application used to create, search, edit, delete and display contacts. Each contact stores a name, phone number, and email address using structures, arrays, and persistent file handling (`fopen`, `fprintf`, `fscanf`)[cite: 13]. | C Programming, Structures (`struct`), Arrays, Pointers, Functions, String Handling (`string.h`), Menu-driven Programming, File Handling, CRUD Operations, Data Persistence[cite: 13]. |
| **Lexical Analyzer** | A C-based application that reads a C source file, tokenizes keywords, identifiers, operators, and preprocessor directives, and performs lexical validation with line-numbered error reporting[cite: 14]. | C Programming, File Handling (`fopen`, `fgetc`, `ungetc`, `fclose`), Structures (`struct`), Enumerations (`enum`), Strings, Tokenization, Input Validation, Stack Data Structure, Error Detection[cite: 14]. |
| **MP3 Tag Reader & Editor** | A menu-driven and command-line C application that reads, displays, and modifies ID3v1 and ID3v2 metadata (title, artist, album, year, genre) embedded in MP3 files without corrupting the underlying audio data[cite: 15]. | C Programming, File Handling (`fopen`, `fread`, `fwrite`, `fseek`, `ftell`), Binary File Processing, ID3v2 Frame Parsing, Endianness Conversion, Dynamic Memory Allocation, CLI (`argc`, `argv`)[cite: 15]. |
| **LSB Image Steganography** | A C-based application that hides secret text messages inside BMP image files using Least Significant Bit (LSB) manipulation, embedding size data, magic strings, and file extensions with full verification and decoding support[cite: 17]. | C Programming, File Handling (`fopen`, `fread`, `fwrite`, `fseek`, `ftell`, `rewind`, `fclose`), LSB Steganography, Bitwise Operations, BMP Image Handling, Dynamic File Operations[cite: 17]. |
| **Source2HTML** | A C-based application that reads and parses C source code files, converts them into styled HTML documents, and highlights different programming elements using predefined colors and optional line numbers[cite: 16]. | C Programming, File Handling (`fopen`, `fgetc`, `fputc`, `fprintf`, `fclose`), State-Event Based Parsing, Finite State Machines, Token Identification, HTML Generation, CSS Styling[cite: 16]. |

## Tech Stack
* **Language:** C 
* **Environment:** Linux / WSL (Ubuntu)
* **Build Tool:** GNU Make / GCC
