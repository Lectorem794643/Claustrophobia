#include <iostream>
#include <vector>
#include <set>

#include <stdlib.h>
#include <time.h>

using namespace std;

int main() {
    srand(time(NULL));

    // Статические установки графа
    const int countRoom = 2000;
    const int countСorridor = 1999;
    int graph[countRoom - 1][3];
    int pen = 1;
    int vertex = 1;
    int allWay = 0;

    // Генерируем граф
    while (pen < countRoom) {
        int vertexTarget;
        if (pen == 1) { vertexTarget = 1; }
        else { vertexTarget = 1 + rand() % pen; }

        int vertexRank = 1 + rand() % (countRoom - pen);
        for (int i = 0; i < vertexRank; i++) {
            graph[pen + i - 1][0] = vertexTarget;
            graph[pen + i - 1][1] = pen + 1 + i;
            graph[pen + i - 1][2] = rand() % 9 + 1;

            allWay += graph[pen + i - 1][2] * 2;
        }
        pen += vertexRank;
    }
    cout << "Graph generated successfully\n";

    // Все листья нашего дерева
    vector<int> deadends; int rang = 0;
    for (int i = 0; i <= countRoom; i++) {
        rang = 0;
        for (int ii = 0; ii < countСorridor; ii++) {
            if (graph[ii][0] == i || graph[ii][1] == i) { rang += 1; }
            if (rang > 1) { break; }
        }
        if (rang == 1) { deadends.push_back(i); }
    }

    // Количество сценариев
    clock_t start = clock();

    for (int i = 0; i < 10000; i++) {
        bool searchScript = true;
        int Start, Key, Trap;
        while (searchScript) {
            Start = 1 + rand() % 1999;
            Key = 1 + rand() % 1999;
            Trap = 1 + rand() % 1999;

            if (Start != Key != Trap) { searchScript = false; }
        }
        cout << "\nScript " << i + 1 << " generated successfully\n";

        clock_t startScript = clock();

        // Составим маршруты от S до вершины и от К до вершины
        vector <int> KeyUp;    KeyUp.push_back(Key);     // Хранят вершины маршрутов
        vector <int> StartUp;  StartUp.push_back(Start); // Хранят вершины маршрутов

        int ptrKey = Key;      // Указатели для поиска вершин маршрута
        int ptrStart = Start;  // Указатели для поиска вершин маршрута

        for (int i = countСorridor - 1; i >= 0; i--) {
            if (graph[i][1] == ptrKey) {
                KeyUp.push_back(graph[i][0]);
                ptrKey = graph[i][0];
            }
            if (graph[i][1] == ptrStart) {
                StartUp.push_back(graph[i][0]);
                ptrStart = graph[i][0];
            }
        }
         
        // Находим первую общую векршину маршрутов KeyUp и StartUp
        int top = -1;
        for (int n : KeyUp) {
            if (find(StartUp.begin(), StartUp.end(), n) != StartUp.end()) {
                top = n;
                break;
            }
        }

        // Составим маршрут от S до К проходящий через top
        vector <int> KeyStart;
        for (int n : KeyUp) {
            if (n != top) { KeyStart.push_back(n); }
            else { break; }
        }
        for (int n : StartUp) {
            if (n != top) { KeyStart.push_back(n); }
            else { break; }
        }
        KeyStart.push_back(top);
                      
        // Ответим на вопрос, возможно ли пройти игру
        if (find(KeyStart.begin(), KeyStart.end(), Trap) != KeyStart.end()) {
            cout << "\nimposible!";
        }
        else {
            // Найдем пройденный путь от S до K
            int wayKeyStart = 0;
            for (int i = countСorridor - 1; i >= 0; i--) {
                if (find(KeyStart.begin(), KeyStart.end(), graph[i][0]) != KeyStart.end() &&
                    find(KeyStart.begin(), KeyStart.end(), graph[i][1]) != KeyStart.end()) {
                    wayKeyStart += graph[i][2];
                }
            }

            // Предположительно ответ
            int answer = 100000;

            // Теперь будем искать "маршруты" от тупиков до любой вершины KeyUp
            int ptrD; // Указатель для поиска вершин маршрута
            vector <int> DeadUp;      // Хранят вершины маршрутов
            vector <int> DeadKey;
            set <int> DeadKeyStart;

            int WayDeadKey = 0;
            int WayDeadKeyStart = 0;

            for (int n : deadends) {
                if (n != Start && n != Key) {
                    DeadUp.clear();
                    DeadKey.clear();
                    DeadKeyStart.clear();

                    // Маршрут от D вершины
                    ptrD = n; DeadUp.push_back(n);
                    for (int i = countСorridor - 1; i >= 0; i--) {
                        if (graph[i][1] == ptrD) {
                            DeadUp.push_back(graph[i][0]);
                            ptrD = graph[i][0];
                        }
                    }

                    // Находим первую общую векршину маршрутов KeyUp и DeadUp
                    top = -1;
                    for (int n : KeyUp) {
                        if (find(DeadUp.begin(), DeadUp.end(), n) != DeadUp.end()) {
                            top = n;
                            break;
                        }
                    }

                    // Составим маршрут от KeyUp и DeadUp проходящий через top
                    for (int n : KeyUp) {
                        if (n != top) { DeadKey.push_back(n); }
                        else { break; }
                    }
                    for (int n : DeadUp) {
                        if (n != top) { DeadKey.push_back(n); }
                        else { break; }
                    }
                    DeadKey.push_back(top);

                    // Coставим общий маршрут DeadKeyStart
                    for (int n : DeadKey) {
                        DeadKeyStart.insert(n);
                    }
                    for (int n : KeyStart) {
                        DeadKeyStart.insert(n);
                    }

                    // Считаем путь DeadKeyStart и DeadKey
                    WayDeadKey = 0;
                    WayDeadKeyStart = 0;
                    for (int i = countСorridor - 1; i >= 0; i--) {
                        // Считаем путь DeadKeyStart
                        if (find(DeadKeyStart.begin(), DeadKeyStart.end(), graph[i][0]) != DeadKeyStart.end() &&
                            find(DeadKeyStart.begin(), DeadKeyStart.end(), graph[i][1]) != DeadKeyStart.end()) {
                            WayDeadKeyStart += graph[i][2];
                        }
                        // Считаем путь DeadKey
                        if (find(DeadKey.begin(), DeadKey.end(), graph[i][0]) != DeadKey.end() &&
                            find(DeadKey.begin(), DeadKey.end(), graph[i][1]) != DeadKey.end()) {
                            WayDeadKey += graph[i][2];
                        }
                    }
                   
                    // Формула вычесления итогового пройденного растояния
                    int a = (allWay - WayDeadKeyStart * 2) + WayDeadKey + wayKeyStart;
                    if (a < answer) {
                        answer = a;
                    }
                }
            }
            cout << "Minimum distance traveled : " << answer;
            clock_t endScript = clock(); printf("\nAlgorithm operation time: %f seconds\n", (double)(endScript - startScript) / CLOCKS_PER_SEC);
        }
    }
    clock_t end = clock(); printf("\nFull program execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}