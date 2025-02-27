//
//  array.cpp
//  array
//
//  Created by Santa Yasumaru on 11/6/23.
//

#include <iostream>
    #include <string>
    #include <cassert>
    using namespace std;

int reduplicate(string a[], int n){
    if (n < 0){ //return -1 for any negative array values
        return -1;
    } else {
        for (int i = 0; i < n; ++i){
            a[i] = a[i] + a[i];
        } //for every i, add its own element
        return n; //return array value
    }
}

int locate(const string a[], int n, string target){
    if (n <= 0){ //return -1 for any negative array values
        return -1;
    }
    for (int i = 0; i < n; ++i){
        if (a[i] == target){
            return i; //return i when element equals target
        }
    }

    return -1; //if target is not found return -1
}

int locationOfMax(const string a[], int n){
    if (n <= 0){ //return -1 for any negative array values
        return -1;
    }
    string max = a[0];
    int maxIndex = 0;
    int i = 0;
    for (i = 0; i < n; ++i){
        if (a[i] > max){ //compare the element to max
            max = a[i];
            maxIndex = i; //asssign the index value of the largest string
        }
    }
    return maxIndex; //return index value of largest string
}

int circleLeft(string a[], int n, int pos){
    if (n <= 0){ //return -1 for any negative array values
        return -1;
    }
    string temp = a[pos]; //temporary storage
    for (int i = pos; i < n - 1; ++i){
        a[i] = a[i+1]; //move every element one to the left
    }
    a[n-1] = temp; //bring back the original element to the back
    return pos;
}

int enumerateRuns(const string a[], int n){
    if (n < 0){ //return -1 for any negative array values
        return -1;
    }
    int continous = 0;
    if (n == 0){
        return 0;
    }
    for (int i = 0; i < n - 1; ++i){
        if (a[i] != a[i+1]){
            continous++; //if elements are not same increment the run
        }
    }
    return continous + 1; //add one for the last sequence
}

int flip(string a[], int n){
    if (n < 0){ //return -1 for any negative array values
        return -1;
    }
    int i = 0;
        for (int j = n-1; j > i; --j){ //loop until the middle
            string temp = a[i]; //temporary storage
            a[i] = a[j];
            a[j] = temp;
            i++;
        }
    return n; //return array value
}

int locateDifference(const string a1[], int n1, const string a2[], int n2){
    if (n1 <= 0 || n2 <= 0){ //return -1 for any negative array values
        return -1;
    }
    int i = 0;
        for (int j = 0; j < n2; ++j){
            if (a2[j] != a1[i]){
                return i; //if the elements are different return the index value
            }
            i++;
        }
    if (n1 > n2){ //return the smallest array value if the elements are equal
        return n2;
    } else if (n2 > n1){
        return n1;
    } else {
        return n2;
    }
}

int locateAny(const string a1[], int n1, const string a2[], int n2){
    if (n1 <= 0 || n2 <= 0){ //return -1 for any negative array values
        return -1;
    }
    for (int i = 0; i < n1; ++i){
        for (int j = 0; j < n2; j++){ //loop through the whole second array for each element in the first array
            if (a1[i] == a2[j]){
                return i; //return the index if the element is found in the second array
            }
        }
        }
    return -1; //return -1 if no match is found
}

int divide(string a[], int n, string divider){
    if (n < 0){ //return -1 for any negative array values
        return -1;
    }
    int lessCount = 0; //counts how many elements are below divider
    int i = 0;
    for (i = 0; i < n; ++i){
        if (a[i] < divider){
            string temp = a[lessCount];
            a[lessCount] = a[i];
            a[i] = temp; //swaps elements lower than the divider with lessCount
            lessCount++; //lessCount increments
        }
    }
    for (i = 0; i < n; ++i){
        if (a[i] == divider){ //assigning the element right after the elements less than divider if a[i] is equal to divider
            string temp2 = a[lessCount];
            a[lessCount] = a[i];
            a[i] = temp2;
        }
    }
    return lessCount; //return the index after the elements that were less than the divder
}

int subsequence(const string a1[], int n1, const string a2[], int n2){
    if (n1 < 0 || n2 < 0){ //return -1 for any negative array values
        return -1;
    }
    if (n1 == 0 || n2 == 0){ //return -1 for any negative array values
        return 0;
    }
    int j = 0;
    int og = 0;
    for (int i = 0; i < n1; ++i){
        j = 0;
        while (i <= n1 && j <= n2){
            if (a1[i] == a2[j]){
                if (j == 0){
                    og = i; //assigning og the index value of starting value of the subsequence
                }
                j++; //incrementing j to keep checking for the sequence
            }
        i++;
        }
    }
    if (j != n2){
        return -1; //if j is not equal to n2 means the whole subsequence was not there so return -1
    } else {
        return og; //returning the starting index value of the subsequence
    }
}


    


int main()
    {
        string h[7] = { "nikki", "ron", "asa", "vivek", "", "chris", "donald" };
        assert(locate(h, 7, "chris") == 5);
        assert(locate(h, 7, "asa") == 2);
        assert(locate(h, 2, "asa") == -1);
        assert(locationOfMax(h, 7) == 3);

        string g[4] = { "nikki", "ron", "chris", "tim" };
        assert(locateDifference(h, 4, g, 4) == 2);
        assert(circleLeft(g, 4, 1) == 1 && g[1] == "chris" && g[3] == "ron");

        string c[4] = { "ma", "can", "tu", "do" };
        assert(reduplicate(c, 4) == 4 && c[0] == "mama" && c[3] == "dodo");

        string e[4] = { "asa", "vivek", "", "chris" };
        assert(subsequence(h, 7, e, 4) == 2);

        string d[5] = { "ron", "ron", "ron", "chris", "chris" };
        assert(enumerateRuns(d, 5) == 2);
    
        string f[3] = { "vivek", "asa", "tim" };
        assert(locateAny(h, 7, f, 3) == 2);
        assert(flip(f, 3) == 3 && f[0] == "tim" && f[2] == "vivek");
    
        assert(divide(h, 7, "donald") == 3);
    
        cout << "All tests succeeded" << endl;
    }
