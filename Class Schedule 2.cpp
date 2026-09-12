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

//Depth-First Search Algorithm for the classes
void traverse(std::array<std::array<std::string, 3>, 3>& path, Class* curr, int row, int col, std::list<Class*>& classes) {
    //Class name is put in the first row of the first column in the path variable.
    path.at(row).at(col) = curr->name;
	//The class is removed from the list of classes to avoid duplicates in the path variable.
    classes.remove(curr);
    //We search through the corequisites of every class in the connected component.
    for (Class* s : curr->corequisite) {
        while(path.at(row).at(col) != "") row++;
        if (std::find(classes.begin(), classes.end(), s) != classes.end()) traverse(path, s, row, col, classes);
    }
	//We search through the future class of every class in the connected component.
    for (Class* s : curr->future) {
        row = 0;
        col++;
        while (path.at(row).at(col) != "") row++;
        if(std::find(classes.begin(), classes.end(), s)!=classes.end()) traverse(path, s, row, col, classes);
        col--;
    }
}

//Backward Depth-First Search Algorithm to find the prerequisite class is in the first class of the connected component, which is the farthest prerequisite class from the current class stored in the curr variable.
int reverse(Class*& lowest, Class* curr, int& temp, int& pathLength) {
    //The pathLength variable records the distance of farthest prerequisite class and the current class. When a temp is found to be greater, pathLength's value is replaced by that temp's value.
    if (curr->prequisite.empty()) {
        temp++;
        if (temp > pathLength) {
            lowest = curr;
            return temp;
        }
        else return pathLength;
    }
	//Each time a prerequisite class is found, temp is incremented to record the distance from the current class.
    temp++;
	//The connected component is searched through the prerequisites of every class to find the prerequisite class that is farthest from the current class. To see which class is the farthest, temp is assigned to each of them to remember the distance.
    for (Class* i : curr->prequisite) {
        int now = temp;
        pathLength = reverse(lowest, i, temp, pathLength);
        temp = now;
    }
    return pathLength;
}

//Form a connected component of classes that are prerequisites, corequisites, and future classes of the current class and copy it to the schedule, row by row.
std::array<std::array<std::string, 3>, 3> connectedComponent(Class* curr, std::array<std::array<std::string, 3>, 3>& schedule, std::list<Class*>& classes) {
    std::array<std::array<std::string, 3>, 3> path{};
    int temp = 0;
    int pathLength = 0;
    int col = 0;
    int row = 0;
	//Find the first class of the connected component
    reverse(curr, curr, temp, pathLength);
	//Create a connected component of classes in the path variable
    traverse(path, curr, row, col, classes);
    int i = 0;
	//Put the classes in the connected component into the schedule by searching for an empty spot in the schedule, where r and c are the row and column of the path variable, respectively, and i and j are the row and the column of the schedule respectively.
    for (int c = 0; c < 3; c++) {
        int r = 0;
        if (path.at(r).at(c) == "") continue;
        std::array<std::array<std::string, 3>, 3> temp = schedule;
		//Iterate through the classes in every row, r, and column, c, of the path variable.
        while (path.at(r).at(c) != "" && r < 3) {
            int j = 0;
            //Search for the empty spot of the row j in the schedule.
            while (schedule.at(j).at(i) != "" && j < 2) {
                j++; 
            }
			//If the row is full, reset the schedule to the previous state and move to the next column
            if (j==2&&schedule.at(j).at(i) != "") {
                schedule = temp;
                i++;
            }
            else {
				//If there is an empty spot, we put the class in the path variable into the schedule and move to the next row of the column.
                schedule.at(j).at(i) = path.at(r).at(c);
                r++;
            }
			//If every row of the column in the path variable is searched, we break out of the loop and move to the next column of the path variable.
            if (r == 3)break;
        }
		//We move to the next column of the schedule.
        i++;
    }
    return path;
}

//Iterate through every class in the list and apply the algorithm to form a proper schedule
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
Problem: Create a schedule of 9 classes that is proper. A proper schedule has every class in which the corequisites and prerequisites are fulfilled when taken.

Constraint: There are exactly 9 classes, and the selection is always valid for a schedule.

Approach: We form a graph of classes with three types of edges: prerequisites, corequisites, and future classes.
The prerequisites classes are the classes that must be taken before the current class.
The corequisites classes are the classes that must be taken with the current class.
The future classes are the classes that can be taken after the current class.
Each connected component of the graph is the relationship between the classes in that component. We find this component and copy it to the schedule.
The connected component is found by finding the first prerequisite class and search through every possible corequisite and future classes.

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
