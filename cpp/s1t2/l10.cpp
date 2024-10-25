//Спринт 3/22: Спринт 1 → Тема 2/5: Структуры и классы → Урок 10/11

Константные методы
В прошлом уроке вы разработали класс SearchServer. Он скрывает детали реализации от внешнего мира, используя приватные данные и методы. Для работы с ним служат публичные методы AddDocument, SetStopWords и FindTopDocuments.
Если попробуете объявить объект поисковой системы константным, ничего не выйдет:
int main() {
        const SearchServer search_server = CreateSearchServer();

        const string query = ReadLine();
        for (auto [document_id, relevance] : search_server.FindTopDocuments(query)) {
        // error: passing 'const SearchServer' as 'this' argument discards qualifiers
                cout << "{ document_id = " << document_id << ", relevance = " << relevance << " }" << endl;
        }
} 
Вызвать FindTopDocuments для константного объекта search_server нельзя. Аналогичная ошибка будет, если вызвать, например, push_back от константного вектора. Он на то и константный, чтобы нельзя было изменить его содержимое.
Почему компилятор решил, что FindTopDocuments изменит объект, непонятно. Ведь метод size для константного вектора вызвать можно. Время разобраться, как сделать то же в классе SearchServer.
Ответ логичен и запутан. Методы, не меняющие объект, в контексте которого вызваны, нужно помечать константными. Чтобы стало понятнее, посмотрите на метод FindTopDocuments:
class SearchServer {
public:
    void SetStopWords(const string& text) { /* ... */ }

    void AddDocument(int document_id, const string& document) { /* ... */ }

    vector<Document> FindTopDocuments(const string& raw_query) const /* ВНИМАНИЕ СЮДА! */ {
        const set<string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        sort(matched_documents.begin(), matched_documents.end(), HasDocumentGreaterRelevance);
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;        
    }

private:
    struct DocumentContent {
        int id = 0;
        vector<string> words;
    };

    vector<DocumentContent> documents_;
    set<string> stop_words_;

    vector<Document> FindAllDocuments(const set<string>& query) { /* ... */ }

    /* Прочие методы */
}; 
В заголовок метода FindTopDocuments добавлено слово const — между списком параметров и телом метода. В итоге:
Поля класса при обращении к ним в теле метода стали константными. Изменить их нельзя. С FindTopDocuments таких проблем нет, так как в нём нет обращения к полям напрямую.
Из этого метода нельзя вызывать неконстантные методы, так что FindAllDocuments тоже придётся поправить.
Константный метод FindTopDocuments теперь можно вызвать для константного объекта.
Контекст вызова статических методов класса не связан ни с одним объектом, поэтому статические методы не должны помечаться как константные. Попытка сделать это приведёт к ошибке компиляции:
class SearchServer {
    ...
    static int MatchDocument(const DocumentContent& content, const set<string>& query_words) const {
        // error: static member function 'static int SearchServer::MatchDocument(...)' cannot have cv-qualifier
    }
}; 
