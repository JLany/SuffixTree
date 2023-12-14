#ifndef LIST_H
#define LIST_H

template <typename Obejct>
class Node
{
public:
    Node(Node* next, Node* prev, const Obejct data)
    {
        this->next = next;
        this->prev = prev;
        this->data = data;
    }

private:
    Node* next;
    Node* prev;
    Obejct data;
};


template <typename Object>
class List
{
private:
    struct Node
    {
        Node* next;
        Node* prev;
        Object data;

        Node(const Object d = Object{}, Node* n = nullptr, Node* p = nullptr)
            : data{ d }, next{ n }, prev{ p }
        {
        }
    };



    Node* _head;
    Node* _tail;
    Node* _enumerator;

    int _size;


public:
    List()
    {
        _head = nullptr;
        _tail = nullptr;
        _enumerator = nullptr;
        _size = 0;
    }

    ~List()
    {
        this->clear();
    }

    void push_back(const Object& val)
    {
        if (_head == nullptr)
        {
            _head = new Node;
            _tail = new Node;
            _head->next = _tail;
            _tail->prev = _head;
        }

        Node* toInsert = new Node(val);
        toInsert->next = _tail;
        toInsert->prev = _tail->prev;
        _tail->prev->next = toInsert;
        _tail->prev = toInsert;

        _size++;
    }

    void pop_back()
    {
        Node* toDelete = _tail->prev;

        if (toDelete == _head)
        {
            return;
        }

        _tail->prev = _tail->prev->prev;
        _tail->prev->next = _tail;

        delete toDelete;
        _size--;
    }

    int size()
    {
        return _size;
    }

    void clear()
    {
        // if (_head == nullptr)
        // {
        //     return;
        // }

        // while (_head->next != _tail)
        // {
        //     auto& old = _head->next;
        //     _head->next = old->next;
        //     delete old;
        //     old = nullptr;
        // }

        // delete _head;
        // delete _tail;
        _head = nullptr;
        _tail = nullptr;
        _enumerator = nullptr;
        _size = 0;
    }

    void start_enumerator()
    {
        if (_head == nullptr)
        {
            return;
        }
        _enumerator = _head->next;
    }

    void next()
    {
        if (_enumerator == nullptr)
        {
            return;
        }
        _enumerator = _enumerator->next;
    }

    bool has_next()
    {
        return _enumerator != _tail && _enumerator != nullptr;
    }

    Object& current() const
    {
        return _enumerator->data;
    }
};



#endif // LIST_H
