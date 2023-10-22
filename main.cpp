#include <iostream>
#include <utility>
#include <regex>

enum class TokenType {
    KEYWORD,
    IDENTIFIER,
    NUMERIC_LITERAL,
    OPERATOR,
    SEPARATOR,
    COMMENT,
    WHITE_SPACE,
    STRING_LITERAL,
    UNKNOWN
};

std::string token_type_fmt(TokenType token_type) {
    switch (token_type) {
        case TokenType::KEYWORD:
            return "KEYWORD";
        case TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case TokenType::NUMERIC_LITERAL:
            return "NUMERIC_LITERAL";
        case TokenType::OPERATOR:
            return "OPERATOR";
        case TokenType::SEPARATOR:
            return "SEPARATOR";
        case TokenType::COMMENT:
            return "COMMENT";
        case TokenType::UNKNOWN:
            return "UNKNOWN";
        case TokenType::WHITE_SPACE:
            return "WHITE_SPACE";
        case TokenType::STRING_LITERAL:
            return "STRING_LITERAL";
    }
    return "UNREAL";
}

struct Token {
    TokenType type;
    std::string value;
};

std::string get_colored_token(Token token) {
    std::string color;

    switch (token.type) {
        case TokenType::KEYWORD:
            color = "\033[1;31m";
            break;
        case TokenType::IDENTIFIER:
            color = "\033[1;32m";
            break;
        case TokenType::NUMERIC_LITERAL:
            color = "\033[1;33m";
            break;
        case TokenType::OPERATOR:
            color = "\033[1;34m";
            break;
        case TokenType::SEPARATOR:
            color = "\033[1;35m";
            break;
        case TokenType::COMMENT:
            color = "\033[1;36m";
            break;
        case TokenType::UNKNOWN:
            color = "\033[1;37m";
            break;
        case TokenType::WHITE_SPACE:
            color = "\033[1;38m";
            break;
        case TokenType::STRING_LITERAL:
            color = "\033[40;97m";
            break;
    }

    return color + token.value + "\033[0m";
}

using namespace std;

class Lexer {
public:
    explicit Lexer(std::string input) : input(std::move(input)) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;

        // split input into lines
        std::vector<std::string> lines;
        std::string line;

        for (char c : input) {
            if (c == '\n') {
                line += c;
                lines.push_back(line);
                line = "";
            } else {
                line += c;
            }
        }

        if (!line.empty()) {
            lines.push_back(line);
        }

        cout << "lines: " << lines.size() << endl;

        for (auto &line : lines) {
            if (line.empty()) {
                continue;
            }

            if (line[0] == '!') {
                tokens.push_back({TokenType::COMMENT, line});
                continue;
            }

            // to_uppercase line

            for (char &c : line) {
                c = toupper(c);
            }

            std::smatch match;

            while (std::regex_search(line, match, word)) {

                std::string token = match.str();

                if (std::find(keywords.begin(), keywords.end(), token) != keywords.end()) {
                    tokens.push_back({TokenType::KEYWORD, token});
                } else if (std::find(operators.begin(), operators.end(), token) != operators.end()) {
                    tokens.push_back({TokenType::OPERATOR, token});
                } else if (std::find(separators.begin(), separators.end(), token) != separators.end()) {
                    tokens.push_back({TokenType::SEPARATOR, token});
                } else if (std::regex_match(token, std::regex(R"(\d+(?:\.\d+)?)"))) {
                    tokens.push_back({TokenType::NUMERIC_LITERAL, token});
                } else if (std::regex_match(token, std::regex(R"(\w+)"))) {
                    tokens.push_back({TokenType::IDENTIFIER, token});
                } else if (std::find(whitespaces.begin(), whitespaces.end(), token) != whitespaces.end()) {
                    tokens.push_back({TokenType::WHITE_SPACE, token});
                } else if (std::regex_match(token, std::regex(R"(["|'].*["|'])"))) {
                    tokens.push_back({TokenType::STRING_LITERAL, token});
                } else {
                    tokens.push_back({TokenType::UNKNOWN, token});
                }

                line = match.suffix();
            }
        }

        return tokens;
    }

private:
    std::string input;


    std::vector<std::string> keywords = {
            "ALLOCATABLE", "ALLOCATE", "ASSIGN", "ASSOCIATE", "ASYNCHRONOUS",
            "BACKSPACE", "BIND", "BLOCK", "BLOCKDATA", "CALL",
            "CASE", "CHARACTER", "CLASS", "CLOSE", "COMMON",
            "COMPLEX", "CONTAINS", "CONTINUE", "CYCLE", "DATA",
            "DEALLOCATE", "DEFAULT", "DO", "DOUBLE", "ELEMENTAL",
            "ELSE", "ELSEIF", "END", "ENDBLOCK", "ENDBLOCKDATA",
            "ENDDO", "ENDFILE", "ENDFORALL", "ENDFUNCTION", "ENDIF",
            "ENDINTERFACE", "ENDMODULE", "ENDPROGRAM", "ENDSELECT", "ENDSUBROUTINE",
            "ENDTYPE", "ENDWHERE", "ENTRY", "ENUM", "EQUIVALENCE",
            "EXIT", "EXTENDS", "EXTERNAL", "FINAL", "FLUSH",
            "FORALL", "FORMAT", "FUNCTION", "GO", "GOTO",
            "IF", "IMPLICIT", "IMPORT", "INQUIRE", "INTEGER",
            "INTERFACE", "INTRINSIC", "IS", "LOGICAL", "MODULE",
            "NAMELIST", "NON_INTRINSIC", "NON_OVERRIDABLE", "NULLIFY", "ONLY",
            "OPEN", "OPERATOR", "OPTIONAL", "PARAMETER", "PAUSE",
            "POINTER", "PRINT", "PRIVATE", "PROCEDURE", "PROGRAM",
            "PROTECTED", "PUBLIC", "PURE", "READ", "REAL",
            "RECURSIVE", "RESULT", "RETURN", "REWIND", "SAVE",
            "SELECT", "SEQUENCE", "STOP", "SUBROUTINE", "TARGET",
            "THEN", "TYPE", "USE", "VOLATILE", "WAIT",
            "WHERE", "WHILE", "WRITE"
    };

    std::vector<std::string> operators = {"+", "-", "*", "/", "=", "<", ">", "<=", ">=", "==", "/=", ".AND.", ".OR.", ".NOT."};

    std::vector<std::string> separators = {"(", ")", "{", "}", "[", "]", ",", ";"};
    std::vector<std::string> whitespaces = {" ", "\t", "\n"};

    std::regex word = std::regex(R"(^(["|'].*["|'])|(<=|>=|==|/=|\+|-|\*|/|=|<|>|\.and\.|\.or\.|\.not\.)|(\(|\)|\{|\}|\[|\]|,|;)|(\d+(?:\.\d+)?)|(\w+)|(\s+))");
};



int main() {
    std::string filename;

    std::cout << "Enter filename: ";

    std::cin >> filename;

    FILE* file = fopen(filename.c_str(), "r");

    if (file == nullptr) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    std::string content;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != nullptr) {
        std::cout << buffer;
        content += buffer;
    }

    fclose(file);

    Lexer lexer(content);

    auto tokens = lexer.tokenize();

    std::cout << std::endl << "Tokens:" << std::endl;


    for (auto &token : tokens) {
        std::cout << "<" << token_type_fmt(token.type) << ", " << token.value << ">" << std::endl;
    }

    cout << endl;

    for (auto &token : tokens) {
        std::cout << get_colored_token(token);
    }


    return 0;
}
