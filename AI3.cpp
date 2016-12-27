
#include "stdafx.h"
#include "main.h"

void outputFile(std::string op[])
{
	std::ofstream outfile;
	outfile.open("output.txt");
	if (outfile.is_open())
	{
		for (int k = 0; k < noOfQ; ++k) {
			outfile << op[k];
			outfile << std::endl;
		}
		outfile.close();
	}
	else
		std::cout << "Unable to open file";
	delete[] op;
}

bool isOperator(char currentChar)
{
	switch (currentChar)
	{
	case '$': //=>
	case '|':
	case '&':
	case '#':
	case '~':
		//case '^':
		//case '%':
		return true;
	default:
		return false;
	}
}

// returns whether a `lOp` b `rOp` c == (a `lOp` b) `rOp` c
bool precedence(char leftOperator, char rightOperator)
{
	if (leftOperator == '~')
	{
		return false;
	}
	if (rightOperator == '~')
	{
		return true;
	}

	else if (leftOperator == '&')//|| leftOperator == '/' || leftOperator == '%' )
	{
		return true;
	}
	else if (rightOperator == '&')// || rightOperator == '/' || rightOperator == '%' )
	{
		return false;
	}
	else if (leftOperator == '|')
	{
		return true;
	}
	else if (rightOperator == '|')
	{
		return false;
	}
	else if (leftOperator == '$')
	{
		return true;
	}
	else if (rightOperator == '$')
	{
		return false;
	}
	return true;
}
bool funBracket(char curr, char prev, char next)
{

	int p = (int)prev;
	int n = (int)next;
	if (curr == '(')
	{
		if (((p >= 97 && p <= 122) || (p >= 65 && p <= 90)) && ((n >= 97 && n <= 122) || (n >= 65 && n <= 90)))
		{
			bracketStack.push('(');
			return true;
		}
	}
	else if (curr == ')')
	{
		if (!bracketStack.empty())
		{
			bracketStack.pop();
			return true;
		}
	}

	return false;
}

std::string convertToPostfix(const std::string& infix)
{
	std::stringstream postfix; // Our return string
	std::stack<char> stack;
	stack.push('('); // Push a left parenthesis ‘(‘ onto the stack.
	bracketStack = std::stack<char>();
	int k1, k2;
	for (std::size_t i = 0, l = infix.size(); i < l; ++i)
	{
		const char current = infix[i];
		if (i == 0) {
			k1 = 1; 
			k2 = -1;
		}
		else {
			k1 = i;
			k2 = i;
		}
		if (isspace(current))
		{
			// ignore
		}
		// If it's a digit or '.' or a letter ("variables"), add it to the output
		else if (isalnum(current) || current == '.' || current == ',' || current == '~' || funBracket(current, infix[k1 - 1], infix[k2 + 1]))
		{
			postfix << current;
		}

		else if ('(' == current)
		{
			stack.push(current);
		}

		else if (isOperator(current))
		{
			char rightOperator = current;
			while (!stack.empty() && isOperator(stack.top()) && precedence(stack.top(), rightOperator))
			{
				postfix << ' ' << stack.top();
				stack.pop();
			}
			postfix << ' ';
			stack.push(rightOperator);
		}

		// We've hit a right parens
		else if (')' == current)
		{
			// While top of stack is not a left parens
			while (!stack.empty() && '(' != stack.top())
			{
				postfix << ' ' << stack.top();
				stack.pop();
			}
			if (stack.empty())
			{
				throw std::runtime_error("missing left paren");
			}
			// Discard the left paren
			stack.pop();
			postfix << ' ';
		}
		else
		{
			// throw std::runtime_error("invalid input character");
		}
	}


	// Started with a left paren, now close it:
	// While top of stack is not a left paren
	while (!stack.empty() && '(' != stack.top())
	{
		postfix << ' ' << stack.top();
		stack.pop();
	}
	if (stack.empty())
	{
		throw std::runtime_error("missing left paren");
	}
	// Discard the left paren
	stack.pop();

	// all open parens should be closed now -> empty stack
	if (!stack.empty())
	{
		throw std::runtime_error("missing right paren");
	}

	return postfix.str();
}
node* createNode()
{
	node* temp;
	temp = new node;
	return temp;
}

bool isSymbol(std::string currentStr)
{
	if (currentStr == "$" || currentStr == "|" || currentStr == "&" || currentStr == "~")
		return true;
	else
		return false;

}

node* postToBT(std::string inputString)
{
	//std::string inputString;

	node *p, *root;
	std::stack<node*> myStack;
	//std::getline(std::cin, inputString);
	//cout << inputString;
	//const char *cstr = inputString.c_str();
	char *is = new char[inputString.length() + 1];
	strcpy(is, inputString.c_str());

	//std::vector<char*> is(inputString.c_str(), inputString.c_str() + inputString.size() + 1);
	char *tempS = strtok(is, " ");

	while (tempS != NULL)
	{
		if (isSymbol(tempS))
		{
			p = createNode();
			p->val = tempS;
			p->right = myStack.top();
			myStack.pop();
			if (tempS[0] != '~')
			{
				p->left = myStack.top();
				myStack.pop();
			}
			myStack.push(p);
		}
		else
		{
			p = createNode();
			p->val = tempS;
			myStack.push(p);
		}

		tempS = strtok(NULL, " ");
	}
	root = myStack.top();
	myStack.pop();
	return root;
}
node* negateVal(node* p)
{

	p->val.erase(remove_if(p->val.begin(), p->val.end(), isspace), p->val.end());
	if (p->val == "N3" + s_and || p->val == s_neg + s_and)
		p->val = s_or;
	else if (p->val == "N3" + s_or || p->val == s_neg + s_or)
		p->val = s_and;
	else if (p->val.substr(0, 4) == "N3N3" || p->val.substr(0, 4) == "~~")
		p->val = p->val.substr(5);

	return p;
}
node* CNF(node *start)
{
	//    node *p;
	if (start->left == NULL && start->right == NULL)
		return start;
	if (start->val == s_imp || start->val == s_n3 + s_imp)
	{
		start->val = s_or;
		if (start && start->left)
			start->left->val = s_n3 + start->left->val;
		if (start && start->right)
			start->right->val = start->right->val;
		if (start && start->left)
			start = CNF(start->left);

		if (start && start->right)
			start = CNF(start->right);
	}
	else if (start->val == s_n3 + s_and || start->val == s_n3 + s_or)
	{
		start = negateVal(start);
		if (start && start->left)
			start->left->val = s_n3 + start->left->val;
		if (start && start->right)
			start->right->val = s_n3 + start->right->val;
		if (start && start->left)
			start = CNF(start->left);
		if (start && start->right)
			start = CNF(start->right);
	}
	else if (start->val == s_and || start->val == s_or)
	{
		if (start && start->left)
			start = CNF(start->left);
		if (start && start->right)
			start = CNF(start->right);
	}
	else if (start->val == s_neg)  //added later for '~'
	{
		if (start && start->right)
		{
			start->right->val = s_neg + start->right->val;
			start->val = " ";
			start = CNF(start->right);
		}
	}
	else//
		return start;
}
std::string param(std::string predicate) 
{
	std::string ss="";
	if (!predicate.empty()) {
		if (predicate.find('(')!= std::string::npos) {
			 ss = predicate.substr(predicate.find('(') + 1);
			ss.pop_back();
		}
	}
	return ss;
}

std::string predicateName(std::string predicate)
{
	std::string ss = "";
	if (!predicate.empty()) {
		if (predicate.front() == '~')
			predicate.erase(0, 1);
		if (predicate.find('(') != std::string::npos) {
		    ss = predicate.substr(0,predicate.find('('));
			//ss.pop_back();
		}
		else
		ss = predicate;
	}
	return ss;
}

void prepareMap(std::string str)
{
	//
	std::string tempStr = str;
	char *datat, *pch;
	datat = (char*)tempStr.c_str();
	pch = strtok(datat, "$|&");
	while (pch != NULL)
	{
		std::string s(pch);
		char p = ' ';
		kbNode node;
		//node.CNFVal = str;
		node.val = s;
		//remove_if(s.begin(), s.end(), isspace);
		s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
		node.CNFVal.push_back(str);
		/*if (s.front() == '~')
		{
			p = 'N';
			//s.erase(0,1);
			node.val = s;
			node.Negative.push_back(str);
			node.CNFVal.push_back(str);
		}
		else
		{
			p = 'P';
			node.val = s;
			node.Positive.push_back(str);
		}
		*/
		it = kbCNFMap.find(s);
		if (it != kbCNFMap.end())   //exists
		{
			it->second.CNFVal.push_back(str); // : it->second.Negative.push_back(str);
			//p == 'P' ? it->second.Positive.push_back(str) : it->second.Negative.push_back(str);

		}

		else          //does'nt existd
		{

			kbCNFMap.insert(std::pair<std::string, kbNode>(s, node));
		}
		pch = strtok(NULL, "$|&");
	}

}
bool myfunctionSort(std::string i, std::string j) { return (i<j); }

void insertvcMap(std::string p1, std::string p2)
{	
	std::string key, value;
	key = p1;
	value = p2;
	for (int i = 0; i < 2; ++i) {
		std::deque<std::string> valDQ;
		itvcMap = vcMap.find(key);
		if (itvcMap != vcMap.end())   //exists
		{
			if(std::find(itvcMap->second.begin(),itvcMap->second.end(),value)!=itvcMap->second.end())
			{ }
			else
				itvcMap->second.push_back(value);
		}
		else          //does'nt exist
		{
			valDQ.push_back(value);
			vcMap.insert(std::pair<std::string, std::deque<std::string>>(key, valDQ));
		}
		key = p2;
		value = p1;
	}
}

std::string getIndiPara(std::string& strP)
{
	std::string res = "";
	std::string::size_type commaPos;
	if (!strP.empty()) {
		commaPos = strP.find(',');
		if (commaPos != std::string::npos)
		{
			res = strP.substr(0, commaPos);
			strP.erase(0, commaPos+1);
		}
		else
		{
			res = strP;
			strP = "";
		}
			
	}
	return res;
}
bool bothCons(std::string c1, std::string c2)
{
	if (isupper(c1.front()) && isupper(c2.front()))
		return true;
	return false;
}

void prepareVCMap()
{
	std::string param1,param2;
	for (it = kbCNFMap.begin(); it != kbCNFMap.end(); ++it)
	{
		it2 = it;
		++it2;
		for (; it2 != kbCNFMap.end(); ++it2)
		{
			if (predicateName(it->first) == predicateName(it2->first))
			{
				param1 = param(it->first);
				param2 = param(it2->first);
				if (param1 != param2)
				{
					insertvcMap(param1, param2);
					//if multiple variables then add to the kb
					while (!param1.empty() && !param2.empty())
					{
						std::string p1 = getIndiPara(param1);
						std::string p2 = getIndiPara(param2);
						if(!p1.empty() && !p2.empty() && p1!=p2)
						insertvcMap(p1, p2);
					}
				}

			}
		}
	}
	//sort by value in deque
	for (itvcMap = vcMap.begin(); itvcMap != vcMap.end(); itvcMap++)
	{
		std::sort(itvcMap->second.begin(), itvcMap->second.end(), myfunctionSort);
	}
	
}

void inOrderDisplay(node *n)
{
	if (!n) return;
	inOrderDisplay(n->left);
	//std::cout<< n->val << " ";
	inOrd = inOrd.empty() ? n->val : inOrd + " " + n->val;
	inOrderDisplay(n->right);
}
std::size_t hashVal(std::string str)
{
	std::hash<std::string> hash_fn;

	return hash_fn(str);
}

void myReplace(std::string& str,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos)
	{
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}
std::string ToString(std::size_t sz)
{
	std::stringstream ss;
	ss << sz;
	return ss.str();
}

std::string negatePredicate(std::string str)
{
	std::string res = str.front() == '~' ? str.erase(0, 1) : str.insert(0, 1, '~');
	return res;
}
std::string removeExtraSymbol(std::string str)
{
	std::stringstream ss;
	char curr;
	
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	if (str.size() <= 0) 
		return "";
	
	if (str.front() == '|')
		str.erase(0, 1);
	else if (str.back() == '|')
		str.pop_back();
	
	for (unsigned int i = 0; i<str.length(); ++i)
	{
		curr = str[i];
		if (curr == '|')
		{
			if (ss.str() == " ")
			{
				str.erase(i, 1);
			}
			else
				ss.str("");
		}
		else
			ss << curr;

	}
	return str;
}

std::deque<std::string> pushIntoDq(std::string pred, std::deque<std::string> dq)
{
	char *pch = strtok((char*)pred.c_str(), "|&");
	while (pch != NULL)
	{
		std::string s(pch);
		dq.push_back(s);
		pch = strtok(NULL, "=>|&");  //#pragma warning(disable : 4996)
	}
	return dq;
}

bool extraNegSign(std::string pred, std::string::size_type findPos)
{
	if (findPos > 0) {
		if (pred[findPos - 1] != '~')
			return true;
		else
			return false;
	}
	return true;
}

std::string myreplace(std::string s,
	const std::string &toReplace,
	const std::string &replaceWith)
{
	return(s.replace(s.find(toReplace), toReplace.length(), replaceWith));
}

char cOrv(std::string var)
{
	if (isupper(var.front())) return 'C';
	else return 'V';
}
std::string getConstant(std::string var)
{
	int end = ConstantStart.size() - 1;
	for (int i = end; i > 0; i--) {
		if (ConstantStart[i] == var)
		{
			for (int j = i - 1; j >= 0; j--)
			{
				if (cOrv(ConstantStart[j]) == 'C')
					return ConstantStart[j];
			}
		}
	}
	return ConstantStart[0];
}

//new resolution
bool resolutionNew(std::string query)
{
	std::string pred, negPred;
	std::string::size_type findPos;
	std::deque<std::string> predicateQueue;
	
	bool predSearch = false;
	--c;
	if (c < 0)
		return false;
	if (query.empty())
		return true;

		predSearch = false;
		negPred = negatePredicate(query);
		it = kbCNFMap.find(negPred);
		if (it != kbCNFMap.end())
		{
			//negPred = negatePredicate(query); //get negative of query to cancel
			//without substituion 
			for (unsigned int i = 0; i < it->second.CNFVal.size(); i++)
			{
				pred = it->second.CNFVal[i];
				findPos = pred.find(negPred);
				if (findPos != std::string::npos && extraNegSign(pred, findPos))
				{
					pred.replace(pred.find(negPred), negPred.length(), " ");
					pred = removeExtraSymbol(pred);
					predSearch = true;
					query = "";
				    if(predicateQueue.size()>0)
						predicateQueue.pop_front(); // pop the front if true 
					if (pred != "")
						predicateQueue = pushIntoDq(pred, predicateQueue); // predicateQueue	
					std::string frontQ = predicateQueue.size() >0 ? predicateQueue.front() : "";
					return resolutionNew(frontQ);
					break;
				}
			}
		}
		//with substitution 
		if (!predSearch)
		{
			std::string varibles = param(query);   //get parameter list
			std::deque<std::string> vcmapQ = vcMap[varibles];
			for (int i = 0; i < vcmapQ.size(); i++)
			{
				//don't assign Const = var
				if (cOrv(varibles) == 'C' && cOrv(vcmapQ[i]) == 'V')// || (cOrv(varibles) == 'C' && cOrv(vcmapQ[i]) == 'C')) {
				{	
					ConstantStart.push_back(varibles);
					ConstantStart.push_back(vcmapQ[i]);
					//continue;
				}
				if (cOrv(varibles) == 'V' && cOrv(vcmapQ[i]) == 'V')
				{
					if (ConstantStart.size() > 0)
					{
						std::deque<std::string> newVarQ = vcMap[vcmapQ[i]];
						std::string s1= getConstant(varibles);
						if (std::find(newVarQ.begin(), newVarQ.end(), s1) != newVarQ.end())
						{

						}
						else
							continue;
					}
				}
				std::string qyeryb4Replace = query;
					query = myreplace(query, varibles, vcmapQ[i]);
					
					if (std::find(visitedKB.begin(), visitedKB.end(), query) != visitedKB.end())
					{ 
						query = qyeryb4Replace;
						continue;
					}
					else
					{
						visitedKB.push_back(query);
					bool r = resolutionNew(query);
					return r;
				}
				
					
					
						//return false;			
			}			
		}	
		
		return false;
}


int main()
{
	kbNode kb1;
	int counter1 = -1, counter2 = -1, fileLineCount = 0;
	//char *datat,*pch;
	
	std::string data = "";
	std::ifstream infile;
	infile.open("input.txt");
	if (infile.is_open())
	{
		while (getline(infile, data))
		{
			//cout << data << endl;

			++fileLineCount;

			if (fileLineCount == 1)
			{
				noOfQ = atoi(data.c_str());

			}
			else if (fileLineCount <= noOfQ + 1)
			{
				++counter1;
				data.erase(remove_if(data.begin(), data.end(), isspace), data.end());
				queryVec[counter1].str = data;

			}
			else if (fileLineCount == noOfQ + 2)
			{
				noOfKB = atoi(data.c_str());
			}
			else
			{
				myReplace(data, "=>", "$");
				//kb1.OriginalVal = data;
				//myMap.insert ( std::make_pair("Ancestor", kb1) );
				// datat = (char*)data.c_str();

				// pch = strtok (datat,"=>|&");
				/*while (pch != NULL)
				{     std::string s(pch);
				if(s[0]=='('|| s[0]==' ')
				s.erase(0,1);
				int lastIdx = s.size()-1;
				//  if(s[lastIdx]==')'|| s[lastIdx]==' ')
				//      s.erase(lastIdx,1);
				if(hashMapKB[s]==NULL){
				//hashMapKB[s] = hashVal(s);
				size_t hash = hashVal(s);
				hashMapKB[s]  = hash;
				std::cout<<"hash of:"<<s<<":-"<<hashMapKB[s]<<std::endl;
				myReplace(data,s,ToString(hashMapKB[s]));
				}
				pch = strtok (NULL, "=>|&");
				}*/
				++counter2;
				data.erase(remove_if(data.begin(), data.end(), isspace), data.end());
				kbVec[counter2].str = data;
			}

			data = "";
		}
	}
	infile.close();
	queryVec.resize(noOfQ);
	queryVec.shrink_to_fit();
	kbVec.resize(noOfKB);
	kbVec.shrink_to_fit();
	std::vector<query>(queryVec.begin(), queryVec.end()).swap(queryVec);
	std::vector<query>(kbVec).swap(kbVec);
	//  std::cout<<queryVec[0].str;
	for (unsigned int k = 0; k<kbVec.size(); ++k)
	{
		std::string infix, postOutput, postNegInput, postNegOutput;
		node *root, *cnfRet;
		infix = kbVec[k].str;
		inOrd = "";
		if (infix.empty()) continue;
		postOutput = convertToPostfix(infix);
		std::cout << "Postfix: " << postOutput << "'\n";
		root = postToBT(postOutput);
		cnfRet = CNF(root);
		inOrderDisplay(root);
		//      std::cout<<"inord:"<<inOrd<<"\n\n";
		for (unsigned int i = 0; i< inOrd.length(); ++i)
		{
			//std::size_t pos = infix.find("~");
			if (inOrd[i] == '~' && inOrd[i + 1] == '~' && inOrd[i + 2] == '~' && inOrd[i + 3] == '~')
			{
				postNegOutput = inOrd.replace(i, 4, " ");
				i = 0;
			}
			else if (inOrd[i] == '~' && inOrd[i + 1] == '~')
			{
				postNegOutput = inOrd.replace(i, 2, " ");
				i = 0;
			}

		}

		kbVec[k].cnfStr = postNegOutput.empty() ? inOrd : postNegOutput;
		std::cout << "kb CNF:" << kbVec[k].cnfStr << std::endl;
		prepareMap(kbVec[k].cnfStr);
	}
	//Prepare varible to constant hash and vice versa hash
	prepareVCMap();
	
	std::cout << "Mera Bharat:\n\n\n";
	
	std::string *result = new std::string[noOfQ];
	for (int i = 0; i<queryVec.size(); ++i)
	{
		//set c to 1000 for each query
		c = 20;
		visitedKB.clear();
		ConstantStart.clear();
		//predicateQueue.clear();
		std::string q = queryVec[i].str;
		std::cout << "\n" << q << " :  " ;
		if (resolutionNew(negatePredicate(q)))
		{
			std::cout << tr;
			result[i] = tr;
			prepareMap(q); //Add to the KB map as it is true
		}
		else {
			std::cout << fl;
			result[i] = fl;
		}
		std::cout << std::endl;
	}
	//create output.txt
	outputFile(result);   //uncomment before submission

	return 0;
}
