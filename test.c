{* This is a test file
   containing various C language features
   for testing the lexical analyzer *}

// Keyword and type tests
typedef int Integer;
enum Color { RED, GREEN, BLUE };
struct Point {
    int x;
    int y;
};
union Data {
    int i;
    float f;
    char str[20];
};

// Storage class tests
auto auto_var = 100;
static int static_var = 200;
extern int extern_var;
register int reg_var = 300;

// Function with various specifiers
static void test_func(const int param) {
    return;
}

int main() {
    // Basic type declaration tests
    int number = 42;
    float pi = 3.14159;
    char c = 'A';
    void* ptr = NULL;
    unsigned int uint_var = 1000U;
    signed char signed_char = -1;
    long double long_double = 123.456L;
    
    // Number literal tests
    int hex = 0xFF;        // hexadecimal
    int octal = 0123;      // octal
    int binary = 0b1101;   // binary
    long big = 123456789L; // long integer
    float f = 1.23f;       // float
    
    // Character and string literal tests
    char newline = '\n';
    char tab = '\t';
    char quote = '\'';
    char backslash = '\\';
    char null_char = '\0';
    char* str = "Hello, World!";
    char* escaped_str = "String with \n newline and \t tab";
    
    // Control flow tests
    if (number > 0) {
        number--;
    } else {
        number++;
    }
    
    // Loop tests
    while (number > 0) {
        number--;
        if (number == 5) continue;
        if (number == 0) break;
    }
    
    do {
        number++;
    } while (number < 10);
    
    for (int i = 0; i < 10; i++) {
        number += i;
    }
    
    // Operator tests
    int a = 5, b = 3;
    int sum = a + b;           // addition
    int diff = a - b;          // subtraction
    int prod = a * b;          // multiplication
    int quot = a / b;          // division
    int rem = a % b;           // modulo
    
    // Compound assignment operators
    a += b;
    a -= b;
    a *= b;
    a /= b;
    a %= b;
    
    // Increment/Decrement
    a++;
    b--;
    
    // Comparison operators
    int less = a < b;
    int greater = a > b;
    int leq = a <= b;
    int geq = a >= b;
    int eq = a == b;
    int neq = a != b;
    
    // Logical operators
    int and_result = (a > 0) && (b > 0);
    int or_result = (a > 0) || (b > 0);
    int not_result = !(a > 0);
    
    // Ternary operator
    int max = (a > b) ? a : b;
    
    // Bitwise operators
    int bit_and = a & b;
    int bit_or = a | b;
    int bit_xor = a ^ b;
    int bit_not = ~a;
    int left_shift = a << 2;
    int right_shift = a >> 1;
    
    // Array and pointer tests
    int arr[5] = {1, 2, 3, 4, 5};
    int* p = &number;
    const int* const_ptr = &number;
    volatile int vol_var = 42;
    
    // Switch-case tests
    switch (number) {
        case 1:
            break;
        case 2:
            continue;
        default:
            return 0;
    }
    
    // Goto statement
    start:
        if (number > 0) {
            number--;
            goto start;
        }
    
    // Size operator
    int size = sizeof(int);
    
    return 0;
}
