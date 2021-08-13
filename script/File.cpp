#include "File.h"

File::File(string name) {
    this->Name = name;
    this->Parent = nullptr;
    this->action = Action::None;
}

File::File(string name, Node* parent) {
    this->Name = name;
    this->Parent = parent;
    this->action = Action::None;
}

File::~File() { }

void File::print(string pwd) {
    // ��ġ + �ڽ��� �̸� ���
    cout << pwd << this->Name << endl;
}

void File::cut() {
    // Action�� Cut���� ���
    this->action = Action::Cut;
}

void File::copy() {
    // Action�� Copy�� ���
    this->action = Action::Copy;
}

Node* File::paste() {
    if (action == Action::Copy) {
        // ���ο� ��ü�� ����
        return new File(this->Name);
    }
    else if (action == Action::Cut) {
        this->action = Action::None;

        // �ڽ� ��ü�� ����
        return this;
    }

    // No Action
    return nullptr;
}