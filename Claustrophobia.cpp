#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>

using namespace std;

int main() {
    clock_t start = clock();
    srand(time(NULL));

    const int countRoom = 2000;
    const int countСorridor = 1999;
    int graph[countRoom - 1][3];
    int pen = 1;
    int vertex = 1;
    int allWay = 0;

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

    // Составляем список ВСЕХ тупиков
    int treeLeaves = 0;
    vector<int> deadends; vector<int> NO_deadends;
    for (int i = 0; i <= countRoom; i++) {
        int rang = 0;
        for (int ii = 0; ii < countСorridor; ii++) {
            if (graph[ii][0] == i || graph[ii][1] == i) {
                rang += 1;
            }
            if (rang > 1) {
                NO_deadends.push_back(i);
                break;
            }
        }
        if (rang == 1) {
            deadends.push_back(i);
            treeLeaves += 1;
        }
    }
    cout << "number of tree leaves = " << treeLeaves << "\n";

    for (int i = 0; i < 1000; i++) { // Количество сценариев
        bool searchScript = true;
        int start, key, trap;
        while (searchScript) {
            start = 1 + rand() % 1999;
            key = 1 + rand() % 1999;
            trap = 1 + rand() % 1999;

            if (start != key != trap) { searchScript = false; }
        }
        cout << "\nScript "<< i + 1 <<" generated successfully\n";

        // Далее мой алгиритм обработки сценария
        clock_t startScript = clock();
        vector<int> routeKey = NO_deadends;
        if (find(routeKey.begin(), routeKey.end(), start) == routeKey.end()) { routeKey.push_back(start); }
        if (find(routeKey.begin(), routeKey.end(), key) == routeKey.end()) { routeKey.push_back(key); }

        // Блок ниже (все что в while) отвечает за поиск маршрута между start и key
        bool cut = true;
        while (cut) {
            cut = false;
            for (int i = 1; i <= countRoom; i++) {
                if (i != start && i != key && (find(routeKey.begin(), routeKey.end(), i) != routeKey.end())) {
                    int rang = 0;
                    for (int ii = 0; ii < countСorridor; ii++) {
                        if ((graph[ii][0] == i && find(routeKey.begin(), routeKey.end(), graph[ii][1]) != routeKey.end()) ||
                            (graph[ii][1] == i && find(routeKey.begin(), routeKey.end(), graph[ii][0]) != routeKey.end())) {
                            rang += 1;
                        }
                    }
                    if (rang == 1) {
                        routeKey.erase(remove(routeKey.begin(), routeKey.end(), i), routeKey.end());
                        cut = true;
                    }
                }
            }
        }

        bool possible = true;
        for (int n : routeKey) {
            if (n == trap) {
                cout << "impossible\n";
                possible = false;
                break;
            }
        }


        if (possible) {
            // Считаем пройденное расстояние routeKey
            int wayKay = 0;
            for (int n : routeKey) {
                for (int i = 0; i < countСorridor; i++) {
                    if (n == graph[i][0] && find(routeKey.begin(), routeKey.end(), graph[i][1]) != routeKey.end()) {
                        wayKay += graph[i][2];
                    }
                }
            }

            int shortWay = 1000000;
            for (int n : deadends) {
                if (n != key && n != start) {
                    vector<int> routeDeadend = NO_deadends;
                    if (find(routeDeadend.begin(), routeDeadend.end(), key) == routeDeadend.end()) { routeDeadend.push_back(key); }
                    if (find(routeDeadend.begin(), routeDeadend.end(), n) == routeDeadend.end()) { routeDeadend.push_back(n); }

                    // Алгоритм
                    bool cut = true;
                    while (cut) {
                        cut = false;
                        for (int i = 1; i <= countRoom; i++) {
                            if (i != key && i != n && (find(routeDeadend.begin(), routeDeadend.end(), i) != routeDeadend.end())) {
                                int rang = 0;
                                for (int ii = 0; ii < countСorridor; ii++) {
                                    if ((graph[ii][0] == i && find(routeDeadend.begin(), routeDeadend.end(), graph[ii][1]) != routeDeadend.end()) ||
                                        (graph[ii][1] == i && find(routeDeadend.begin(), routeDeadend.end(), graph[ii][0]) != routeDeadend.end())) {
                                        rang += 1;
                                    }
                                }
                                if (rang == 1) {
                                    routeDeadend.erase(remove(routeDeadend.begin(), routeDeadend.end(), i), routeDeadend.end());
                                    cut = true;
                                }
                            }
                        }
                    }

                    // Считаем пройденное расстояние routeDeadend (от K до конечной)
                    int wayDeadend = 0;
                    for (int n : routeDeadend) {
                        for (int i = 0; i < countСorridor; i++) {
                            if (n == graph[i][0] && find(routeDeadend.begin(), routeDeadend.end(), graph[i][1]) != routeDeadend.end()) {
                                wayDeadend += graph[i][2];
                            }
                        }
                    }

                    int notOptimalWay = 0;
                    vector<int>affectedVertices = routeDeadend;
                    for (int n : routeKey) {
                        if (find(affectedVertices.begin(), affectedVertices.end(), n) == affectedVertices.end()) {
                            affectedVertices.push_back(n);
                        }
                    }
                    for (int n : affectedVertices) {
                        for (int i = 0; i < countСorridor; i++) {
                            if (n == graph[i][1] && find(affectedVertices.begin(), affectedVertices.end(), graph[i][0]) != affectedVertices.end()) {
                                notOptimalWay += graph[i][2] * 2;
                            }
                        }
                    }

                    int currentWay = allWay - notOptimalWay + wayDeadend + wayKay;
                    if (currentWay < shortWay) { shortWay = currentWay; }
                }
            }
            cout << "Minimum distance traveled = " << shortWay << "\n";
        }
        clock_t endScript = clock(); printf("Algorithm operation time: %f seconds\n", (double)(endScript - startScript) / CLOCKS_PER_SEC);
    }
    clock_t end = clock(); printf("\nFull program execution time: %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
}
