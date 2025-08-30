#include <iostream>
#include <string>

using namespace std;

// ---------------------- Linked List for Storing All Members ----------------------
class Member 
{
public:
    string name;
    int id;
    Member* firstRecruit; // First child
    Member* nextSibling;  // Next sibling
    Member* next;         // Next member in the linked list (for fast searching)

    // Constructor
    Member(string name, int id) 
    {
        this->name = name;
        this->id = id;
        this->firstRecruit = nullptr;
        this->nextSibling = nullptr;
        this->next = nullptr;
    }
};

// ---------------------- Stack (For Undo Feature) ----------------------
class Stack 
{
private:
    struct StackNode 
    {
        Member* member;
        StackNode* next;
        StackNode(Member* m) : member(m), next(nullptr) {}
    };
    StackNode* top;

public:
    Stack() 
    { 
        top = nullptr; 
    }

    void push(Member* m) 
    {
        StackNode* newNode = new StackNode(m);
        newNode->next = top;
        top = newNode;
    }

    Member* pop() 
    {
        if (top == nullptr) return nullptr;
        StackNode* temp = top;
        Member* m = temp->member;
        top = top->next;
        delete temp;
        return m;
    }

    bool isEmpty() 
    { 
        return top == nullptr; 
    }
};

// ---------------------- Queue (For Level-Order Traversal) ----------------------
class Queue 
{
private:
    struct QueueNode 
    {
        Member* member;
        QueueNode* next;
        QueueNode(Member* m) : member(m), next(nullptr) {}
    };
    QueueNode* front, * rear;

public:
    Queue() 
    {
        front = rear = nullptr;
    }

    void enqueue(Member* m) 
    {
        QueueNode* newNode = new QueueNode(m);
        if (rear == nullptr) 
        {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    Member* dequeue() 
    {
        if (front == nullptr) return nullptr;
        QueueNode* temp = front;
        Member* m = temp->member;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
        return m;
    }

    bool isEmpty() 
    { 
        return front == nullptr; 
    }
};

// ---------------------- MLM System ----------------------
class MLMSystem 
{
private:
    Member* root;
    Member* memberListHead; // Head of linked list for fast searching
    Stack undoStack;

public:
    // Constructor
    MLMSystem(string rootName, int rootId) 
    {
        root = new Member(rootName, rootId);
        memberListHead = root;
    }

    // Function to search for a member by ID in linked list
    Member* searchMember(int id) 
    {
        Member* temp = memberListHead;
        while (temp != nullptr) 
        {
            if (temp->id == id)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Function to add a recruit
    void addRecruit(int recruiterId, string recruitName, int recruitId) 
    {
        Member* recruiter = searchMember(recruiterId);
        if (recruiter == nullptr) 
        {
            cout << "Recruiter with ID " << recruiterId << " not found.\n";
            return;
        }

        Member* newMember = new Member(recruitName, recruitId);
        newMember->next = memberListHead; // Add to linked list
        memberListHead = newMember;

        if (recruiter->firstRecruit == nullptr) 
        {
            recruiter->firstRecruit = newMember;
        }
        else 
        {
            Member* temp = recruiter->firstRecruit;
            while (temp->nextSibling != nullptr) 
            {
                temp = temp->nextSibling;
            }
            temp->nextSibling = newMember;
        }

        // Push to undo stack
        undoStack.push(newMember);

        cout << "Member " << recruitName << " added under " << recruiter->name << "!\n";
    }

    // Function to remove the last added member (Undo Feature)
    void undoLastAdd() 
    {
        Member* lastAdded = undoStack.pop();
        if (lastAdded == nullptr) 
        {
            cout << "No members to undo!\n";
            return;
        }

        // Remove from linked list
        if (memberListHead == lastAdded) 
        {
            memberListHead = memberListHead->next;
        }
        else 
        {
            Member* temp = memberListHead;
            while (temp->next != nullptr && temp->next != lastAdded) 
            {
                temp = temp->next;
            }
            if (temp->next == lastAdded)
                temp->next = lastAdded->next;
        }

        cout << "Undo: Removed " << lastAdded->name << " from the system.\n";
        delete lastAdded;
    }

    // Function to display MLM hierarchy using queue (Level Order Traversal)
    void displayHierarchy() 
    {
        if (root == nullptr) return;
        Queue q;
        q.enqueue(root);

        cout << "\nMLM Hierarchy (Level Order Traversal):\n";
        while (!q.isEmpty()) 
        {
            Member* curr = q.dequeue();
            cout << curr->name << " (ID: " << curr->id << ")\n";

            for (Member* child = curr->firstRecruit; child != nullptr; child = child->nextSibling) 
            {
                q.enqueue(child);
            }
        }
    }
};

// ---------------------- Main Function ----------------------
int main() 
{
    string rootName;
    int rootId;

    cout << "Enter the name of the root sponsor: ";
    getline(cin, rootName);
    cout << "Enter ID for the root sponsor: ";
    cin >> rootId;
    cin.ignore();

    MLMSystem mlm(rootName, rootId);

    int choice;
    do 
    {
        cout << "\nMENU:\n";
        cout << "1. Add a Member\n";
        cout << "2. Display MLM Hierarchy\n";
        cout << "3. Undo Last Addition\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) 
        {
            int recruiterId, recruitId;
            string recruitName;

            cout << "Enter the ID of the recruiter: ";
            cin >> recruiterId;
            cin.ignore();

            cout << "Enter the name of the new member: ";
            getline(cin, recruitName);

            cout << "Enter ID for the new member: ";
            cin >> recruitId;
            cin.ignore();

            mlm.addRecruit(recruiterId, recruitName, recruitId);
        }
        else if (choice == 2) 
        {
            mlm.displayHierarchy();
        }
        else if (choice == 3) 
        {
            mlm.undoLastAdd();
        }
        else if (choice != 4) 
        {
            cout << "Invalid choice! Try again.\n";
        }
    } 
    while (choice != 4);

    cout << "Exiting program. Goodbye!\n";
    return 0;
}
