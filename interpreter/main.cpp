#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
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
    _LBRACE, _RBRACE,                    //30, 31

    //REVERSE POLISH NOTATION(RPN) LEXEMS
    _RPN_LABEL, _RPN_ADDRESS,            //32, 33
    _RPN_GO, _RPN_FGO ,                  //34, 35
};

map <int, string> lex_type_table =
{
    {0, "NULL"}, {1, "program"}, {2, "if"}, {3, "else"}, {4, "while"}, {5, "read"}, {6, "write"}, {7, "int"}, {8, "string"},
    {11, "not"}, {12, "and"}, {13, "or"}, {14, "variable"}, {15, "+"}, {16, "-"}, {17, "*"}, {18, "/"}, {19, "<"}, {20, ">"}, 
    {21, "=="}, {22, "<="}, {23, ">="}, {24, "!="}, {25, "="}, {26, ","}, {27, ";"}, {28, "("}, {29, ")"}, {30, "{"}, {31, "}"},
    {32, "label"}, {33, "address"}, {34, "go"}, {35, "fgo"}
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

class Variable
{
public:
    lex_type type;
    int int_value;
    string str_value;
    Variable(int _int_value = 0)
    {
        type = _INT;
        int_value = _int_value;
        str_value = "";
    }
    Variable(string _str_value)
    {
        type = _STRING;
        int_value = 0;
        str_value = _str_value;
    }
    friend ostream & operator<< ( ostream &s, Variable v)
    {
        if (v.type == _INT)
            s << "int        " << v.int_value;
        else
            s << "string     " << v.str_value;
        return s;
    }

};

map <string, Variable> variables_table;

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
    friend ostream & operator<< ( ostream &s, Lexeme l )
    {
        if (l.t == _INT_VALUE)
            s << l.int_value;
        else if (l.t == _STR_VALUE)
            s << l.str_value;
        else if (l.t == _VAR_NAME)
            s << l.str_value << "     (variable)";
        else if (l.t == _RPN_ADDRESS)
            s << l.str_value << "     (address)";
        else if (l.t == _RPN_LABEL)
            s << "label     " << l.int_value;
        else if (l.t == _WRITE && l.int_value != 0)
            s << "write     (" << l.int_value << " arguments)";
        else
            s << lex_type_table[l.t];
        return s;
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

class Lexical_analyzer
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
    
    Lexical_analyzer(const char* filename)
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

vector <Lexeme> rpn;

class Syntax_analyzer
{
public:
    Lexeme l = Lexeme(_PROGRAM);
    stack <lex_type> st_lex;
    Variable curr;
    string curr_var_name;

    void Parse()
    {
        gl(l);
        P();
    }

    void P() //program
    {
        if (l.t == _PROGRAM)
            gl(l);
        else
            throw l;
        if (l.t == _LBRACE)
            gl(l);
        else
            throw l;
        D();
        B();
        if (l.t != _NULL)
            throw l;
    }

    void D() //few descriptions
    {
        while (l.t == _INT || l.t == _STRING)
        {
            D1();
        }
    }

    void D1() //description
    {
        if (l.t == _INT || l.t == _STRING)
        {
            curr.type = l.t;
            gl(l);
            V();
            while (l.t == _COMMA)
            {
                gl(l);
                V();
            }
        }
        else throw l;
        if (l.t == _SEMICOLON)
        {
            gl(l);
        }
        else throw l;
    }

    void V() //variable
    {
        if (l.t == _VAR_NAME)
        {
            curr_var_name = l.str_value;
            if (variables_table.find(curr_var_name) != variables_table.end())
                throw "Variable declared twice";
            gl(l);
            if (l.t == _ASSIGN)
            {
                gl(l);
                if (l.t == _INT_VALUE || l.t == _STR_VALUE)
                {
                    curr.int_value = l.int_value;
                    curr.str_value = l.str_value;
                    variables_table[curr_var_name] = curr;
                    gl(l);
                }
                else throw l;
            }
            else
            {
                curr.int_value = 0;
                curr.str_value = "";
                variables_table[curr_var_name] = curr;
            }
        }
        else throw l;
    }

    void B() //body - few statements
    {
        while (l.t != _RBRACE)
        {
            S();
        }
        gl(l);
    }

    void S() //statement
    {
        int p0, p1, p2, p3;
        if (l.t == _IF)
        {
            gl(l);
            if (l.t == _LPAREN)
            {
                gl(l);
                E();
                check_int_type();
                p2 = rpn.size();
                rpn.push_back(Lexeme(_NULL));
                rpn.push_back(Lexeme(_RPN_FGO));
                if (l.t == _RPAREN)
                {
                    gl(l);
                    S();
                    p3 = rpn.size();
                    rpn.push_back(Lexeme(_NULL));
                    rpn.push_back(Lexeme(_RPN_GO));
                    rpn[p2] = Lexeme(_RPN_LABEL, rpn.size());
                    if (l.t == _ELSE)
                    {
                        gl(l);
                        S();
                        rpn[p3] = Lexeme(_RPN_LABEL, rpn.size());
                    }
                    else throw l;
                }
                else throw l;
            }
            else throw l;
        }
        else if (l.t == _WHILE)
        {
            gl(l);
            if (l.t == _LPAREN)
            {
                p0 = rpn.size();
                gl(l);
                E();
                check_int_type();
                p1 = rpn.size();
                rpn.push_back(Lexeme(_NULL));
                rpn.push_back(Lexeme(_RPN_FGO));
                if (l.t == _RPAREN)
                {
                    gl(l);
                    S();
                    rpn.push_back(Lexeme(_RPN_LABEL, p0));
                    rpn.push_back(Lexeme(_RPN_GO));
                    rpn[p1] = Lexeme(_RPN_LABEL, rpn.size());
                }
                else throw l;
            }
            else throw l;
        }
        else if (l.t == _READ)
        {
            gl(l);
            if (l.t == _LPAREN)
            {
                gl(l);
                if (l.t == _VAR_NAME)
                {
                    check_id_in_read();
                    rpn.push_back(Lexeme(_RPN_ADDRESS, l.str_value));
                    gl(l);
                    if (l.t == _RPAREN)
                    {
                        gl(l);
                        if (l.t == _SEMICOLON)
                        {
                            gl(l);
                            rpn.push_back(Lexeme(_READ));
                        }
                        else throw l;
                    }
                    else throw l;
                }
                else throw l;
            }
            else throw l;
        }
        else if (l.t == _WRITE)
        {
            int arg_count = 1;
            gl(l);
            if (l.t == _LPAREN)
            {
                gl(l);
                E();
                while (l.t == _COMMA)
                {
                    arg_count++;
                    gl(l);
                    E();
                }
                if (l.t == _RPAREN)
                {
                    gl(l);
                    if (l.t == _SEMICOLON)
                        {
                            gl(l);
                            rpn.push_back(Lexeme(_WRITE, arg_count));
                        }
                    else throw l;
                }
                else throw l;
            }
            else throw l;
        }
        else if (l.t == _VAR_NAME || l.t == _INT_VALUE || l.t == _STR_VALUE)
        {
            E();
            if (l.t == _SEMICOLON)
            {
                gl(l);
            }
            else throw l;
        }
        else if (l.t == _LBRACE)
        {
            gl(l);
            while(l.t != _RBRACE)
            {
                S();
            }
            gl(l);
        }
        else
            throw l;
    }

    void E() //expression
    {
        if (l.t == _VAR_NAME && array_of_lex[ind].t == _ASSIGN)
        {
            check_id();
            rpn.push_back(Lexeme(_RPN_ADDRESS, l.str_value));
            gl(l);
            gl(l);
            E();
            check_assign();
            rpn.push_back(Lexeme(_ASSIGN));
        }
        else
            E1();
    }

    void E1()
    {
        E2();
        while (l.t == _OR)
        {
            st_lex.push(l.t);
            gl(l);
            E2();
            check_op();
        }
    }

    void E2()
    {
        E3();
        while (l.t == _AND)
        {
            st_lex.push(l.t);
            gl(l);
            E3();
            check_op();
        }
    }

    void E3()
    {
        E4();
        while (l.t == _LSS || l.t == _GRT || l.t == _LEQ || l.t == _GEQ ||l.t == _EQL || l.t == _NEQ)
        {
            st_lex.push(l.t);
            gl(l);
            E4();
            check_op();
        }
    }
    
    void E4()
    {
        E5();
        while (l.t == _PLUS || l.t == _MINUS)
        {
            st_lex.push(l.t);
            gl(l);
            E5();
            check_op();
        }
    }
    
    void E5()
    {
        E6();
        while (l.t == _MUL || l.t == _DIV)
        {
            st_lex.push(l.t);
            gl(l);
            E6();
            check_op();
        }
    }
    
    void E6()
    {
        if (l.t == _VAR_NAME)
        {
            check_id();
            rpn.push_back(Lexeme(_VAR_NAME, l.str_value));
            gl(l);
        }
        else if (l.t == _INT_VALUE)
        {
            st_lex.push(_INT);
            rpn.push_back(l);
            gl(l);
        }
        else if (l.t == _STR_VALUE)
        {
            st_lex.push(_STRING);
            rpn.push_back(l);
            gl(l);
        }
        else if (l.t == _NOT)
        {
            gl(l);
            E6();
            check_not();
            rpn.push_back(Lexeme(_NOT));
        }
        else if (l.t == _LPAREN)
        {
            gl(l);
            E();
            if (l.t == _RPAREN)
                gl(l);
            else
                throw l;
        }
        else
            throw l;
    }

    void check_assign()
    {
        lex_type a, b;
        a = st_lex.top();
        st_lex.pop();
        b = st_lex.top();
        st_lex.pop();
        if (a != b)
            throw "Wrong types in operation '='";
        st_lex.push(a);
    }
    
    void check_not()
    {
        if (st_lex.top() != _INT)
            throw "Wrong type for operation 'NOT'";
    }

    void check_op()
    {
        lex_type a, op, b;
        b = st_lex.top();
        st_lex.pop();
        op = st_lex.top();
        st_lex.pop();
        a = st_lex.top();
        st_lex.pop();
        if (a != b)
        {
            throw "Wrong types in operation";
        }
        else if (a != _INT && a != _STRING)
        {
            throw "Wrong types in operation";
        }
        else if ((a == _STRING) && (op == _MINUS || op == _MUL || op == _DIV || op == _AND || op == _OR || op == _LEQ || op == _GEQ))
            throw "This operation isn`t allowed for type 'STRING'";
        if (a == _STRING && op == _PLUS)
            st_lex.push(_STRING);
        else
            st_lex.push(_INT);
        rpn.push_back(op);
    }

    void check_int_type()
    {
        if (st_lex.top() != _INT)
            throw "Expression in IF/WHILE shoud have 'INT' type";
        else
            st_lex.pop();
    }
    
    void check_id()
    {
        if (variables_table.find(l.str_value) != variables_table.end())
            st_lex.push(variables_table[l.str_value].type);
        else
            throw "Variable wasn`t declared";
    }
    
    void check_id_in_read()
    {
        if (variables_table.find(l.str_value) == variables_table.end())
            throw "Variable wasn`t declared";
    }
};

class Interpreter
{
public:
    void Execute()
    {
        Lexeme l = Lexeme(_PROGRAM);
        int index = 0;
        int size = rpn.size();
        int i1 = 0, i2 = 0;
        string s1 = "", s2 = "";
        stack <Variable> operand_stack;
        while (index < size)
        {
            l = rpn[index];
            if (l.t == _INT_VALUE || l.t == _RPN_LABEL)
            {
                operand_stack.push(Variable(l.int_value));
            }
            else if (l.t == _STR_VALUE || l.t == _RPN_ADDRESS)
            {
                operand_stack.push(Variable(l.str_value));
            }
            else if (l.t == _VAR_NAME)
            {
                if (variables_table[l.str_value].type == _INT)
                    operand_stack.push(Variable(variables_table[l.str_value].int_value));
                else
                    operand_stack.push(Variable(variables_table[l.str_value].str_value));
            }
            else if (l.t == _NOT)
            {
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(! i1));
            }
            else if (l.t == _MINUS)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 - i2));
            }
            else if (l.t == _MUL)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 * i2));
            }
            else if (l.t == _DIV)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                if (i2 == 0)
                    throw "Division by zero";
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 / i2));
            }
            else if (l.t == _GEQ)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 >= i2));
            }
            else if (l.t == _GEQ)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 <= i2));
            }
            else if (l.t == _AND)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 && i2));
            }
            else if (l.t == _OR)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                operand_stack.push(Variable(i1 || i2));
            }
            else if (l.t == _RPN_GO)
            {
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                index = i1;
                continue;
            }
            else if (l.t == _RPN_FGO)
            {
                i2 = operand_stack.top().int_value;
                operand_stack.pop();
                i1 = operand_stack.top().int_value;
                operand_stack.pop();
                if (!i1)
                    index = i2;
                else
                    index++;
                continue;
            }
            else if (l.t == _EQL)
            {
                if (operand_stack.top().type == _INT)
                {
                    i2 = operand_stack.top().int_value;
                    operand_stack.pop();
                    i1 = operand_stack.top().int_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(i1 == i2));
                }
                else
                {
                    s2 = operand_stack.top().str_value;
                    operand_stack.pop();
                    s1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(s1 == s2));
                }
            }
            else if (l.t == _NEQ)
            {
                if (operand_stack.top().type == _INT)
                {
                    i2 = operand_stack.top().int_value;
                    operand_stack.pop();
                    i1 = operand_stack.top().int_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(i1 != i2));
                }
                else
                {
                    s2 = operand_stack.top().str_value;
                    operand_stack.pop();
                    s1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(s1 != s2));
                }
            }
            else if (l.t == _GRT)
            {
                if (operand_stack.top().type == _INT)
                {
                    i2 = operand_stack.top().int_value;
                    operand_stack.pop();
                    i1 = operand_stack.top().int_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(i1 > i2));
                }
                else
                {
                    s2 = operand_stack.top().str_value;
                    operand_stack.pop();
                    s1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(s1 > s2));
                }
            }
            else if (l.t == _LSS)
            {
                if (operand_stack.top().type == _INT)
                {
                    i2 = operand_stack.top().int_value;
                    operand_stack.pop();
                    i1 = operand_stack.top().int_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(i1 < i2));
                }
                else
                {
                    s2 = operand_stack.top().str_value;
                    operand_stack.pop();
                    s1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(s1 < s2));
                }
            }
            else if (l.t == _PLUS)
            {
                if (operand_stack.top().type == _INT)
                {
                    i2 = operand_stack.top().int_value;
                    operand_stack.pop();
                    i1 = operand_stack.top().int_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(i1 + i2));
                }
                else
                {
                    s2 = operand_stack.top().str_value;
                    operand_stack.pop();
                    s1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    operand_stack.push(Variable(s1 + s2));
                }
            }
            else if (l.t == _ASSIGN)
            {
                string name1 = "";
                if (operand_stack.top().type == _INT)
                {
                    i2 = operand_stack.top().int_value;
                    operand_stack.pop();
                    name1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    variables_table[name1].int_value = i2;
                    operand_stack.push(Variable(i2));
                }
                else
                {
                    s2 = operand_stack.top().str_value;
                    operand_stack.pop();
                    name1 = operand_stack.top().str_value;
                    operand_stack.pop();
                    variables_table[name1].str_value = s2;
                    operand_stack.push(Variable(s2));
                }
            }
            else if (l.t == _WRITE)
            {
                stack <Variable> write_stack;
                for (int j = 0; j < l.int_value; j++)
                {
                    write_stack.push(operand_stack.top());
                    operand_stack.pop();
                }

                for (int j = 0; j < l.int_value; j++)
                {
                    if (write_stack.top().type == _INT)
                    {
                        cout << write_stack.top().int_value << endl;
                        write_stack.pop();
                    }
                    else
                    {
                        cout << write_stack.top().str_value << endl;
                        write_stack.pop();
                    }
                }
            }
            else if (l.t == _READ)
            {
                string name1 = "";
                name1 = operand_stack.top().str_value;
                operand_stack.pop();
                if (variables_table[name1].type == _INT)
                {
                    cin >> i1;
                    variables_table[name1].int_value = i1;
                }
                else
                {
                    cin >> s1;
                    variables_table[name1].str_value = s1;
                }
            }
            else
                throw "Unexpected elem in reverse polish notation";
            index++;
        }
    }
};

int main(int argc, char** argv)
{
    Lexical_analyzer L(argv[1]);
    Syntax_analyzer S;
    Interpreter I;
    try
    {
        L.Scan();
        S.Parse();
        I.Execute();
    }
    catch(char c)
    {
        cout << "Incorrect symbol: " << c << endl;
        return 1;
    }
    catch(Lexeme l)
    {
        cout << "Incorrect lexeme: " << l << endl;
        return 1;
    }
    catch(const char* error_message)
    {
        cout << error_message << endl;
        return 1;
    }
    
    ofstream output1("lexems.txt");
    for (int i = 0; i < array_of_lex.size(); i++)
        output1 << array_of_lex[i] << endl;
    output1.close();

    ofstream output2("variables.txt");
    for (auto it = variables_table.begin(); it != variables_table.end(); ++it)
    {
        output2 << it->first << "     " << it->second << endl;
    }
    output2.close();

    ofstream output3("rpn.txt");
    for (int i = 0; i < rpn.size(); i++)
        output3 << rpn[i] << endl;
    output3.close();
    
    array_of_lex.clear();
}