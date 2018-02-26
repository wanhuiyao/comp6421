#ifndef SCANNER_H
#define SCANNER_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <ostream>
#include <sstream>
#include <iomanip>
#define COL_COUNT 23
#define ROW_COUNT 47

/**
 * Token type definition
 */
enum TokenType{
	ERR_ID,
	ID,
	INT,
	ERR_INT,
	FLOAT,
	ERR_FLOAT,
	ADD,
	SUB,
	MUL,
	DIV,
	CMT,
	DOT,
	COMMA,
	COL,
	DOUBLE_COL,
	SEMI_COL,
	OPEN_PAR,
	CLOSE_PAR,
	OPEN_SQUARE,
	CLOSE_SQUARE,
	OPEN_CURLY,
	CLOSE_CURLY,
	ASSIGN,
	EQUAL,
	GREAT,
	GREAT_EQUAL,
	LESS,
	LESS_EQUAL,
	NOT_EQUAL,
	KEY_WORD,
	ERR
};

/**
 * The structre of Token
 */
class Token
{
public:
	Token(TokenType type, const std::string& value, size_t line) :
		type(type),
		value(value),
		line(line)
	{

	}

public:
	/**
	 * to string
	 */
	std::string toString(const std::unordered_map<int, std::string>& map) const;
    std::string toJson() const;
	std::string toAtoCC(size_t int_idx, size_t float_idx) const;
private:
	TokenType type;
	std::string value;
	size_t line;

    friend class Scanner;
};

/*
 * The structure of a transition table
 */
typedef struct
{
	int collumn[COL_COUNT];
	bool isFinal;
	bool backtrace;
}TransitionTable;


/**
 * DFA class
 */
class DFA
{
public:
	/**
	* CTOR
	*/
	DFA()
	{
		buildCharToCollumnMapping();
	}

	/**
	 * get the state according to the lookup and state
	 */
	int getState(char lookup, int state) const;

	/**
	 * get TokenType from the state
	 */
	TokenType getTokenTypeFromState(int state) const
	{
		auto x = stateToTokenTypeMapping.find(state)->second;
		return x;
		//return stateToTokenTypeMapping.find(state)->second;
	}
	bool isBackup(int state) const
	{
		return table[state - 1].backtrace;

	}
	bool isFinalState(int state) const
	{
		return table[state - 1].isFinal;
	}

private:
	void buildCharToCollumnMapping();

    std::unordered_map<std::string, int> charToCollumnMapping;
    std::unordered_map<int, TokenType> stateToTokenTypeMapping;
	TransitionTable table[ROW_COUNT] = {
        /* state    _   L-e e   D   +   -   *   /   .   ,   :   ;   (   )   [   ]   {   }   =   >   <   SP  CR     isFinal  backtrack
    ------------------------------------------------------------------------------------------------------------------------------*/
        /*1*/   { { 2,  3,  3,  5,  16, 17, 18, 19, 25, 26, 27, 30, 31, 32, 33, 34, 35, 36, 37, 40, 43, 1,  1  },  false,   false },
        /*2*/   { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*3*/   { { 4,  3,  3,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4  },  false,   false },
        /*4*/   { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*5*/   { { 6,  6,  10, 5,  6,  6,  6,  6,  7,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6  },  false,   false },
        /*6*/   { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*7*/   { { 15, 15, 15, 8,  15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },  false,   false },
        /*8*/   { { 9,  9,  10, 8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9  },  false,   false },
        /*9*/   { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*10*/  { { 15, 15, 15, 12, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },  false,   false },
        /*11*/  { { 15, 15, 15, 12, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },  false,   false },
        /*12*/  { { 9,  9,  9,  12, 9,  9,  9,  9,  13, 9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9  },  false,   false },
        /*13*/  { { 15, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },  false,   false },
        /*14*/  { { 9,  9,  9,  14, 9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9  },  false,   false },
        /*15*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*16*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*17*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*18*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*19*/  { { 20, 20, 20, 20, 20, 20, 23, 21, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },  false,   false },
        /*20*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*21*/  { { 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22 },  false,   false },
        /*22*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*23*/  { { 23, 23, 23, 23, 23, 23, 24, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23 },  false,   false },
        /*24*/  { { 23, 23, 23, 23, 23, 23, 23, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23 },  false,   false },
        /*25*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*26*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*27*/  { { 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28 },  false,   false },
        /*28*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*29*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*30*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*31*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*32*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*33*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*34*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*35*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*36*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*37*/  { { 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 39, 38, 38, 38, 38 },  false,   false },
        /*38*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*39*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*40*/  { {41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 41, 42, 41, 41, 41, 41  },  false,   false },
        /*41*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*42*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*43*/  { {44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 44, 45, 44, 46, 44, 44  },  false,   false },
        /*44*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    true  },
        /*45*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*46*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false },
        /*47*/  { { 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1  },  true,    false }
	};
};

/**
 * definition of a scanner
 */
class Scanner
{
public:
	/**
	 * CTOR
	 */
	Scanner(std::string fn){
		std::ifstream is(fn, std::ifstream::in);
		if (!is.good()){
			std::cout << "cannot open file: " << fn << std::endl;
			throw "bad file name";
		}
		is.seekg(0, is.end);
		size_t length = is.tellg();
		is.seekg(0, is.beg);
		buffer = new char[length];
		is.read(buffer, length);
		is.close();
		buildTypeToStringMapping();
		buildKeywordSet();
	}
	/**
	 * scan the buffer and generate all the tokens
	 */
	std::vector<Token*> scan();
	/**
	 * generate next token from current position
	 */
	Token* nextToken(char*& start, size_t& line);

	/**
	 * create Token node according to the provided value, state and line number
	 */
	Token * createToken(char* start, int length, int state, size_t line);
	/**
	 * print the generated Token list
	 */
	void print() const;
	/**
	 * to atocc code
	 */
	std::string getAtoCC() const;
	/**
	 * DCTOR
	 */

    std::string toJson() const;
	~Scanner()
	{
		delete[] buffer;
		for (auto e : tokens){
			delete e;
		}
	}

private:
    /**
    * fill typeToStringMapping
    */
    void buildTypeToStringMapping();
	/**
	 * fill keyWords
	 */
	void buildKeywordSet();
	/**
	 * is string end with 0
	 */
	bool endWithZero(const std::string& str);
	/**
	 * is string end with 0
	 */
	bool beginWithZero(const std::string& str);
	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

	/**
	 * split string into a vector of string with delim
	 */
	std::vector<std::string> split(const std::string &s, char delim);

	/**
	 *is strin a valid float format
	 */
	bool isValidFloat(const std::string& str);

	std::unordered_map<int, std::string> typeToStringMapping;
	std::unordered_set<std::string> keyWords;

	std::vector<Token*> tokens;
	char* buffer;
	DFA dfa;
};

#endif // SCANNER_H
