//Спринт 3/22: Спринт 1 → Тема 2/5: Структуры и классы → Урок 3/11

Структура
В прошлом уроке вы узнали, что pair плохо подходит для хранения id документа и его слов. Нужен другой тип данных. Сценарий применения объектов этого типа уже есть:
if (relevance > 0) {
    matched_documents.push_back({relevance, document.first});
} 
По сути требуется объединить целое число и вектор строк в один тип и дать им названия: id и words. Для такой задачи подходит структура — тип данных, в котором можно объединить переменные разных типов. В С++ структура обозначается ключевым словом struct. Новый тип данных объявляется вне функции main:
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct DocumentContent {
    int id;
    vector<string> words;
};
int main() {
    DocumentContent document_content = {1, {"cat"s, "likes"s, "milk"s}};
    cout << document_content.id << " "s << document_content.words.size() << endl;
} 
Вывод программы:
1 3 
Чтобы объявить структуру, нужны:
Ключевое слово struct. Как и слова вроде using, namespace или int, его нельзя использовать в качестве названия переменной или функции.
Название нового типа. Обычно его пишут с большой буквы, а при наличии нескольких слов в нём — ВотТак. Но в ряде кодовых баз могут быть другие правила. Регистр имеет значение: в нашей программе DocumentContent — тип, а document_content — переменная этого типа. Удобно и понятно.
Типы и названия полей в фигурных скобках — в том же формате, что объявление переменных: int id; и vector<string> words;.
Точка с запятой после фигурных скобок. О ней часто забывают, потому что после таких же фигурных скобок, обозначающих тело функции или цикла, точка с запятой не ставится. Забывать не стоит.
struct DocumentContent {
    int id;
    vector<string> words;
}
// ошибка в этой строке: «error: expected ';' after struct definition»
int main() {
} 
Теперь можно объявить переменную типа DocumentContent и обращаться к полям переменной-структуры по их имени:
DocumentContent document_content;
document_content.id = 3;
document_content.words = {"found"s, "a"s, "furry"s, "cat"s, "with"s, "green"s, "eyes"s}; 
Значения полей можно указать внутри фигурных скобок сразу при объявлении переменной-структуры:
// Документ с id=3 и текстом "найден пушистый кот с зелёными глазами"
DocumentContent document_content = {
    // Значение поля id 
    3,
    // внутри этих фигурных скобок перечислены элементы вектора words
    {"found"s, "a"s, "furry"s, "cat"s, "with"s, "green"s, "eyes"s}
}; 
Так DocumentContent стал полноправным типом. Его можно сделать константным — const DocumentContent. Можно передать в функцию, вернуть из функции. Можно даже создать вектор из DocumentContent:
// Вектор, содержащий два элемента типа DocumentContent
vector<DocumentContent> documents = {
    // белый попугай ищет хозяина
    {1, {"a"s, "white"s, "parrot"s, "is"s, "looking"s, "for"s, "an"s, "owner"s}}, 
    // найдена коричневая морская свинка породы тедди
    {2, {"found"s, "a"s, "brown"s, "teddy", "guinea"s, "pig"s}}, 
}; 
Фигурные скобки позволяют создать структуру «на лету»:
vector<DocumentContent> documents;

int id = ReadLineWithNumber();
vector<string> words = SplitIntoWords(ReadLine());

// Создаём структуру DocumentContent и инициализируем её поля значениями переменных id и words
// Созданная структура добавляется в вектор
documents.push_back({id, words}); 
К полям обращаются только по их названию. Если ошибиться в названии поля, компилятор сообщит об этом:
struct DocumentContent {
    int id;
    vector<string> words;
};

int main() {
    DocumentContent document;
    document.size = 3;
    // error: 'struct DocumentContent' has no member named 'size'
} 