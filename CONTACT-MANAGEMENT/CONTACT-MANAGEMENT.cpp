#define _CRT_SECURE_NO_DEPRECATE
#include<iostream>
#include<fstream>
#include<vector>
#include<string.h>
#include<string>
#include<stack>
#include <ctime>
using namespace std;

string Time() {

	time_t currentTime = time(0);

	char* timeString = ctime(&currentTime);

	return timeString;
}

class contact
{
public:

	long int ph;
	char name[20], add[20], email[30];
	time_t callTime;

	void create_contact()
	{
		cout << "Phone: ";
		cin >> ph;
		cout << "Name: ";
		cin.ignore();
		cin >> name;

		cin.ignore();
		cout << "Address: ";
		cin.ignore();
		cin >> add;

		cout << "Email address: ";
		cin.ignore();
		cin >> email;

		cout << "\n";
	}

	void show_contact()
	{
		cout << endl << "Phone #: " << ph;
		cout << endl << "Name: " << name;
		cout << endl << "Address: " << add;
		cout << endl << "Email Address : " << email;
	}

	char* getName()
	{
		return name;
	}

	void makeCall() {
		callTime = time(0);
	}
};

fstream fp;
contact cont;

void save_contact()
{
	fp.open("contactBook.txt", ios::out | ios::app);
	cont.create_contact();
	fp.write((char*)&cont, sizeof(contact));
	fp.close();
	cout << endl << endl << "Contact Has Been Sucessfully Created...";

}

void show_all_contacts()
{
	cout << "\n\t\t================================\n\t\t\tLIST OF CONTACTS\n\t	\t================================\n";
	fp.open("contactBook.txt", ios::in);
	while (fp.read((char*)&cont, sizeof(contact)))
	{
		cont.show_contact();
		cout << endl << "=================================================\n" << endl;
	}
	fp.close();
}

void display_contact(int num)
{
	bool found;
	int ch;

	found = false;
	fp.open("contactBook.txt", ios::in);
	while (fp.read((char*)&cont, sizeof(contact)))
	{
		if (cont.ph == num)
		{
			cont.show_contact();
			found = true;
		}
	}
	fp.close();
	if (found == false) {
		cout << "\n\nNo record found..." << endl;
	}
}


class contactNode {
public:
	contact cont;
	contactNode* next;
	contactNode* prev;

	contactNode(contact other) {
		cont.ph = other.ph;
		strcpy(cont.name, other.name);
		strcpy(cont.add, other.add);
		strcpy(cont.email, other.email);
	}

	contactNode& operator=(const contact& other) {
		if (&cont != &other) {
			cont.ph = other.ph;
			strcpy(cont.name, other.name);
			strcpy(cont.add, other.add);
			strcpy(cont.email, other.email);
		}
		return *this;
	}
};


class contact_linkedList {

public:
	contactNode* head = NULL;

	contact_linkedList() {
		fp.open("contactBook.txt", ios::in);
		while (fp.read((char*)&cont, sizeof(contact)))
		{
			insertNode(cont);
		}
		fp.close();
	}

	void insertNode(contact d) {

		contactNode* newNode = new contactNode(d);

		if (head == NULL) {
			head = newNode;
			head->next = NULL;
			return;
		}

		contactNode* temp;
		temp = head;

		while (temp->next != NULL) {
			temp = temp->next;
		}

		temp->next = newNode;
	}

	void display() {
		contactNode* temp = head;
		while (temp != NULL) {
			temp->cont.show_contact();
			cout << endl;
			temp = temp->next;
		}
	}

	vector<contact> toArray() {
		vector<contact> array;
		contactNode* temp = head;
		while (temp != NULL) {
			array.push_back(temp->cont);
			temp = temp->next;
		}
		return array;
	}

	~contact_linkedList() {
		fp.open("contactBook.txt", ios::out | ios::trunc);
		contactNode* temp = head;
		while (temp != NULL) {
			fp.write((char*)&temp->cont, sizeof(contact));
			temp = temp->next;
		}

		fp.close();

	}
};

class contactTreeNode {
public:
	contact cont;
	contactTreeNode* right;
	contactTreeNode* left;

	contactTreeNode(contact other) {
		cont.ph = other.ph;
		strcpy(cont.name, other.name);
		strcpy(cont.add, other.add);
		strcpy(cont.email, other.email);
	}

};


class contact_tree {

	bool flag = 0;

	// Recursive function to build a balanced BST from an array
	contactTreeNode* sortedArrayToBST(vector<contact>& arr, int start, int end) {
		if (start > end) {
			return nullptr;
		}

		int mid = (start + end) / 2;
		contactTreeNode* newNode = new contactTreeNode(arr[mid]);

		newNode->left = sortedArrayToBST(arr, start, mid - 1);
		newNode->right = sortedArrayToBST(arr, mid + 1, end);

		return newNode;
	}

	// Recursive function to insert a contact into the BST
	contactTreeNode* insertRecursive(contactTreeNode* node, contact d) {
		if (node == nullptr) {
			return new contactTreeNode(d);
		}

		if (strcmp(d.name,node->cont.name) < 0) {
			node->left = insertRecursive(node->left, d);
		}
		else if (strcmp(d.name,node->cont.name) > 0) {
			node->right = insertRecursive(node->right, d);
		}

		return node;
	}

	//Recursive function to delelte contact on BST
	contactTreeNode* deleteRecursive(contactTreeNode* current, string value) {
		if (current == NULL) {
			flag = 1;
			return current;
		}

		if (strcmp(value.c_str(), current->cont.name) < 0) {
			current->left = deleteRecursive(current->left, value);
		}
		else if (strcmp(value.c_str(), current->cont.name) > 0) {
			current->right = deleteRecursive(current->right, value);
		}
		else {
			
			if (current->left == NULL) {

				contactTreeNode* temp = current->right;
				delete current;
				return temp;
			}
			else if (current->right == NULL) {
				contactTreeNode* temp = current->left;
				delete current;
				return temp;
			}

			
			contactTreeNode* temp = findMin(current->right);

			
			current->cont = temp->cont;

			
			current->right = deleteRecursive(current->right, temp->cont.name);
		}

		return current;
	}

	contactTreeNode* findMin(contactTreeNode* node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}

	// Helper function to convert BST to linked list
	void bstToLinkedListHelper(contactTreeNode* node, contactNode*& head) {
		if (node == nullptr) {
			return;
		}

		
		bstToLinkedListHelper(node->left, head);

		
		contactNode* newNode = new contactNode(node->cont);

		
		newNode->next = head;
		head = newNode;


		bstToLinkedListHelper(node->right, head);
	}

	// Recursive function to find a contact in the BST
	contactTreeNode* findRecursive(contactTreeNode* node, string d) {
		if (node == nullptr || strcmp(node->cont.name, d.c_str()) == 0) {
			return node;
		}

		if (d < node->cont.name) {
			return findRecursive(node->left, d);
		}
		else if (d > node->cont.name) {
			return findRecursive(node->right, d);
		}

	}

	// Recursive function to edit a contact in the BST
	bool editRecursive(contactTreeNode*& node, string oldName, contact newContact) {
		if (node == nullptr) {
			return false;  
		}

		if (strcmp(oldName.c_str(), node->cont.name) < 0) {
			
			if (editRecursive(node->left, oldName, newContact)) {
				
				if (oldName < node->cont.name) {
					
					rotateRight(node);
				}
				return true;
			}
		}
		else if (strcmp(oldName.c_str(), node->cont.name) > 0) {
			
			if (editRecursive(node->right, oldName, newContact)) {
				
				if (oldName > node->cont.name) {
					
					rotateLeft(node);
				}
				return true;
			}
		}
		else {

			node->cont = newContact;
			return true;
		}

		return false;
	}


	// Recursive function for inorder traversal
	void inorderTraversal(contactTreeNode* node) {
		if (node != nullptr) {
			inorderTraversal(node->left);
			node->cont.show_contact();
			cout << endl;
			inorderTraversal(node->right);
		}
	}

	// Function to perform a left rotation
	void rotateLeft(contactTreeNode*& node) {
		contactTreeNode* newRoot = node->right;
		node->right = newRoot->left;
		newRoot->left = node;
		node = newRoot;
	}

	// Function to perform a right rotation
	void rotateRight(contactTreeNode*& node) {
		contactTreeNode* newRoot = node->left;
		node->left = newRoot->right;
		newRoot->right = node;
		node = newRoot;
	}

public:
	contactTreeNode* root;

	contact_tree() {}

	void listtoBST(contact_linkedList& linkedList) {
		vector<contact> array = linkedList.toArray();
		int size = array.size();
		root = sortedArrayToBST(array, 0, size - 1);
	}

	void insert(contact d) {
		root = insertRecursive(root, d);
	}

	bool dlt(string d) {
		contactTreeNode* temp = deleteRecursive(root, d);
		root = temp;
		if (!flag)
			return 1;
		else
			return 0;
	}

	void inorder() {
		inorderTraversal(root);
	}

	void bstToLinkedList(contact_linkedList& linkedList) {
		linkedList.head = NULL;

		bstToLinkedListHelper(root, linkedList.head);
	}

	contactTreeNode* findContact(string name) {
		return findRecursive(root, name);
	}

	bool editContact(string name, contact& newContact) {
		return editRecursive(root, name, newContact);
	}

};


class contactManager {

	contact_linkedList cl1;
	contact_tree ct1;
	vector<string> recentActivity;
	stack<contact>callHistory;

	void addActivity(const string& activity) {
		recentActivity.push_back(activity);
	}

public:
	contactManager() {
		ct1.listtoBST(cl1);
	}

	void insertContact() {
		contact d;
		d.create_contact();
		ct1.insert(d);

		string name = d.getName();
		string time = Time();
		string activity = "Contact Added with name " + name + " at " + time;
		addActivity(activity);
	}

	void deleteContact(string d) {
		if (ct1.dlt(d)) {
			cout << "Contact deleted successfully.";

			string name = d;
			string time = Time();
			string activity = "Contact Deleted with name " + name + " at " + time;
			addActivity(activity);
		}

		else {
			cout << "Contact not found. Unable to delete." << endl;
		}
	}

	void showContact() {
		ct1.inorder();
	}

	void exit() {
		ct1.bstToLinkedList(cl1);
	}

	void editContact(string name) {
		contact newContact;
		cout << "\n\nEnter details for editing: \n\n";
		newContact.create_contact();

		if (ct1.editContact(name, newContact)) {
			cout << "Contact edited successfully." << endl;

			string time = Time();
			string activity = "Contact Edited with name " + name + " at " + time;
			addActivity(activity);

		}
		else {
			cout << "Contact not found. Unable to edit." << endl;
		}
	}

	void displayRecentActivity() {
		cout << "----- Recent Activity -----\n";
		for (const auto& activity : recentActivity) {
			cout << activity << "\n";
		}
		cout << "-----------------------------\n";
	}

	void makeCall() {
		string name;
			cout << " \nEnter the name of the contact you want call: ";
			cin >> name;

			contactTreeNode* foundContact = ct1.findContact(name);
			if (foundContact != nullptr) {
				foundContact->cont.makeCall();
				callHistory.push(foundContact->cont);
				cout << "Call made to " << name << " successfully." << endl;
			}
			else {
				cout << "Contact not found. Unable to make a call." << endl;
			}
		
	}

	void showCallHistory() {
		cout << "\n\n\t\t================================\n\t\t\tCALL HISTORY\n\t\t================================\n";
		while (!callHistory.empty()) {
			contact call = callHistory.top();
			call.show_contact();
			cout << endl << "Call Time: " << ctime(&call.callTime);
			cout << endl << "=================================================\n" << endl;
			callHistory.pop();
		}
	}
};




int main()
{
	contactManager cms;
	string name;

	cout << "\n\n\n\n\n";
	cout << "\n\t\t\t\t\t **** Welcome to Contact Management System ****";

	while (1)
	{
		int ch, ch1;
		cout << "\n\n\n\t\t\t\t\t\t\t  MAIN MENU\n\t\t\t\t\t\t  =========================\n\t\t\t\t\t\t   [1] Dial Call \n\t\t\t\t\t\t   [2] Activity/Info\n\t\t\t\t\t\t   [3] Settings\n\t\t\t\t\t\t   [0] Exit\n\t\t\t\t\t\t  =========================";    //\n\t\t\t\t\t\t[4] Edit a Contact\n\t\t\t\t\t\t   [5] Delete a Contact\n\t\t\t\t\t\t   [6] Show Recent Activity\n\t\t\t\t\t\t   [7] Make Call\n\t\t\t\t\t\t   [8] Show Call Logs
		cout << " \n\n\t\t\t\t\t\tEnter the choice: ";
		cin >> ch;

		switch (ch)
		{
		case 0: cout << "\n\n\t\t\t\t\t\tThank you for using CMS...\n\n";
			cms.exit();
			return 0;

		case 1://make call


			system("cls");
			cms.makeCall();
			break;


		case 2:// Activity

			system("cls");
			cout << "\n\n\n\t\t\t\t\t\t\t  ACTIVITY\n\t\t\t\t\t\t  =========================\n\t\t\t\t\t\t [1] All Contacts\n\t\t\t\t\t\t [2] Recent Activity \n\t\t\t\t\t\t [3] Call logs";
			cout << " \n\n\t\t\t\t\t\tEnter the choice: ";
			cin >> ch1;
			switch (ch1)
			{
			case 1:
				system("cls");
				cms.showContact();
				break;

			case 2:
				system("cls");
				cms.displayRecentActivity();
				break;

			case 3:
				system("cls");
				cms.showCallHistory();
				break;

			default:
				cms.exit();
				return 0;

			}
			break;

		case 3://settings

			system("cls");
			cout << "\n\n\n\t\t\t\t\t\t\t  SETTINGS\n\t\t\t\t\t\t  =========================\n\t\t\t\t\t\t [1] Add a Contact \n\t\t\t\t\t\t [2] Edit Contact \n\t\t\t\t\t\t [3] Delete a Contact";
			cout << " \n\n\t\t\t\t\t\tEnter the choice: ";
			cin >> ch1;
			switch (ch1)
			{
			case 1:
				system("cls");
				cout << "\n\nEnter Contact Details: \n======================\n" << endl;
				cms.insertContact();
				break;

			case 2:
				system("cls");
				cout << "\n\nEnter name of the contact you want to edit: ";
				cin.ignore();
				cin >> name;
				cms.editContact(name);
				break;

			case 3:
				system("cls");
				cout << "\n\nEnter Name of the contact you want to delete: ";
				cin.ignore();
				cin >> name;
				cms.deleteContact(name);
				break;

			default:
				cms.exit();
				return 0;

			}
			break;


		default:
			cms.exit();
			return 0;
		}

		int opt;
		cout << "\n\n\n\t\t\t\t\t[1] Main Menu\t\t[0] Exit\n\t\t\t\t\t..::Enter the Choice: ";
		cin.ignore();
		cin >> opt;

		switch (opt)
		{
		case 1:
			system("cls");
			continue;

		case 0:
			cout << "\n\n\t\t\t\t\t\tThank you for using CMS...\n\n";
			cms.exit();
			return 0;
		}
	}
	return 0;
}
