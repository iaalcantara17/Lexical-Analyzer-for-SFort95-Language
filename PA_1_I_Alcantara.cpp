//Israel Alcantara
//Lexical Analyzer for SFort95 Lanugage
//CS-280

#include <cctype>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <climits>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "lex.h"


// Keywords map
map<string, Token> keywords = 
{
    {"program", PROGRAM},
    {"end", END},
    {"if", IF},
    {"else", ELSE},
    {"integer", INTEGER},
    {"real", REAL},
    {"character", CHARACTER},
    {"print", PRINT},
    {"len", LEN}
};

LexItem id_or_kw(const string& lexeme, int linenum) 
{
    if (keywords.find(lexeme) != keywords.end()) 
    {
        return LexItem(keywords[lexeme], lexeme, linenum);
    }
    return LexItem(IDENT, lexeme, linenum);
}

ostream& operator<<(ostream& out, const LexItem& tok) 
{
    Token token = tok.GetToken();
    string lexeme = tok.GetLexeme();

    if (token == IDENT) 
    {
        out << "IDENT: '" << lexeme << "'";
    } 
    else if (token == ICONST) 
    {
        out << "ICONST: " << lexeme;
    } 
    else if (token == RCONST) 
    {
        out << "RCONST: " << lexeme;
    } 
    else if (token == SCONST) 
    {
        out << "SCONST: \"" << lexeme << "\"";
    } 
    else 
    {
        out << token;
    }
    return out;
}

LexItem getNextToken(istream& in, int& linenum) 
{
    char ch;
    string lexeme = "";

    while (in.get(ch)) 
    {
        if (ch == '\n') 
        {
            linenum++;
            continue;
        }
        if (isspace(ch)) continue;

        if (isalpha(ch)) 
        {
            lexeme += ch;
            while (in.get(ch) && (isalnum(ch) || ch == '_')) 
            {
                lexeme += ch;
            }
            in.putback(ch);
            return id_or_kw(lexeme, linenum);
        }

        if (isdigit(ch)) 
        {
            lexeme += ch;
            while (in.get(ch) && isdigit(ch)) 
            {
                lexeme += ch;
            }
            if (ch == '.') 
            {
                lexeme += ch;
                if (!in.get(ch) || !isdigit(ch)) 
                {
                    return LexItem(ERR, lexeme, linenum);
                }
                do 
                {
                    lexeme += ch;
                } 
                while (in.get(ch) && isdigit(ch));
            }
            in.putback(ch);
            if (lexeme.find('.') != string::npos) 
            {
                return LexItem(RCONST, lexeme, linenum);
            } 
            else 
            {
                return LexItem(ICONST, lexeme, linenum);
            }
        }

        if (ch == '"' || ch == '\'') 
        {
            char delim = ch;
            lexeme += ch;
            while (in.get(ch) && ch != delim) 
            {
                if (ch == '\n') return LexItem(ERR, lexeme, linenum);
                lexeme += ch;
            }

            if (ch != delim) return LexItem(ERR, lexeme, linenum);
            lexeme += ch;
            return LexItem(SCONST, lexeme, linenum);
        }

        switch (ch) {
            case '+': return LexItem(PLUS, "+", linenum);
            case '-': return LexItem(MINUS, "-", linenum);
            case '*': return LexItem(MULT, "*", linenum);
            case '/': return LexItem(DIV, "/", linenum);
            case '=': return LexItem(ASSOP, "=", linenum);
            case '<': return LexItem(LTHAN, "<", linenum);
            case '>': return LexItem(GTHAN, ">", linenum);
            case ',': return LexItem(COMMA, ",", linenum);
            case '(': return LexItem(LPAREN, "(", linenum);
            case ')': return LexItem(RPAREN, ")", linenum);
            case '.': return LexItem(DOT, ".", linenum);
            case '!':
                while (in.get(ch) && ch != '\n');
                linenum++;
                continue;
            default:
                return LexItem(ERR, string(1, ch), linenum);
        }
    }

    return LexItem(DONE, "", linenum);
}

int main(int argc, char* argv[]) 
{
    ifstream infile;
    bool printAllTokens = false, printInts = false, printReals = false, printStrings = false;
    bool printIdentifiers = false, printKeywords = false;

    map<string, int> idCount, kwCount, intCount, realCount, strCount;

    if (argc < 2) 
    {
        cerr << "NO SPECIFIED INPUT FILE." << endl;
        return 1;
    }

    string filename;
    for (int i = 1; i < argc; i++) 
    {
        string arg = argv[i];
        if (arg == "-all") printAllTokens = true;
        else if (arg == "-int") printInts = true;
        else if (arg == "-real") printReals = true;
        else if (arg == "-str") printStrings = true;
        else if (arg == "-id") printIdentifiers = true;
        else if (arg == "-kw") printKeywords = true;
        else if (arg[0] == '-') 
        {
            cerr << "UNRECOGNIZED FLAG " << arg << endl;
            return 1;
        } 
        else
        {
            if (!filename.empty()) 
            {
                cerr << "ONLY ONE FILE NAME IS ALLOWED." << endl;
                return 1;
            }
            filename = arg;
        }
    }

    if (filename.empty()) 
    {
        cerr << "NO SPECIFIED INPUT FILE." << endl;
        return 1;
    }

    infile.open(filename);
    if (!infile) 
    {
        cerr << "CANNOT OPEN THE FILE " << filename << endl;
        return 1;
    }

    infile.seekg(0, ios::end);
    if (infile.tellg() == 0) 
    {
        cout << "Empty File." << endl;
        return 0;
    }
    infile.seekg(0, ios::beg);

    int linenum = 1;
    LexItem token;
    while ((token = getNextToken(infile, linenum)).GetToken() != DONE && token.GetToken() != ERR) 
    {
        if (printAllTokens) cout << token << endl;

        Token tokType = token.GetToken();
        if (tokType == IDENT) idCount[token.GetLexeme()]++;
        if (tokType == ICONST) intCount[token.GetLexeme()]++;
        if (tokType == RCONST) realCount[token.GetLexeme()]++;
        if (tokType == SCONST) strCount[token.GetLexeme()]++;
        if (tokType >= PROGRAM && tokType <= LEN) kwCount[token.GetLexeme()]++;
    }

    if (token.GetToken() == ERR) 
    {
        cerr << "Error in line " << linenum << ": Unrecognized Lexeme {"
             << token.GetLexeme() << "}" << endl;
        return 1;
    }

    if (printIdentifiers && !idCount.empty()) 
    {
        cout << "IDENTIFIERS: ";
        for (const auto& id : idCount)
        {
            cout << id.first << "(" << id.second << ") ";
        }
        cout << endl;
    }

    if (printKeywords && !kwCount.empty()) 
    {
        cout << "KEYWORDS: ";
        for (const auto& kw : kwCount) 
        {
            cout << kw.first << "(" << kw.second << ") ";
        }
        cout << endl;
    }

    if (printInts && !intCount.empty()) 
    {
        cout << "INTEGERS: ";
        for (const auto& ic : intCount) 
        {
            cout << ic.first << " ";
        }
        cout << endl;
    }

    if (printReals && !realCount.empty()) 
    {
        cout << "REALS: ";
        for (const auto& rc : realCount) 
        {
            cout << rc.first << " ";
        }
        cout << endl;
    }

    if (printStrings && !strCount.empty()) 
    {
        cout << "STRINGS: ";
        for (const auto& sc : strCount) 
        {
            cout << "\"" << sc.first << "\" ";
        }
        cout << endl;
    }

    return 0;
}
