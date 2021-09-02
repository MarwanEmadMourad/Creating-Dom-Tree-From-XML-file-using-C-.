#include <iostream>
#include "bits/stdc++.h"

using namespace std;

// XML Node
class Node {
public:
	string tag;
	string text;
	Node* parent;
	vector<Node*> children;

	Node(string tagval = "", string textval = "", Node* p = NULL) :tag{ tagval }, text{ textval }, parent{ p } {
		//cout << "default constructor";
	}
	~Node() {
		// just was making sure that the allocated nodes are being destructed correctly 
		// cout << "Calling destructor"<<endl;
	}
};


// converting the whole file into one string
string readFileIntoString(const string& path) {
	ifstream input_file(path);
	if (!input_file.is_open()) {
		cerr << "Could not open the file - '"
			<< path << "'" << endl;
		return "";
	}
	return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

// generate XML tree
Node* Parse_XML(const string XML_content) {
	// store the root node in order to delete all the nodes after parsing 
	Node* root = new Node();
	Node* curr_node = root;
	Node* prev_node = root;

	int i = 0;
	string curr_tag, curr_text{};

	// Parsing loop and creating XML tree structure
	while (XML_content[i] != '\0') {
		if (XML_content[i] == '<') {
			if (XML_content[i + 1] == '/')
				goto up;
			i++;
			//obtaining the tag
			while (XML_content[i] != '>')
			{
				curr_tag += XML_content[i];
				i++;
				// neglecting the tag attributes
				if (XML_content[i] == ' ')
					break;
			}
			i++;
			// obtaining the text
			while (XML_content[i] != '<')
			{
				curr_text += XML_content[i];
				i++;
			}
			// deleting any trailing spaces or new-line chars
			while (curr_text.size() && (curr_text[0] == ' ' || curr_text[0] == '\n' || curr_text[curr_text.size() - 1] == ' ' || curr_text[curr_text.size() - 1] == '\n'))
			{
				if (curr_text.size() && (curr_text[0] == ' ' || curr_text[0] == '\n'))
					curr_text.erase(0, 1);
				if (curr_text.size() && (curr_text[curr_text.size() - 1] == ' ' || curr_text[curr_text.size() - 1] == '\n'))
					curr_text.erase(curr_text.size() - 1, 1);
			}
			// we haven't yet created our root so we will create it
			if (root->tag == "") {
				root->tag = curr_tag;
				root->text = curr_text;
			}
			// if the root is created then we should create child node to that root
			else {
				prev_node = curr_node;
				curr_node = new Node(curr_tag, curr_text, prev_node);
				prev_node->children.push_back(curr_node);
			}
			curr_tag.clear();
			curr_text.clear();

			// after obtaining the text and creating our node, we either gonna hit a closing tag for the our opened tag or another new tag
			// checking if we hit the closing tag for our opened tag and we are not at the root cause the root has no parent
		up:
			if (XML_content[i + 1] == '/')
			{
				i++;
				while (XML_content[i] != '<')
				{
					i++;
					if (i == XML_content.size())
						break;
				}
				if (curr_node != root)
					curr_node = curr_node->parent;
			}
			continue;
		}
		i++;
	}
	// returning the root node of the constructed tree
	return root;
}

// freeing all the XML tree allocated nodes
void Free_XML(Node* root) {
	if (root) {
		for (auto& child : root->children) {
			Free_XML(child);
			delete child;
		}
		if (root->parent == NULL)
			delete root;
	}
}

// DFS to print all the node (tag - text) values
void DFS(Node* root){
	if (root){
		cout << "Tag: " << root->tag;
		if (root->text.size())
			cout <<setw(30-(root->tag.size()))<< "Value: " << root->text << endl;
		else
			cout << endl;
		for (auto &adj : root->children){
			DFS(adj) ;
		}
	}
}

int main()
{
	// reading the XML file into a srting
	string File_Path{ "./test.xml" };
	string XML_content = readFileIntoString(File_Path);

	// root of our DOM tree
	Node* root = Parse_XML(XML_content);

	// you preform a dfs on the DOM tree to print it 
	DFS(root) ;

	// free-up the allocated tree nodes
	Free_XML(root);
	
	return 0;
}

