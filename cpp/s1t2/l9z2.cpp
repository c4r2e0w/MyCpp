Задание 2
Чтобы добавить документ в поисковый индекс, служит метод AddDocument. Однако поисковая система хранит кроме документов ещё и стоп-слова. Раньше для парсинга стоп-слов служила функция ParseStopWords, а чтобы сохранить их, нужно было заводить отдельную переменную типа set<string>:
int main() {
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);
    ...
} 
 Теперь, когда множество стоп-слова стало частью класса поисковой системы, можно сделать лучше: превратить свободную функцию ParseStopWords в метод SetStopWords. Он всё так же будет принимать на вход строку с разделёнными пробелом словами, но не будет создавать и возвращать множество, а сразу добавит эти слова в соответствующее поле класса. Использовать такую поисковую систему станет проще:
 int main() {
    SearchServer search_server;
    search_server.SetStopWords("a the in of an with"s);   
    search_server.AddDocument(1, "a small dog with brown eyes"s);
 }
  
 Добавьте в класс SearchServer публичный метод SetStopWords.
Как будет тестироваться ваш код
 Тренажёр проверит работу методов AddDocument и SetStopWords. Содержимое функции main будет проигнорировано.

 #include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/*
const int MAX_RESULT_DOCUMENT_COUNT = 5;
string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}
int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}
*/

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

/*
set<string> ParseStopWords(const string& text) {
    set<string> stop_words;
    for (const string& word : SplitIntoWords(text)) {
        stop_words.insert(word);
    }
    return stop_words;
}
struct Document {
    int id;
    int relevance;
};
bool HasDocumentGreaterRelevance(const Document& lhs, const Document& rhs) {
    return lhs.relevance > rhs.relevance;
}
set<string> ParseQuery(const string& text, const set<string>& stop_words) {
    set<string> query_words;
    for (const string& word : SplitIntoWordsNoStop(text, stop_words)) {
        query_words.insert(word);
    }
    return query_words;
}
int MatchDocument(const DocumentContent& content, const set<string>& query_words) {
    if (query_words.empty()) {
        return 0;
    }
    set<string> matched_words;
    for (const string& word : content.words) {
        if (matched_words.count(word) != 0) {
            continue;
        }
        if (query_words.count(word) != 0) {
            matched_words.insert(word);
        }
    }
    return static_cast<int>(matched_words.size());
}
vector<Document> FindAllDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& query_words) {
    vector<Document> matched_documents;
    for (const auto& document : documents) {
        const int relevance = MatchDocument(document, query_words);
        if (relevance > 0) {
            matched_documents.push_back({document.id, relevance});
        }
    }
    return matched_documents;
}
vector<Document> FindTopDocuments(const vector<DocumentContent>& documents,
                                  const set<string>& stop_words, const string& raw_query) {
    const set<string> query_words = ParseQuery(raw_query, stop_words);
    auto matched_documents = FindAllDocuments(documents, query_words);
    sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}
*/

class SearchServer {
public:
    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        documents_.push_back({document_id, words});
    }

    void SetStopWords(const string& text) {
        set<string> stop_words;
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }    
    }
    
private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };

    vector<DocumentContent> documents_;

    set<string> stop_words_;

    bool IsStopWord(const string& word) {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }
};

int main() {
    SearchServer search_server;
    search_server.AddDocument(1, "brown cat likes milk"s);
}

/*
int main() {
    const string stop_words_joined = ReadLine();
    const set<string> stop_words = ParseStopWords(stop_words_joined);
    // Read documents
    vector<DocumentContent> documents;
    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        AddDocument(documents, stop_words, document_id, ReadLine());
    }
    const string query = ReadLine();
    for (auto [document_id, relevance] : FindTopDocuments(documents, stop_words, query)) {
        cout << "{ document_id = "s << document_id << ", relevance = "s << relevance << " }"s
             << endl;
    }
}
*/

/*Добавьте в SearchServer приватное поле stop_words_ типа set<string>. В этом поле будут храниться стоп-слова поисковой системы.
Перенесите функцию ParseStopWords внутрь класса (под public:!) и переименуйте в SetStopWords.
Полученные стоп-слова добавляйте сразу во множество stop_words_. Для этого используйте метод insert.
Возвращаемый тип метода SetStopWords должен быть void.*/