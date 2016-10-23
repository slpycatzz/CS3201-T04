#include <string>
#include <vector>
#include <unordered_map>

#include "stdafx.h"
#include "targetver.h"

#include "Constants.h"
#include "QueryProcessor/QueryPreprocessor.h"
#include "QueryProcessor/RelationTable.h"
#include "Utils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using std::string;
using std::unordered_map;
using std::vector;

namespace UnitTesting {
    TEST_CLASS(QueryPreprocessorUnitTest) {
     public:
         TEST_METHOD(QueryPreprocessor_7xmlSelectAll) {
             string expected, actual;
             
             vector<string> queryList;
             vector<string> expectedList;
             string querySyntaxErrorMsg = "Query parser encountered a syntax error in the query : ";
             // queries7.txt
             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 3 "); 
             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 12 ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 17 ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 22 ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, s1) with s1.stmt# = 29 ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 5) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 14) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 16) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 23) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (s, 30) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (4, s) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (10, s) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (12, s) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent (28, s) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 5) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 17) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 18) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 22) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Parent* (s, 30) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (s, 5) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (s, 12) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (s, 21) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 2) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 12) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 14) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 18) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 23) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Parent* (w, 30) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 1) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 4) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 10) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 11) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 19) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows (s, 22) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 1) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 4) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 10) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 11) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 19) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows (a, 22) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Follows* (s, w) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Follows* (w, s) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 2) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 11) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 12) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 17) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Follows* (s, 22) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 11) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 18) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 26) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select ifstat such that Follows* (ifstat, 28) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 5) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 8) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 12) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 18) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Follows* (a, 28) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (4, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (6, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (10, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (12, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Modifies (w, \"i\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select w such that Modifies (w, \"t\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Modifies (\"Mama\", v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Uses (s,\"x\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Uses (s, \"t\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s such that Uses (s, \"i\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (19, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (20, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (28, v) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select v such that Uses (a, v)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"i\") and Uses (a, \"i\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, a) and Uses (a, a) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"x\") and Uses (a,\"x\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"a\") and Parent (w, a)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Parent (w, a) and Modifies (a, \"a\")  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a such that Modifies (a, \"a\") such that Parent (w, a)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select p such that Calls* (p, \"Zehzeh\")  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select p such that Calls (\"Papa\", p) and Modifies (p, \"i\") and Uses (p, \"c\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select p such that Calls* (\"Papa\", p) and Modifies (p, \"t\")  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (\"t\", _)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (\"a\", \"2 * c\")  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (\"t\", _) such that Follows (w, a)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_,\"t + 1\")  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_,\"t * 3 + 2\")  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"2 * c\") ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"t + b * x\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"t + a\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"t + b\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"3 + 2\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"x + a\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a (_, _\"c * a\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select a pattern a1 (_, _\"t + a\"_)  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select s with s.stmt# = c.value  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN pattern ifstat (\"i\", _, _) with c.value = 2  ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (1, 2) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (7,9) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (9, 10) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (11, 12) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (12, 13) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (13, 12) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (14, 15) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (18, 11) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (18, 19) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (21, 27) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select BOOLEAN such that Next (27, 30) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select n such that Next* (11, n) and Next* (n, 10) ");

             queryList.push_back("stmt s, s1; assign a, a1, a2; while w; if ifstat; procedure p; variable v; constant c; prog_line n, n1, n2;Select n such that Next* (n, n)");
             
             expectedList.push_back("s Parent s s1 with s1 3 ");

             expectedList.push_back("s Parent s s1 with s1 12 ");

             expectedList.push_back("s Parent s s1 with s1 17 ");

             expectedList.push_back("s Parent s s1 with s1 22 ");

             expectedList.push_back("s Parent s s1 with s1 29 ");

             expectedList.push_back("s Parent s 5 ");

             expectedList.push_back("s Parent s 14 ");

             expectedList.push_back("s Parent s 16 ");

             expectedList.push_back("s Parent s 23 ");

             expectedList.push_back("s Parent s 30 ");

             expectedList.push_back("s Parent 4 s ");

             expectedList.push_back("s Parent 10 s ");

             expectedList.push_back("s Parent 12 s ");

             expectedList.push_back("s Parent 28 s ");

             expectedList.push_back("s Parent* s 5 ");

             expectedList.push_back("s Parent* s 17 ");

             expectedList.push_back("s Parent* s 18 ");

             expectedList.push_back("s Parent* s 22 ");

             expectedList.push_back("s Parent* s 30 ");

             expectedList.push_back("w Parent* s 5 ");

             expectedList.push_back("w Parent* s 12 ");

             expectedList.push_back("w Parent* s 21 ");

             expectedList.push_back("w Parent* w 2 ");

             expectedList.push_back("w Parent* w 12 ");

             expectedList.push_back("w Parent* w 14 ");

             expectedList.push_back("w Parent* w 18 ");

             expectedList.push_back("w Parent* w 23 ");

             expectedList.push_back("w Parent* w 30 ");

             expectedList.push_back("s Follows s 1 ");

             expectedList.push_back("s Follows s 4 ");

             expectedList.push_back("s Follows s 10 ");

             expectedList.push_back("s Follows s 11 ");

             expectedList.push_back("s Follows s 19 ");

             expectedList.push_back("s Follows s 22 ");

             expectedList.push_back("a Follows a 1 ");

             expectedList.push_back("a Follows a 4 ");

             expectedList.push_back("a Follows a 10 ");

             expectedList.push_back("a Follows a 11 ");

             expectedList.push_back("a Follows a 19 ");

             expectedList.push_back("a Follows a 22 ");

             expectedList.push_back("w Follows* s w ");

             expectedList.push_back("w Follows* w s ");

             expectedList.push_back("s Follows* s 2 ");

             expectedList.push_back("s Follows* s 11 ");

             expectedList.push_back("s Follows* s 12 ");

             expectedList.push_back("s Follows* s 17 ");

             expectedList.push_back("s Follows* s 22 ");

             expectedList.push_back("ifstat Follows* ifstat 11 ");

             expectedList.push_back("ifstat Follows* ifstat 18 ");

             expectedList.push_back("ifstat Follows* ifstat 26 ");

             expectedList.push_back("ifstat Follows* ifstat 28 ");

             expectedList.push_back("a Follows* a 5 ");

             expectedList.push_back("a Follows* a 8 ");

             expectedList.push_back("a Follows* a 12 ");

             expectedList.push_back("a Follows* a 18 ");

             expectedList.push_back("a Follows* a 28 ");

             expectedList.push_back("v Modifies 4 v ");

             expectedList.push_back("v Modifies 6 v ");

             expectedList.push_back("v Modifies 10 v ");

             expectedList.push_back("v Modifies 12 v ");

             expectedList.push_back("w Modifies w \"i\" ");

             expectedList.push_back("w Modifies w \"t\" ");

             expectedList.push_back("v Modifies \"Mama\" v ");

             expectedList.push_back("s Uses s \"x\" ");

             expectedList.push_back("s Uses s \"t\" ");

             expectedList.push_back("s Uses s \"i\" ");

             expectedList.push_back("v Uses 19 v ");

             expectedList.push_back("v Uses 20 v ");

             expectedList.push_back("v Uses 28 v ");

             expectedList.push_back("v Uses a v ");

             expectedList.push_back("a Modifies a \"i\" Uses a \"i\" ");

             expectedList.push_back(querySyntaxErrorMsg+"10");

             expectedList.push_back("a Modifies a \"x\" Uses a \"x\" ");

             expectedList.push_back("a Modifies a \"a\" Parent w a ");

             expectedList.push_back("a Parent w a Modifies a \"a\" ");

             expectedList.push_back("a Modifies a \"a\" Parent w a ");

             expectedList.push_back("p Calls* p \"Zehzeh\" ");

             expectedList.push_back("p Calls \"Papa\" p Modifies p \"i\" Uses p \"c\" ");

             expectedList.push_back("p Calls* \"Papa\" p Modifies p \"t\" ");

             expectedList.push_back("a pattern a \"t\" _ ");

             expectedList.push_back("a pattern a \"a\" \"(2*c)\" ");

             expectedList.push_back("a Follows w a pattern a \"t\" _ ");

             expectedList.push_back("a pattern a _ \"(t+1)\" ");

             expectedList.push_back("a pattern a _ \"((t*3)+2)\" ");

             expectedList.push_back(querySyntaxErrorMsg+"30");

             expectedList.push_back("a pattern a _ _\"(t+(b*x))\"_ ");

             expectedList.push_back("a pattern a _ _\"(t+a)\"_ ");

             expectedList.push_back("a pattern a _ _\"(t+b)\"_ ");

             expectedList.push_back("a pattern a _ _\"(3+2)\"_ ");

             expectedList.push_back("a pattern a _ _\"(x+a)\"_ ");

             expectedList.push_back("a pattern a _ _\"(c*a)\"_ ");

             expectedList.push_back("a pattern a1 _ _\"(t+a)\"_ ");

             expectedList.push_back("s with s c ");

             expectedList.push_back("BOOLEAN pattern ifstat \"i\" _ _ with c 2 ");

             expectedList.push_back("BOOLEAN Next 1 2 ");

             expectedList.push_back("BOOLEAN Next 7 9 ");

             expectedList.push_back("BOOLEAN Next 9 10 ");

             expectedList.push_back("BOOLEAN Next 11 12 ");

             expectedList.push_back("BOOLEAN Next 12 13 ");

             expectedList.push_back("BOOLEAN Next 13 12 ");

             expectedList.push_back("BOOLEAN Next 14 15 ");

             expectedList.push_back("BOOLEAN Next 18 11 ");

             expectedList.push_back("BOOLEAN Next 18 19 ");

             expectedList.push_back("BOOLEAN Next 21 27 ");

             expectedList.push_back("BOOLEAN Next 27 30 ");

             expectedList.push_back("n Next* 11 n Next* n 10 ");

             expectedList.push_back("n Next* n n ");


             int i = 0;
             for (string query : queryList) {
                 QueryPreprocessor qp;
                 QueryTree qt;
                 actual = "";
                 try {
                     qp.preprocessQuery(query);
                 }
                 catch (std::exception& ex) {
                     actual = ex.what();
                     Assert::AreEqual(expectedList[i] + " at:" + Utils::IntToString(i), actual + " at:" + Utils::IntToString(i));
                     i++;
                     continue;
                 }
                 qt = qp.getQueryTree();
                 vector<string> varList;

                 varList = qt.getResults();
                 for (unsigned int ii = 0; ii < varList.size(); ii++) {
                     actual += varList[ii] + " ";
                 }
                 vector<Clause> clauseList = qt.getClauses();

                 for (Clause c : clauseList) {
                     actual += c.getClauseType() + " ";
                     int num = c.getArgCount();
                     if (c.getClauseType() == "with") {
                         num--;
                     }
                     for (int n = 0; n < num; n++) {
                         actual += c.getArg()[n] + " ";
                     }
                 }
                 Assert::AreEqual(expectedList[i]+" at:"+Utils::IntToString(i), actual+" at:"+Utils::IntToString(i));
                 i++;
             }
         }
        TEST_METHOD(QueryPreprocessor_1Select) {
            string expected, actual, query;
            QueryPreprocessor qp;
            QueryTree qt;

            query = "assign a,a1; while w1,w2;Select a";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<string> varList;

            varList = qt.getResults();
            expected = "a ";
            for (unsigned int i = 0; i < varList.size(); i++) {
                actual += varList[i] + " ";
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_2SuchThat) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; while w1,w2; variable x;";
            query += "Select a such that Uses(a1, x)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses a1 x ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_3SuchThatWithStringConstant) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(a1, \"x\")";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses a1 \"x\" ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_4SuchThatWithNumericConstant) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(1, x)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses 1 x ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_5SuchThatTwoVars) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that Uses(a1, x)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "Uses a1 x ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
        }
        TEST_METHOD(QueryPreprocessor_6Pattern) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1;";
            query += "Select a pattern a1(\"x\",_)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "pattern a1 \"x\" _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_7PatternSelectTwo) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query = "assign a,a1;";
            query += "Select a pattern a1(\"x\",_)";
            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "pattern a1 \"x\" _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }
        TEST_METHOD(QueryPreprocessor_8MultipleAnd) {
            string expected, actual, query;
            QueryPreprocessor qp;
            QueryTree qt;

            query = "assign a1,a2,a3; stmt s1,s2,s3; variable v1,v2,v3;";
            query += "Select <s1,s2,v2> such that Uses(s3,v1) and Modifies(s3,\"x\") and ";
            query += "Follows(s1,s2) and Parent(s3,s1) such that Uses(s2,v1) and Uses(5,\"y\") and ";
            query += "Follows(3,4) pattern a1(v2,\"x+y\") and a3(\"z\",_) such that Modifies(a3,v3)";
            try {
                qp.preprocessQuery(query);
            } catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<string> varList;

            varList = qt.getResults();
            for (unsigned int i = 0; i < varList.size(); i++) {
                actual += varList[i] + " ";
            }
            vector<Clause> clauseList = qt.getClauses();

            expected = "s1 s2 v2 ";
            expected += "Uses s3 v1 Modifies s3 \"x\" Follows s1 s2 Parent s3 s1 Uses s2 v1 Uses 5 \"y\" ";
            expected += "Follows 3 4 Modifies a3 v3 ";
            expected += "pattern a1 v2 pattern a3 \"z\" ";
            for (Clause c : clauseList) {
                actual += c.getClauseType() + " ";
                actual += c.getArg()[0] + " ";
                actual += c.getArg()[1] + " ";
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_InvalidQuery) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            bool isValid;

            string query;
            query = "assign a,a1; variable x;";
            query += "Select a such that uses(\"1a\", x)";

            try {
                qp.preprocessQuery(query);
                isValid = true;
            } catch (std::exception& ex) {
                isValid = false;
            }
            Assert::AreEqual(isValid, false);
        }

        TEST_METHOD(QueryPreprocessor_With) {
            string expected, actual;
            QueryPreprocessor qp;
            QueryTree qt;

            string query;
            query += "procedure p; Select p with \"Mudkip\" = p.procName";

            try {
                qp.preprocessQuery(query);
            }
            catch (std::exception& ex) {
                actual = ex.what();
            }
            qt = qp.getQueryTree();
            vector<Clause> resList;

            resList = qt.getClauses();
            expected = "pattern a1 \"x\" _ ";
            for (unsigned int i = 0; i < resList.size(); i++) {
                actual += resList[i].getClauseType() + " ";
                for (unsigned int j = 0; j < resList[i].getArg().size(); j++) {
                    actual += resList[i].getArg()[j] + " ";
                }
            }
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_ExtractQueryTreeOneClause) {
            string query = "assign a; Select a pattern a(\"a\", _)";
            QueryPreprocessor queryPreprocessor = QueryPreprocessor();
            queryPreprocessor.preprocessQuery(query);
            QueryTree queryTree = queryPreprocessor.getQueryTree();

            string actual, expected;

            unordered_map<string, Symbol> varMap = queryTree.getVarMap();
            for (auto kv : varMap) {
                actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
            }

            vector<string> selectList = queryTree.getResults();
            for (auto varName : selectList) {
                actual += Constants::SymbolToString(varMap[varName]) + " " + varName + "; ";
            }

            vector<Clause> clauses = queryTree.getClauses();
            for (Clause c : clauses) {
                actual += c.getClauseType() + " ";
                for (int i = 0; i < c.getArgCount(); i++) {
                    actual += c.getArg()[i] + " ";
                }
            }

            expected = "assign a; assign a; pattern a \"a\" _ ";  // declaration; select a; pattern
            Assert::AreEqual(expected, actual);
        }

        TEST_METHOD(QueryPreprocessor_ExtractQueryTreeTwoClauses) {
            string query = "assign a, a1; Select a pattern a(\"e\", _) such that Follows(a, a1)";
            QueryPreprocessor queryPreprocessor = QueryPreprocessor();
            queryPreprocessor.preprocessQuery(query);
            QueryTree queryTree = queryPreprocessor.getQueryTree();

            string actual, expected;

            unordered_map<string, Symbol> varMap = queryTree.getVarMap();
            for (auto kv : varMap) {
                actual += Constants::SymbolToString(kv.second) + " " + kv.first + "; ";
            }

            vector<string> selectList = queryTree.getResults();
            for (auto varName : selectList) {
                actual += Constants::SymbolToString(varMap[varName]) + " " + varName + "; ";
            }

            vector<Clause> clauses = queryTree.getClauses();
            for (Clause c : clauses) {
                actual += c.getClauseType() + " ";
                for (int i = 0; i < c.getArgCount(); i++) {
                    actual += c.getArg()[i] + " ";
                }
            }

            expected = "assign a; assign a1; assign a; Follows a a1 pattern a \"e\" _ ";  // declaration; select a; Follows
            Assert::AreEqual(actual, expected);
        }
    };
}  // namespace UnitTesting

