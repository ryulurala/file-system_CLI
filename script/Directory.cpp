#include "Directory.h"

Directory::Directory(string name) {
    this->Name = name;
    this->Parent = nullptr;
    this->action = Action::None;
}

Directory::Directory(string name, Node* parent) {
    this->Name = name;
    this->Parent = parent;
    this->action = Action::None;
}

Directory::~Directory() {
    for (Node* node : this->_nodeList) {
        delete node;
    }
}

void Directory::add(string name) {
    if (name.empty())
        return;

    // �ߺ� �˻�
    if (this->findNode(name) != nullptr) {
        cout << "duplicated!" << endl;
        return;
    }

    // ���丮 or ���� ����
    Node* node;
    if (name.back() == '/') 
        node = new Directory(name, this);   // ���丮
    else 
        node = new File(name, this);        // ����

    // ��� ����Ʈ�� �߰�
    this->_nodeList.push_back(node);
}

void Directory::add(Node* node) {
    if (node == nullptr)
        return;

    // �ߺ� �˻�
    if (this->findNode(node->Name) != nullptr) {
        // �޸� ���� ����
        // copy�� ������ ����
        if (node->Parent == nullptr) delete node;

        cout << "duplicated!" << endl;
        return;
    }
    
    // Action�� cut�̶�� parent �����ϹǷ�
    if (node->Parent != nullptr) {
        // �ش� ���丮�� ����Ʈ���� ����
        ((Directory*)node->Parent)->remove(node);
    }

    // parent ���
    node->Parent = this;

    // ����Ʈ�� �߰�
    this->_nodeList.push_back(node);
}

void Directory::remove(string name) {
    for (int i = 0; i < this->_nodeList.size(); i++) {
        if (this->_nodeList[i]->Name == name) {
            // ��ü ����
            delete this->_nodeList[i];

            // ����Ʈ���� ����
            this->_nodeList.erase(this->_nodeList.begin() + i);
        }
    }
}

void Directory::remove(Node* node) {
    for (int i = 0; i < this->_nodeList.size(); i++) {
        if (this->_nodeList[i]->Name == node->Name) {
            // ����Ʈ������ ����
            this->_nodeList.erase(this->_nodeList.begin() + i);
        }
    }
}

void Directory::print(string pwd) {
    if (pwd == this->Name) {
        // ó�� ���ٿ� ���ؼ�
        pwd = "";
    }
    else{
        // �ش� ���丮�� ���
        cout << pwd << this->Name << endl;

        // ���� ��ġ + �ش� ���丮��
        pwd += this->Name;
    }

    for (Node* node : this->_nodeList) {
        // �� ���鿡 ���� ���
        node->print(pwd);
    }
}

Directory* Directory::changeDirectory(string& pwd, string& args) {
    if (args.empty())
        return this;    // ���� ������
    
    int index = args.find('/');
    if (index == string::npos)
        return nullptr;

    string arg = args.substr(0, index + 1);
    args = args.substr(index + 1);

    // ���� ���
    if (arg == "/") {
        Node* root = this;
        while (root->Parent != nullptr) {
            root = root->Parent;
        }
        pwd = arg;

        // ���� ���丮
        return ((Directory*)root)->changeDirectory(pwd, args);
    }

    // ��� ���
    if (arg == "../" && this->Parent != nullptr) {
        int currentNameLen = this->Name.length();
        int pwdLen = pwd.length();
        
        pwd = pwd.substr(0, pwdLen - currentNameLen);
        
        // ���� ���丮
        return ((Directory*)this->Parent)->changeDirectory(pwd, args);
    }

    Node* nextDirectory = this->findNode(arg);
    if (nextDirectory != nullptr) {
        pwd += arg;

        // ���� ���丮
        return ((Directory*)nextDirectory)->changeDirectory(pwd, args);
    }

    return nullptr;
}

Node* Directory::findNode(string name) {
    for (Node* node : this->_nodeList) {
        // Name�� ��
        if (node->Name == name) {
            return node;
        }
    }

    return nullptr;
}

void Directory::cut() {
    this->action = Action::Cut;
    
    for (Node* node : this->_nodeList) {
        // ����� Action�� Cut���� ���
        node->cut();
    }
}

void Directory::copy() {
    this->action = Action::Copy;
    
    for (Node* node : this->_nodeList) {
        // ����� Action�� Copy�� ���
        node->copy();
    }
}

Node* Directory::paste() {
    if (action == Action::Copy) {
        // ���ο� ���丮 ����
        Directory* copyDirectory = new Directory(this->Name);

        for (Node* node : this->_nodeList) {
            // ���ο� ������ ����
            Node* copyNode = node->paste();

            // ���ο� ���丮�� push
            copyDirectory->_nodeList.push_back(copyNode);
        }

        return copyDirectory;
    }
    else if (action == Action::Cut) {
        for (Node* node : this->_nodeList) {
            // ����� Action�� None���� �ʱ�ȭ
            node->paste();
        }
        this->action = Action::None;

        // �ڽ��� ����
        return this;
    }

    // No Action
    return nullptr;
}