#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>

using namespace std;

enum lex_type
{
    _NULL,                               //0
    _PROGRAM,                            //1
    _IF, _ELSE,                          //2, 3
    _WHILE,                              //4
    _READ, _WRITE,                       //5, 6
    _INT, _STRING,                       //7, 8
    _INT_VALUE, _STR_VALUE,              //9, 10
    _NOT, _AND, _OR,                     //11, 12, 13
    _VAR_NAME,                           //14
    _PLUS, _MINUS, _MUL, _DIV,           //15, 16, 17, 18
    _LSS, _GRT, _EQL, _LEQ, _GEQ, _NEQ,  //19, 20, 21, 22, 23, 24
    _ASSIGN,                             //25
    _COMMA, _SEMICOLON,                  //26, 27
    _LPAREN, _RPAREN,                    //28, 29
    _LBRACE, _RBRACE                     //30, 31
};

map <int, string> lex_type_table =
{
    {0, "NULL"}, {1, "program"}, {2, "if"}, {3, "else"}, {4, "while"}, {5, "read"}, {6, "write"}, {7, "int"}, {8, "string"},
    {11, "not"}, {12, "and"}, {13, "or"}, {14, "variable name"}, {15, "+"}, {16, "-"}, {17, "*"}, {18, "/"}, {19, "<"}, {20, ">"}, 
    {21, "="}, {22, "<="}, {23, ">="}, {24, "!="}, {25, "="}, {26, ","}, {27, ";"}, {28, "("}, {29, ")"}, {30, "{"}, {31, "}"}
};

map <string, lex_type> words_table = 
{
    {"program", _PROGRAM}, {"if", _IF}, {"else", _ELSE}, {"while", _WHILE}, {"read", _READ}, {"write", _WRITE}, {"int", _INT},
    {"string", _STRING}, {"not", _NOT}, {"and", _AND}, {"or", _OR}
};

map <string, lex_type> symbols_table = 
{
    {"+", _PLUS}, {"-", _MINUS}, {"*", _MUL}, {"/", _DIV}, {"<", _LSS}, {">", _GRT}, {"==", _EQL}, {"<=", _LEQ}, {">=", _GEQ},
    {"=", _ASSIGN}, {",", _COMMA}, {";", _SEMICOLON}, {"(", _LPAREN}, {")", _RPAREN}, {"{", _LBRACE}, {"}", _RBRACE}
};

class Lexeme
{
public:
    lex_type t;
    int int_value;
    string str_value;

    Lexeme(lex_type _t, int _int_value = _NULL)
    {
        t = _t;
        int_value = _int_value;
        str_value = "";
    }
    Lexeme(lex_type _t, string _str_value)
    {
        t = _t;
        int_value = _NULL;
        str_value = _str_value;
    }
    void Print_lex()
    {
        if (t == 9)
            cout << int_value;
        else if (t == 10)
            cout << str_value;
        else
            cout << lex_type_table[t];
    }
};

vector <Lexeme> array_of_lex;
int ind = 0;
void gl(Lexeme & l)
    {
        if (ind < array_of_lex.size())
            l = array_of_lex[ind];
        else
        {
            l = Lexeme(_NULL);
        }
        ind++;
    }

class Scanner
{
public:
    enum state {H, IDENT, NUMB, STR, DIV, COM, COM1, RELAT, NEQ, END, OTHER, ERR};
    state CS;
    FILE *fp;
    char c;
    string curr;
    
    void gc()
    {
        c = fgetc(fp);
    }
    
    Scanner(const char* filename)
    {
        fp = fopen(filename, "r");
        gc();
    }

    void Scan()
    {
        CS = H;
        do
        {
            switch(CS)
            {
            case H:
                if (c == EOF)
                {
                    CS = END;
                }
                else if (c == ' ' || c == '\n' || c == '\t')
                    gc();
                else if (isalpha(c))
                {
                    curr.clear();
                    curr.push_back(c);
                    CS = IDENT;
                    gc();
                }
                else if (isdigit(c))
                {
                    curr.push_back(c);
                    CS = NUMB;
                    gc();
                }
                else if (c == '"')
                {
                    CS = STR;
                    gc();
                }
                else if (c == '/')
                {
                    CS = DIV;
                    gc();
                }
                else if (c == '>' || c == '<' || c == '=')
                {
                    curr.push_back(c);
                    CS = RELAT;
                    gc();
                }
                else if (c == '!')
                {
                    curr.push_back(c);
                    CS = NEQ;
                    gc();
                }
                else
                {
                    curr.push_back(c);
                    CS = OTHER;
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c))
                {
                    curr.push_back(c);
                    gc();
                }
                else
                {
                    if (words_table.find(curr) != words_table.end())
                        array_of_lex.push_back(Lexeme(words_table[curr]));
                    else
                        array_of_lex.push_back(Lexeme(_VAR_NAME, curr));
                    curr.clear();
                    CS = H;
                }
                break;
            case NUMB:
                if (isdigit(c))
                {
                    curr.push_back(c);
                    gc();
                }
                else if (isalpha(c))
                    CS = ERR;
                else
                {
                    array_of_lex.push_back(Lexeme(_INT_VALUE, stoi(curr)));
                    curr.clear();
                    CS = H;
                }
                break;
            case STR:
                if (c == EOF)
                    CS = ERR;
                else if (c == '"')
                {
                    array_of_lex.push_back(Lexeme(_STR_VALUE, curr));
                    curr.clear();
                    CS = H;
                    gc();
                }
                else
                {
                    curr.push_back(c);
                    gc();
                }
                break;
            case DIV:
                if (c == '*')
                    CS = COM;
                else
                {
                    curr.push_back('/');
                    array_of_lex.push_back(Lexeme(_DIV));
                    curr.clear();
                    CS = H;
                }
                break;
            case COM:
                if (c == '*')
                {
                    CS = COM1;
                    gc();
                }
                else if (c == EOF)
                    CS = ERR;
                else
                    gc();
                break;
            case COM1:
                if (c == '/')
                {
                    CS = H;
                    gc();
                }
                else if (c == EOF)
                    CS = ERR;
                else
                {
                    CS = COM;
                    gc();
                }
                break;
            case RELAT:
                if (c == '=')
                {
                    curr.push_back(c);
                    array_of_lex.push_back(Lexeme(symbols_table[curr]));
                    curr.clear();
                    CS = H;
                    gc();
                }
                else
                {
                    array_of_lex.push_back(Lexeme(symbols_table[curr]));
                    curr.clear();
                    CS = H;
                }
                break;
            case NEQ:
                if (c == '=')
                {
                    curr.push_back(c);
                    array_of_lex.push_back(Lexeme(_NEQ));
                    curr.clear();
                    CS = H;
                    gc();
                }
                else
                {
                    c = '!';
                    CS = ERR;
                }
                break;
            case OTHER:
                if (c == '*' || c == '+' || c == '-' || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}')
                {
                    array_of_lex.push_back(Lexeme(symbols_table[curr]));
                    curr.clear();
                    CS = H;
                    gc();
                }
                else
                    CS = ERR; 
                break;
            case ERR:
                curr.clear();
                throw c;
                break;
            }
        } while (CS != END);
    }
};

class Parser
{
public:
    Lexeme l = Lexeme(_PROGRAM);

    void P()
    {
        if (l.t == _PROGRAM)
            gl(l);
        else
            throw l;
        if (l.t == _LBRACE)
            gl(l);
        else
            throw l;
        D1();
        S1();
        if (l.t == _RBRACE)
            gl(l);
        else
            throw l;
    }

    void D1()
    {
        D();
        while (l.t == _SEMICOLON)
        {
            gl(l);
            D();
        }
    }

    void D()
    {
        if (l.t == _IF)
            gl(l);
        else
            throw l;
    }

    void S1()
    {
        if (l.t == _ELSE)
            gl(l);
        else
            throw l;
    }

    void Analyze()
    {
        gl(l);
        P();
    }

};

int main()
{
    Scanner Scanner("ptest.txt");
    Parser Parser;
    try
    {
        Scanner.Scan();
        //Parser.Analyze();
    }
    catch(char c)
    {
        cout << "Incorrect symbol: " << c << endl;
        return 1;
    }
    catch(Lexeme l)
    {
        cout << "Incorrect lexeme: ";
        l.Print_lex();
        cout << endl;
    }
    
    ofstream output("lexems.txt");
    for (int i = 0; i < array_of_lex.size(); i++)
    {
        output << array_of_lex[i].t << "          " << array_of_lex[i].int_value << "          " << array_of_lex[i].str_value << endl;
    }
    output.close();
    
    array_of_lex.clear();
}