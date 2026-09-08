// Class Schedule 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <list>
#include <array>


class Class {
public:
    std::list<Class*> prequisite;
    std::list<Class*> future;
    std::list<Class*> corequisite;
    std::string name;

    Class(std::string name) {
        this->name = name;
    }

    void Addco(Class* c) {
        this->corequisite.push_back(c);
    }

    void Addpre(Class* c) {
        this->prequisite.push_back(c);
    }

    void Addfu(Class* c) {
        this->future.push_back(c);
    }

};

void traverse(std::array<std::array<std::string, 3>, 3>& path, Class* curr, int row, int col, std::list<Class*>& classes) {
    path.at(row).at(col) = curr->name;
    classes.remove(curr);
    for (Class* s : curr->corequisite) {
        while(path.at(row).at(col) != "") row++;
        if (std::find(classes.begin(), classes.end(), s) != classes.end()) traverse(path, s, row, col, classes);
    }
    for (Class* s : curr->future) {
        row = 0;
        col++;
        while (path.at(row).at(col) != "") row++;
        if(std::find(classes.begin(), classes.end(), s)!=classes.end()) traverse(path, s, row, col, classes);
        col--;
    }
}

int reverse(Class*& lowest, Class* curr, int& temp, int& pathLength) {
    if (curr->prequisite.empty()) {
        temp++;
        if (temp > pathLength) {
            lowest = curr;
            return temp;
        }
        else return pathLength;
    }
    temp++;
    for (Class* i : curr->prequisite) {
        int now = temp;
        pathLength = reverse(lowest, i, temp, pathLength);
        temp = now;
    }
    return pathLength;
}

std::array<std::array<std::string, 3>, 3> connectedComponent(Class* curr, std::array<std::array<std::string, 3>, 3>& schedule, std::list<Class*>& classes) {
    std::array<std::array<std::string, 3>, 3> path{};
    int temp = 0;
    int pathLength = 0;
    int col = 0;
    int row = 0;
    reverse(curr, curr, temp, pathLength);
    traverse(path, curr, row, col, classes);
    int i = 0;
    for (int c = 0; c < 3; c++) {
        int r = 0;
        if (path.at(r).at(c) == "") continue;
        std::array<std::array<std::string, 3>, 3> temp = schedule;
        while (path.at(r).at(c) != "" && r < 3) {
            int j = 0;
            while (schedule.at(j).at(i) != "" && j < 2) {
                j++; 
            }
            if (j==2&&schedule.at(j).at(i) != "") {
                schedule = temp;
                i++;
            }
            else { 
                schedule.at(j).at(i) = path.at(r).at(c);
                r++;
            }
            if (r == 3)break;
        }
        i++;
    }
    return path;
}

void scheduling(std::array<std::array<std::string, 3>, 3>& schedule, std::list<Class*>& classes) {
    while (!classes.empty()){ 
        Class* curr = classes.front();
        connectedComponent(curr, schedule, classes); 
    }
    
    std::cout << "Q1: " << "Q2: " << "Q3: " << "\n";
    for (std::array<std::string, 3> arr : schedule) {
        for (std::string s : arr) {
            if (s == "") std::cout << "    "; else std::cout << s << "   ";
        }
        std::cout << "\n";
    }
}

int main()
{
    std::array<std::array<std::string, 3>, 3> schedule{};
    Class a("A");
    Class b("B");
    Class c("C");
    Class d("D");
    Class e("E");
    Class f("F");
    Class g("G");
    Class h("H");
    Class l("L");
    a.Addfu(&b);
    b.Addpre(&a);
    a.Addfu(&c);
    c.Addpre(&a);
    d.Addfu(&a);
    a.Addpre(&d);
    e.Addfu(&f);
    f.Addpre(&e);
    f.Addfu(&g);
    g.Addpre(&f);
    a.Addco(&f);
    std::list<Class*> classes{ &e, &b, &c, &d, &a, &f, &g, &h, &l };
    scheduling(schedule, classes);
    
}

/*
Problem: Create a schedule of 9 classes that is proper. A proper class is a class in which every corequisite and prerequisite is fulfilled to be taken.

Constraint: There are exactly 9 classes, and the selection is always valid for a schedule.

Approach: We select one class and create an order of corequisite and prequisite that should be taken. We do this by first identifying the first class in the series with no prerequisite and then search from it to the end of the series.
From this order, we search every possible row and column in the schedule to fit in. We search row by row and then column by column. Because we search row by row, long series are guaranteed to have enough columns to fit in.

Complexity:
Space: Because we need an order of every class, we have a space complexity of O(n) to create these orders.
Time: When we search for a spot in the schedule, the worst case is that we search every spot for each class. Therefore, the time complexity is O(n^2).
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
