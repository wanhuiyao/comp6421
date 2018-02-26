#include "scanner.h"


std::string Token::toString(const std::unordered_map<int, std::string>& map) const
{
	std::ostringstream ss;
	ss << std::left << std::setw(50) << value << "line: " << std::setw(10) << line << "type: " << map.find(type)->second;
	return ss.str();
}

std::string Token::toJson() const
{
    std::string x;
    switch(type)
    {
    case ERR_ID:
    case ID:
        x = "id";
        break;
    case INT:
    case ERR_INT:
        x = "intNum";
        break;
    case FLOAT:
    case ERR_FLOAT:
        x = "floatNum";
        break;
    case ADD:
        x = "+";
        break;
    case SUB:
        x = "-";
        break;
    case MUL:
        x = "*";
        break;
    case DIV:
        x = "/";
        break;
    case CMT:
        x = "cmt";
        break;
    case DOT:
        x = ".";
        break;
    case COMMA:
        x = ",";
        break;
    case COL:
        x = ":";
        break;
    case DOUBLE_COL:
        x = "sr";
        break;
    case SEMI_COL:
        x = ";";
        break;
    case OPEN_PAR:
        x = ")";
        break;
    case CLOSE_PAR:
        x = ")";
        break;
    case OPEN_SQUARE:
        x = "[";
        break;
    case CLOSE_SQUARE:
        x = "]";
        break;
    case OPEN_CURLY:
        x = "{";
        break;
    case CLOSE_CURLY:
        x = "}";
        break;
    case ASSIGN:
        x = "=";
        break;
    case EQUAL:
        x = "eq";
        break;
    case GREAT:
        x = "gt";
        break;
    case GREAT_EQUAL:
        x = "geq";
        break;
    case LESS:
        x = "lt";
        break;
    case LESS_EQUAL:
        x = "leq";
        break;
    case NOT_EQUAL:
        x = "neq";
        break;
    case KEY_WORD:
        x = value;
        break;
    case ERR:
        x = "err";
        break;
    }
    std::ostringstream ss;
    ss << "\t{" << std::endl;
    ss << "\t\t\"type\" : " << "\"" << x << "\"," << std::endl;
    ss << "\t\t\"value\" : " << "\"" << value << "\"," << std::endl;
    ss << "\t\t\"line\" : " << line << std::endl;
    ss << "\t}";
    return ss.str();
}

std::string Token::toAtoCC(size_t int_idx, size_t float_idx) const
{
	if (type == ERR_ID || type == ERR_INT || type == ERR_FLOAT || type == ERR || type == CMT)
	{
		return "";
	}
	else
	{
		if(type == INT)
		{
			std::ostringstream ss;
			ss << "int_" << int_idx << " ";
			return ss.str();
		}
		if(type == FLOAT)
		{
			std::ostringstream ss;
			ss << "float_" << float_idx << " ";
			return ss.str();
		}
		return value + " ";
	}
}

int DFA::getState(char lookup, int state) const
{
	std::string key;
	if (isdigit(static_cast<unsigned char>(lookup)))
	{
		key = "d";
	}
	else if (lookup == ' ')
	{
		key = "SP";
	}
	else if (lookup == '\n')
	{
		key = "CR";
	}
	else if (lookup == 'e')
	{
		key = "e";
	}
	else if (isalpha(static_cast<unsigned char>(lookup)))
	{
		key = "l-e";
	}
	else
	{
		key += lookup;
	}

	std::unordered_map<std::string, int>::const_iterator got = charToCollumnMapping.find(key);
	if (got == charToCollumnMapping.end())
	{
		std::cout << "wrong mapping" << std::endl;
		return 47;
	}
	else
	{
		return table[state - 1].collumn[got->second];
	}
}

void DFA::buildCharToCollumnMapping(){
	for (auto name : { "_", "l-e", "e", "d", "+", "-", "*", "/", ".", ",", ":", ";", "(", ")", "[", "]", "{", "}", "=", ">", "<", "SP", "CR" })
	{
		static auto index = 0;
		charToCollumnMapping[name] = index++;
	}
	stateToTokenTypeMapping[2] = ERR_ID;
	stateToTokenTypeMapping[4] = ID;
	stateToTokenTypeMapping[6] = INT;
	stateToTokenTypeMapping[9] = FLOAT;
	stateToTokenTypeMapping[15] = ERR_FLOAT;
	stateToTokenTypeMapping[16] = ADD;
	stateToTokenTypeMapping[17] = SUB;
	stateToTokenTypeMapping[18] = MUL;
	stateToTokenTypeMapping[20] = DIV;
	stateToTokenTypeMapping[22] = CMT;
	stateToTokenTypeMapping[25] = DOT;
	stateToTokenTypeMapping[26] = COMMA;
	stateToTokenTypeMapping[28] = COL;
	stateToTokenTypeMapping[29] = DOUBLE_COL;
	stateToTokenTypeMapping[30] = SEMI_COL;
	stateToTokenTypeMapping[31] = OPEN_PAR;
	stateToTokenTypeMapping[32] = CLOSE_PAR;
	stateToTokenTypeMapping[33] = OPEN_SQUARE;
	stateToTokenTypeMapping[34] = CLOSE_SQUARE;
	stateToTokenTypeMapping[35] = OPEN_CURLY;
	stateToTokenTypeMapping[36] = CLOSE_CURLY;
	stateToTokenTypeMapping[38] = ASSIGN;
	stateToTokenTypeMapping[39] = EQUAL;
	stateToTokenTypeMapping[41] = GREAT;
	stateToTokenTypeMapping[42] = GREAT_EQUAL;
	stateToTokenTypeMapping[44] = LESS;
	stateToTokenTypeMapping[45] = LESS_EQUAL;
	stateToTokenTypeMapping[46] = NOT_EQUAL;
	stateToTokenTypeMapping[47] = ERR;
}
std::vector<Token*> Scanner::scan()
{
	size_t line = 1;
	char* cursor = buffer;
	tokens.clear();
	while (*cursor)
	{
		if (*cursor == ' ')
		{
			cursor++;
			continue;
		}
		if (*cursor == '\n')
		{
			cursor++;
			line++;
			continue;
		}
		tokens.push_back(nextToken(cursor, line));
	}
	return tokens;
}

Token* Scanner::nextToken(char*& start, size_t& line)
{
	int state = 1;
	char *cursor = start;
	Token *token = nullptr;
	auto newLine = line;
	do{
		char lookup = *cursor++;

		if (lookup == '\n')
			newLine++;

		//std::cout << "state: " << state << "   apply " << lookup << " --> " << dfa.getState(lookup, state) << std::endl;
		state = dfa.getState(lookup, state);

		if (dfa.isFinalState(state))
		{
            //std::cout << "is fininal" << std::endl;
			if (dfa.isBackup(state)){
                //std::cout << "is backup" << std::endl;
				cursor--;
			}
            //std::cout << "will create token: <" << cursor - start << " state: " << state << std::endl;
			token = createToken(start, cursor - start, state, line);
            //std::cout << token->toString(typeToStringMapping) << std::endl;
			break;
		}
	} while (*cursor);
	start = cursor;
	line = newLine;
	return (token);
}

Token * Scanner::createToken(char* start, int length, int state, size_t line){
	std::string value(start, length);
	auto tp = dfa.getTokenTypeFromState(state);
	if (tp == ID && keyWords.find(value) != keyWords.end())
		tp = KEY_WORD;

	if (tp == INT)
	{
		if (beginWithZero(value))
		{
			tp = ERR_INT;
		}
	}

	if (tp == FLOAT)
	{
		if (!isValidFloat(value))
		{
			tp = ERR_FLOAT;
		}
	}
	return new Token{ tp, value, line };
}

std::string Scanner::getAtoCC() const
{
	size_t int_cout = 0, float_count = 0;
	std::string atoccString = "";
	for (auto e: tokens){
		atoccString += e->toAtoCC(int_cout, float_count);
	}
	return atoccString;
}

std::string Scanner::toJson() const
{
    std::ostringstream ss;
    ss << "[" << std::endl;
    bool addComma = false;
    for (auto e : tokens)
    {
        if (e->type == ERR_ID
                || e->type == ERR_INT
                || e->type == ERR_FLOAT
                || e->type == ERR
                || e->type == CMT)
        {
            continue;
        }
        if(addComma)
        {
            ss << "," << std::endl;
        }
        else
        {
            addComma = true;
        }
        ss << e->toJson();
    }
    ss << "\n]";
    return ss.str();
}

void Scanner::print() const
{
	for (auto e : tokens)
	{
		std::cout << e->toString(typeToStringMapping) << std::endl;
		std::cout << "-----------------------------------------------------------------------------------------------------------------------------" << std::endl;
	}
}

void Scanner::buildTypeToStringMapping()
{
	typeToStringMapping[static_cast<int>(ERR_ID)] = "invalid id";
	typeToStringMapping[static_cast<int>(ID)] = "id";
	typeToStringMapping[static_cast<int>(INT)] = "int";
	typeToStringMapping[static_cast<int>(ERR_INT)] = "invalid int";
	typeToStringMapping[static_cast<int>(FLOAT)] = "float";
	typeToStringMapping[static_cast<int>(ERR_FLOAT)] = "invalid float";
	typeToStringMapping[static_cast<int>(ADD)] = "add";
	typeToStringMapping[static_cast<int>(SUB)] = "subtract";
	typeToStringMapping[static_cast<int>(MUL)] = "multiply";
	typeToStringMapping[static_cast<int>(DIV)] = "divide";
	typeToStringMapping[static_cast<int>(CMT)] = "comment";
	typeToStringMapping[static_cast<int>(DOT)] = "dot";
	typeToStringMapping[static_cast<int>(COMMA)] = "comma";
	typeToStringMapping[static_cast<int>(COL)] = "colon";
	typeToStringMapping[static_cast<int>(DOUBLE_COL)] = "double colon";
	typeToStringMapping[static_cast<int>(SEMI_COL)] = "semicolon";
	typeToStringMapping[static_cast<int>(OPEN_PAR)] = "open parenthesis";
	typeToStringMapping[static_cast<int>(CLOSE_PAR)] = "close parenthesis";
	typeToStringMapping[static_cast<int>(OPEN_SQUARE)] = "open square brackets";
	typeToStringMapping[static_cast<int>(CLOSE_SQUARE)] = "close square brackets";
	typeToStringMapping[static_cast<int>(OPEN_CURLY)] = "open curly brackets";
	typeToStringMapping[static_cast<int>(CLOSE_CURLY)] = "close curly brackets";
	typeToStringMapping[static_cast<int>(ASSIGN)] = "assign";
	typeToStringMapping[static_cast<int>(EQUAL)] = "equal";
	typeToStringMapping[static_cast<int>(GREAT)] = "great";
	typeToStringMapping[static_cast<int>(GREAT_EQUAL)] = "great or equal";
	typeToStringMapping[static_cast<int>(LESS)] = "less";
	typeToStringMapping[static_cast<int>(LESS_EQUAL)] = "less or equal";
	typeToStringMapping[static_cast<int>(NOT_EQUAL)] = "not equal";
	typeToStringMapping[static_cast<int>(KEY_WORD)] = "key word";
	typeToStringMapping[static_cast<int>(ERR)] = "not supported ";

}
void Scanner::buildKeywordSet()
{
	for (auto e : { "not", "and", "or", "if", "then", "else", "for", "class", "int", "float", "get", "put", "return", "program" })
	{
		keyWords.insert(e);
	}
}

bool Scanner::endWithZero(const std::string& str)
{
	size_t length = str.length();
	if (length > 0 && str.at(length - 1) == '0')
		return true;
	return false;
}
/**
* is string end with 0
*/
bool Scanner::beginWithZero(const std::string& str)
{
	size_t length = str.length();
	if (length > 0 && str.at(0) == '0')
		return true;
	return false;
}

std::vector<std::string> & Scanner::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

/**
* split string into a vector of string with delim
*/
std::vector<std::string> Scanner::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

/**
*is strin a valid float format
*/
bool Scanner::isValidFloat(const std::string& str)
{
	std::vector<std::string> finalFields;
	auto scien_fileds = split(str, 'e');
	for (auto field : scien_fileds)
	{
		if (field.length() == 0) continue;
		auto ff = split(field, '-');
		for (auto fff : ff) {
			if (fff.length() == 0) continue;
			auto dot_p = fff.find('.');
			if (dot_p == std::string::npos) //no . in str
			{
				if (beginWithZero(fff) && fff.length() > 0)
					return false;
			}
			else{
				if (endWithZero(fff))
					return false;
				if (dot_p != 1 && beginWithZero(fff))
					return false;
			}
		}
	}
	return true;
}
