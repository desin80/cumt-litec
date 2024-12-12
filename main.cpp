#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// ANSI color codes for console output
namespace Color {
    const std::string RED = "\033[1;31m";
    const std::string GREEN = "\033[1;32m";
    const std::string YELLOW = "\033[1;33m";
    const std::string BLUE = "\033[1;34m";
    const std::string RESET = "\033[0m";
}

// Function to enable ANSI color on Windows
void enableWindowsConsoleColor() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
    SetConsoleMode(hOut, dwMode);
#endif
}

std::string readFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    enableWindowsConsoleColor();

    if (argc != 2) {
        std::cerr << Color::RED << "Error: Incorrect number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <source_file>" << Color::RESET << std::endl;
        return 1;
    }

    try {
        std::string source = readFile(argv[1]);
        
        // 保存源代码的每一行，用于错误显示
        std::vector<std::string> sourceLines;
        std::istringstream sourceStream(source);
        std::string line;
        
        // 读取并存储每一行
        while (std::getline(sourceStream, line)) {
            sourceLines.push_back(line);
        }
        
        // 打印源代码（带行号）
        std::cout << Color::BLUE << "\nSource code:\n" << Color::RESET;
        for (size_t i = 0; i < sourceLines.size(); ++i) {
            std::cout << Color::GREEN << std::setw(4) << (i + 1) << " | " 
                     << Color::RESET << sourceLines[i] << std::endl;
        }
        
        // 创建解析器并解析
        Parser parser(source);
        std::unique_ptr<BlockStmt> ast = parser.parse();
        
        if (ast) {
            std::cout << Color::GREEN << "\nParsing successful!" << Color::RESET << std::endl;
            std::cout << Color::BLUE << "\nAbstract Syntax Tree:\n" << Color::RESET;
            ast->print();
        } else {
            std::cout << Color::RED << "\nParsing failed." << Color::RESET << std::endl;
        }
        
    } catch (const ParseError& e) {
        std::cerr << Color::RED << "\n[PARSE ERROR] " << e.what() << Color::RESET << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << Color::RED << "\n[ERROR] " << e.what() << Color::RESET << std::endl;
        return 1;
    }

    return 0;
}
