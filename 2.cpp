 

using namespace std;

class TreeNode;

class SentenceParser {
public:
    static TreeNode* constructTree(const string& sentence);
    static void constructSubject(TreeNode* parent, vector<string>& words);
    static void constructVerb(TreeNode* parent, vector<string>& words);
    static void constructObject(TreeNode* parent, vector<string>& words);
    static string takeAdjectives(vector<string>& words);
};

class TreeNode {
public:
    string label;
    string word;
    vector<TreeNode*> children;

    TreeNode(string label, string word = "") : label(label), word(word) {}

    void addChild(TreeNode* child) {
        children.push_back(child);
    }

    void printTree(int level = 0) {
        cout << string(2 * level, ' ') << label << " " << (word.empty() ? "" : word) << endl;
        for (vector<TreeNode*>::iterator it = children.begin(); it != children.end(); ++it) {
            (*it)->printTree(level + 1);
        }
    }

    ~TreeNode() {
        for (vector<TreeNode*>::iterator it = children.begin(); it != children.end(); ++it) {
            delete *it;
        }
    }
};

TreeNode* SentenceParser::constructTree(const string& sentence) {
    istringstream iss(sentence);
    vector<string> words;
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(words));

    TreeNode* tree = new TreeNode("S");
    constructSubject(tree, words);
    constructVerb(tree, words);
    constructObject(tree, words);

    return tree;
}

void SentenceParser::constructSubject(TreeNode* parent, vector<string>& words) {
    if (words.empty()) return;

    string determiner = words.front();
    words.erase(words.begin());

    string adjective = takeAdjectives(words);
    string noun = words.front();
    words.erase(words.begin());

    TreeNode* subjectNode = new TreeNode("NP");
    parent->addChild(subjectNode);

    subjectNode->addChild(new TreeNode("Det", determiner));
    subjectNode->addChild(new TreeNode("Adj", adjective));
    subjectNode->addChild(new TreeNode("N", noun));
}

void SentenceParser::constructVerb(TreeNode* parent, vector<string>& words) {
    if (words.empty()) return;

    string verb = words.front();
    words.erase(words.begin());

    TreeNode* verbNode = new TreeNode("V", verb);
    parent->addChild(verbNode);
}

void SentenceParser::constructObject(TreeNode* parent, vector<string>& words) {
    if (words.empty()) return;

    string preposition = words.front();
    words.erase(words.begin());

    string determiner = words.front();
    words.erase(words.begin());

    string adjective = takeAdjectives(words);
    string noun = words.front();
    words.erase(words.begin());

    TreeNode* objectNode = new TreeNode("NP");
    parent->addChild(objectNode);

    objectNode->addChild(new TreeNode("P", preposition));
    objectNode->addChild(new TreeNode("Det", determiner));
    objectNode->addChild(new TreeNode("Adj", adjective));
    objectNode->addChild(new TreeNode("N", noun));
}

string SentenceParser::takeAdjectives(vector<string>& words) {
    string adjectives;
    while (!words.empty() && words.front() == "Adjective") {
        adjectives += words.front() + " ";
        words.erase(words.begin());
    }
    return adjectives;
}

int main() {
    string sentence = "The quick brown fox jumps over the lazy dog.";
    SentenceParser parser;
    TreeNode* tree = parser.constructTree(sentence);
    tree->printTree();

    delete tree;

    return 0;
}
