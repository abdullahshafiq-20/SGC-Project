// Creator: Muhammad Bilal
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <iomanip>

using namespace std;
void create()
{
    fstream fout;
    int i, ID;
    string name, type, area, link_to_regiter;
    double amount;

    cout << "Enter the ID" << endl;
    cin >> ID;
    string fname;
    fname = to_string(ID);
    fname += ".csv";
    fout.open(fname, ios::out | ios::app);
    // Read the input

    cout << "Enter the details of Scholarship/Grant:"
         << " name, Type, field, amount and Link to registration" << endl;
    cin >> name >> type >> area >> amount >> link_to_regiter;

    // Insert the data to file
    fout << ID << ", "
         << name << ", "
         << type << ", "
         << area << ", "
         << amount << ", "
         << link_to_regiter
         << "\n";
}

class FileNode
{
public:
    string file_name;
    int size; // You may add other file attributes as needed
    time_t created_at;

    FileNode(string name, int s, time_t created) : file_name(name), size(s), created_at(created) {}
};

class BTreeNode
{
public:
    bool leaf;
    vector<FileNode> keys;
    vector<BTreeNode *> children;

    BTreeNode(bool is_leaf = true) : leaf(is_leaf) {}

    ~BTreeNode()
    {
        for (auto child : children)
        {
            delete child;
        }
    }
};

class BTree
{
public:
    BTreeNode *root;
    int degree;

    void insertNonFull(BTreeNode *x, const FileNode &file_node)
    {
        int i = x->keys.size() - 1;

        if (x->leaf)
        {
            x->keys.push_back(FileNode("", 0, 0)); // Placeholder for the new key
            while (i >= 0 && file_node.created_at < x->keys[i].created_at)
            {
                x->keys[i + 1] = x->keys[i];
                i--;
            }
            x->keys[i + 1] = file_node;
        }
        else
        {
            while (i >= 0 && file_node.created_at < x->keys[i].created_at)
            {
                i--;
            }
            i++;
            if (x->children[i]->keys.size() == (2 * degree) - 1)
            {
                splitChild(x, i);
                if (file_node.created_at > x->keys[i].created_at)
                {
                    i++;
                }
            }
            insertNonFull(x->children[i], file_node);
        }
    }
    void splitChild(BTreeNode *x, int i)
    {
        int degree = this->degree;
        BTreeNode *y = x->children[i];
        BTreeNode *z = new BTreeNode(y->leaf);
        x->children.insert(x->children.begin() + i + 1, z);
        x->keys.insert(x->keys.begin() + i, y->keys[degree - 1]);
        z->keys = vector<FileNode>(y->keys.begin() + degree, y->keys.end());
        y->keys = vector<FileNode>(y->keys.begin(), y->keys.begin() + degree - 1);
    }

    BTree(int deg) : root(nullptr), degree(deg) {}

    ~BTree()
    {
        delete root;
    }

    void insert(const FileNode &file_node)
    {
        if (!root)
        {
            root = new BTreeNode(true);
            root->keys.push_back(file_node);
        }
        else
        {
            if (root->keys.size() == (2 * degree) - 1)
            {
                BTreeNode *new_root = new BTreeNode(false);
                new_root->children.push_back(root);
                splitChild(new_root, 0);
                root = new_root;
            }
            insertNonFull(root, file_node);
        }
    }
    void populateFromFolder(const string &folderPath)
    {
        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile((folderPath + "\\*").c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            cerr << "Error opening directory: " << folderPath << endl;
            return;
        }

        int fileNumber = 1;

        do
        {
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue; // Skip directories
            }

            ostringstream oss;
            oss << setw(3) << setfill('0') << fileNumber++;
            string fileName = oss.str() + ".csv";

            FileNode newFile(fileName, 0, 0); // Size and created_at are not specified in this example
            insert(newFile);

        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }
    void displayFileNames(BTreeNode *node)
    {
        if (node != nullptr)
        {
            for (const auto &file : node->keys)
            {
                cout << file.file_name << endl;
            }

            if (!node->leaf)
            {
                for (auto child : node->children)
                {
                    displayFileNames(child);
                }
            }
        }
    };
};

int main()
{
    BTree btree(8);

    // Populating B-tree from a Windows folder
    btree.populateFromFolder("F:\\University\\Semester 3\\DSA\\SGC Project\\test");

    // Displaying file names

    int optionNumber;
    cout << "~~  B-tree File System (Btrfs) Simulation ~~" << endl;
    while (1)
    {
        cout << "\n\n````````` The Main Menu `````````" << endl;
        cout << "---------------------------------" << endl;
        cout << "1.Add a new scholarship \n2.Delete a Scholarship \n3.Modify \n4.Find a scholarship \n5.Sort scholarships \n6.List Files \n7.Exit\n";
        cout << "---------------------------------" << endl;
        cout << "\nEnter The Option Number: "<<endl;
        cin >> optionNumber;
        cout << endl;

        if (optionNumber == 1)
        {
        }
        else if (optionNumber == 2)
        {
        }
        else if (optionNumber == 3)
        {
        }
        else if (optionNumber == 4)
        {
            cout << "File names in B-tree:" << endl;
            btree.displayFileNames(btree.root);
        }
        else if (optionNumber == 5)
        {
        }
        else if (optionNumber == 6)
        {
        }
        else if (optionNumber == 7)
        {
            break;
        }
        else
        {
            cout << "Invalid Input.! Please Try Again." << endl;
        }
    }

    return 0;
}