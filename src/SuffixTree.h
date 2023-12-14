#ifndef SUFFIX_TREE_H
#define SUFFIX_TREE_H

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "List.h"
using namespace std;

class SuffixTree {
private:

    struct SuffixNode {
        List<SuffixNode> children;
        int index;
        int suffixStart = -1;

        SuffixNode(int idx = -1, int sfStart = -1)
        {
            index = idx;
            suffixStart = sfStart;
        }
    };


    SuffixNode* _root;
    char* _text;
    int _textLength;

public:
    SuffixTree(const char* text)
    {
        Init(text);
    }

    ~SuffixTree()
    {
        delete[] _text;

        // delete the tree itself
        delete _root;
    }

private:

    void Init(const char* text)
    {
        _textLength = strlen(text);
        _text = new char[_textLength + 1];
        strcpy(_text, text);

        _root = new SuffixNode;
        _root->index = -1;
        _root->suffixStart = -1;

        for (int i = _textLength - 1; i >= 0; --i)
        {
            Construct(_root, i, 0);
        }
    }

    void Construct(SuffixNode* currNode, int suffixPointer, int offset)
    {
        bool foundMatch = false;

        // Offset determines how much of the current suffix have been matched so far in out traversal.
        // Find a child that matches with the first char in the current suffix.
        for (currNode->children.start_enumerator(); currNode->children.has_next(); currNode->children.next())
        {
            SuffixNode& current = currNode->children.current();

            if (_text[suffixPointer + offset] == _text[current.index])
            {
                // Proceed with insertion of the suffix in that child.
                ConstructHelper(&current, suffixPointer, offset);
                foundMatch = true;
                break;
            }
        }

        // If non of the children matches, make a new entry for the new suffix 
        // at the offset reached so far in the traversal.
        if (!foundMatch)
        {
            currNode->children.push_back(SuffixNode{ suffixPointer + offset, suffixPointer });
        }
    }

    // Offset here represents how much of the input suffix has matched so far in our traversal
    void ConstructHelper(SuffixNode* currNode, int suffixPointer, int offset)
    {
        // Acquire current node suffix length
        int currNodeLength = SubSuffixSize(currNode);

        // Compare current node's suffix with mine
        int i;
        for (i = 1; i < currNodeLength; ++i)
        {
            if (_text[suffixPointer + i + offset] != _text[currNode->index + i])
                break;
        }

        // If reached end of current node's suffix text:
        //     If my text[suffixPointer] matches any child's text[idx]
        //         Traverse to child
        if (i == currNodeLength)
        {
            // Parent gets the index of the earlier apperance of the shared prefix.
            currNode->index = suffixPointer + offset;

            Construct(currNode, suffixPointer, i + offset);
        }
        // If stopped earlier:
        //     current node gets two new children:
        //         One representing new suffix inserted
        //         One representing the old suffix minus the shared prefix
        //     Mark current node as internal (-1)
        else
        {
            Split(currNode, i, offset, suffixPointer);

            // Parent gets the index of the earlier apperance of the shared prefix.
            currNode->index = suffixPointer + offset;
        }
    }

    void Split(SuffixNode* parent, int i, int offset, int suffixPointer)
    {
        SuffixNode node1, node2;

        node1.index = parent->index + i;
        node1.suffixStart = parent->suffixStart;
        node1.children = parent->children;

        node2.index = suffixPointer + i + offset;
        node2.suffixStart = suffixPointer;

        // Splitted node goes one level higher.
        parent->suffixStart = -1;
        parent->children.clear();

        parent->children.push_back(node1);
        parent->children.push_back(node2);
    }

    int FindMin(List<SuffixNode> l)
    {
        int min = INT32_MAX;

        if (l.size() < 1)
            return -1;

        for (l.start_enumerator(); l.has_next(); l.next())
        {
            SuffixNode& current = l.current();

            if (current.index < min)
                min = current.index;
        }

        return min;
    }

    // Get the size of of the substring on the edge going to the current node.
    int SubSuffixSize(SuffixNode* node)
    {
        int l;

        if (node == _root)
            return -1;

        if (node->children.size() < 1)
        {
            l = _textLength - node->index;
        }
        else
        {
            l = FindMin(node->children) - node->index;
        }

        return l;
    }


public:
    void Search(const char* text)
    {
        SearchHelper(_root, text, strlen(text), 0);
    }

private:
    // The offset here helps memorize how far we have matched from the search text.
    void SearchHelper(SuffixNode* currNode, const char* text, int textSize, int offset)
    {
        // Traverse with the input text on the tree
        // If reached end of input while on the tree:
        //     Land on a node if on an edge
        //     If no children:
        //         Print suffixIndex
        //     DFS on this node to get suffix starts of leaf decendents
        //     At each leaf:
        //         Print suffixIndex
        if (textSize < 1)
        {
            printf("%d\n", -1);
            return;
        }

        int i;
        for (i = 0; i < SubSuffixSize(currNode); ++i)
        {
            if (i + offset >= textSize)
                break;

            if (_text[currNode->index + i] != text[i + offset])
                break;
        }

        // If we reached end of suffix.
        if (i + offset >= textSize)
        {
            DfsLeaves(currNode);
            printf("\n");
        }
        // If we reached end of current node's subsuffix.
        else if (i >= SubSuffixSize(currNode))
        {
            bool found = false;

            // Traverse to the child that matches with the char reached so far in the search.
            for (currNode->children.start_enumerator(); currNode->children.has_next(); currNode->children.next())
            {
                SuffixNode& current = currNode->children.current();

                if (_text[current.index] == text[i + offset])
                {
                    SearchHelper(&current, text, textSize, i + offset);
                    found = true;
                    break;
                }
            }

            if (!found)
                printf("%d\n", -1);
        }
        else
        {
            printf("%d\n", -1);
        }
    }

    // Search for leaves and print their suffixStart value.
    void DfsLeaves(SuffixNode* currNode)
    {
        if (currNode->suffixStart == -1)
        {
            // No need to reverse iterate since anyways parts of the solution will not be good any way.
            for (currNode->children.start_enumerator(); currNode->children.has_next(); currNode->children.next())
            {
                SuffixNode& current = currNode->children.current();

                DfsLeaves(&current);
            }
        }
        else
        {
            printf("%d ", currNode->suffixStart);
        }
    }
};

#endif // SUFFIX_TREE_H
