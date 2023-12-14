#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "SuffixTree.h"
using namespace std;

void test(const char*, const char*, const char*, const char*);

int main(int argc, char** argv)
{
    test("bananabanaba$", "bana", "banana", "an");
    test("bananabanana$", "bana", "banana", "an");
    test("bananabananabanana$", "bana", "banana", "an");
    test("banana$", "bana", "banana", "ana");
    test("hello My name is Yousef MOstAfa MOhammeD KILANY, this is my Suffix Tree!", "Tree", "e", "is");
    test("yfkjjjjjjjjjjjjjeufdii", "j", "jeu", "efd");
    test("can you find the secret word?", "can you find the secret word?", "can you find the secret word!", "can you find the secret word??");
    test("too many test cases!", "t", "es", "se");
    test("There is no substitute for HARD WORK", "HARd", "HARD", "D W");
    test("SuffixTree", "f", "uffix", "ix");
}

void test(const char* text, const char* t1, const char* t2, const char* t3)
{
    printf("%s\n", text);
    SuffixTree t(text);

    printf("%s: ", t1);
    t.Search(t1);

    printf("%s: ", t2);
    t.Search(t2);

    printf("%s: ", t3);
    t.Search(t3);

    printf("\n\n");
}

