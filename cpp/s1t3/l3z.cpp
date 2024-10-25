Задание
В стандартном потоке дана одна строка, которая состоит из числа N и следующего за ним набора S из N слов. Число отделено от слов пробелом.
Отсортируйте набор слов S в алфавитном порядке по возрастанию, игнорируя регистр букв. Выведите их в стандартный поток вывода через пробел. При сравнении слов цифры должны предшествовать буквам: слово «R2D2» находится после слова «R259», так как цифра 5 предшествует любой из букв.
После последнего слова также выведите пробел, а затем перевод строки.
Ознакомьтесь с функцией tolower и алгоритмом lexicographical_compare, чтобы отсортировать слова в алфавитном порядке без учета регистра символов.
Как будет тестироваться ваш код
Гарантируется, что входные данные программы удовлетворяют следующим условиям:
0 <= N <= 1000;
1 <= |S[i]| <= 15;
каждое слово S[i] содержит только цифры, заглавные и строчные буквы английского алфавита;
в наборе слов S нет слов, которые отличаются только регистром символов, например: «apple» и «Apple».
Пример
Ввод
8 quantity AMOUNT Selection Search 4ever music Mickey market 
Вывод 
4ever AMOUNT market Mickey music quantity Search Selection 

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

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

bool CaseInsensitiveCompare(const string& a, const string& b) {
    // Используем lexicographical_compare с переводом символов в нижний регистр
    return lexicographical_compare(
        a.begin(), a.end(), // Первый диапазон
        b.begin(), b.end(), // Второй диапазон
        [](char ch1, char ch2) { return tolower(ch1) < tolower(ch2); } // Лямбда для сравнения символов
    );
}

int main() {
    string s;
    int result = 0;
    cin >> result;
    getline(cin, s);
    vector<string> slova =  SplitIntoWords(s);
    sort(slova.begin(), slova.end(), CaseInsensitiveCompare);
    for (const string& c : slova) {
        cout << c << ' ' ; // ...    
     }
}