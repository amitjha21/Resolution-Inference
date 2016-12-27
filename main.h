//#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdexcept>
#include <cctype>
#include <sstream>
#include <stack>
#include <cstring>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <typeinfo>
#include <list>

#pragma warning(disable:4996)
const char c_imp = '$';
const char c_and = '&';
const char c_or = '|';
const char c_neg = '~';

const std::string s_imp = "$";
const std::string s_and = "&";
const std::string s_or = "|";
const std::string s_neg = "~";
const std::string s_n3 = "~";

const std::string tr = "TRUE";
const std::string fl = "FALSE";
typedef struct node
{
	std::string val;
	node* left = NULL;
	node* right = NULL;
} node;
typedef struct kbNode
{
	std::string val;
	//std::string CNFVal;
	std::deque<std::string> CNFVal;
	std::deque<std::string> Positive;
	std::deque<std::string> Negative;
	//kbNode* left = NULL;
	//kbNode* right = NULL;
} kbNode;

std::string inOrd;
std::map<std::string, kbNode> kbCNFMap;
std::map<std::string, kbNode>::iterator it;
std::map<std::string, kbNode>::iterator it2;
std::pair<std::map<std::string, kbNode>::iterator, bool> ret;
int noOfQ = 0, noOfKB = 0;

std::map<std::string, std::deque<std::string>> vcMap;
//std::map<std::string, std::deque<std::string>> cvMap;
std::map<std::string, std::deque<std::string>>::iterator itvcMap;
//std::map<std::string, std::deque<std::string>>::iterator itcvMap;
typedef struct query
{
	std::string str;
	std::string cnfStr;
}query;
std::vector<query> queryVec(100);
std::vector<query> kbVec(1000);
std::map<std::string, std::size_t> hashMapKB;
std::stack<char> bracketStack;
//std::deque<std::string> predicateQueue;
std::deque<std::string> visitedKB;
std::deque<std::string> ConstantStart;
std::deque<std::string>:: iterator ConstantStartIT;
int c = 1000;
#pragma once
