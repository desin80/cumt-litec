#include "lexer.h"
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

std::string readFile(const std::string& path) {
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

    // 保存源代码的每一行，用于错误显示
    std::vector<std::string> sourceLines;
    
    try {
        std::string source = readFile(argv[1]);
        
        // 读取源代码的每一行
        std::istringstream sourceStream(source);
        std::string line;
        while (std::getline(sourceStream, line)) {
            sourceLines.push_back(line);
        }
        
        // 打印源代码（带行号）
        std::cout << Color::BLUE << "\nSource code:\n" << Color::RESET;
        for (size_t i = 0; i < sourceLines.size(); ++i) {
            std::cout << Color::GREEN << std::setw(4) << (i + 1) << " | " 
                     << Color::RESET << sourceLines[i] << std::endl;
        }
        
        Lexer lexer(source);
        
        // 获取并打印所有token
        std::cout << Color::BLUE << "\nTokens:\n" << Color::RESET;
        lexer.getAllTokens();
        lexer.printTokens();
        
    } catch (const std::runtime_error& e) {
        std::string errorMsg = e.what();
        size_t linePos = errorMsg.find("line ");
        size_t columnPos = errorMsg.find("column ");
        
        if (linePos != std::string::npos && columnPos != std::string::npos) {
            // 提取行号和列号
            size_t lineNumStart = linePos + 5;
            size_t lineNumEnd = errorMsg.find(",", lineNumStart);
            size_t columnNumStart = columnPos + 7;
            size_t columnNumEnd = errorMsg.find_first_of(" \n", columnNumStart);
            
            int errorLine = std::stoi(errorMsg.substr(lineNumStart, lineNumEnd - lineNumStart));
            int errorColumn = std::stoi(errorMsg.substr(columnNumStart, columnNumEnd - columnNumStart));
            
            // 打印错误信息
            std::cerr << Color::RED << "\n[LEXICAL ERROR] " << errorMsg << Color::RESET << std::endl;
            
            // 打印出错的代码行及其上下文
            if (errorLine > 1 && errorLine - 2 < sourceLines.size()) {
                std::cout << Color::GREEN << std::setw(4) << (errorLine - 1) << " | " 
                         << Color::RESET << sourceLines[errorLine - 2] << std::endl;
            }
            
            if (errorLine - 1 < sourceLines.size()) {
                std::cout << Color::GREEN << std::setw(4) << errorLine << " | " 
                         << Color::RESET << sourceLines[errorLine - 1] << std::endl;
                
                // 打印错误位置指示器
                std::cout << "     | " << std::string(errorColumn - 1, ' ') 
                         << Color::RED << "^" << Color::RESET << std::endl;
            }
            
            if (errorLine < sourceLines.size()) {
                std::cout << Color::GREEN << std::setw(4) << (errorLine + 1) << " | " 
                         << Color::RESET << sourceLines[errorLine] << std::endl;
            }
        } else {
            // 如果无法解析行号和列号，直接打印错误信息
            std::cerr << Color::RED << "\n[LEXICAL ERROR] " << errorMsg << Color::RESET << std::endl;
        }
        return 1;
    }

    return 0;
}
