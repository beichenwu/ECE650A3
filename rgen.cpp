//
// Created by wu on 10/21/18.
//
#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

vector<int> Point_Generator(int range){
    int x = 0;
    int y = 0;
    vector<int> point;
    FILE *fp;
    fp = fopen("/dev/urandom", "r");
    fread(&x, 1, sizeof(x), fp);
    fread(&y, 1, sizeof(y), fp);
    x = x % (range+1);
    y = y % (range+1);
    fclose(fp);
    point.push_back(x);
    point.push_back(y);
    return point;
}

bool Is_Point_Exist(vector<vector<int> > street, vector<int> point){
    for (vector<int> element : street){
        if((element[0] == point[0])&& (element[1] == point[1])){
            return true;
        }
    }
    return false;
}

vector<vector<int> > Add_Point(vector<vector<int> > street, vector<int> point){         //function to add point
    int vector_size = street.size();
    vector<vector<int> > new_street;
    int index = 0;
    int countx = 0;
    int county = 0;
    int countl = 0;
    if (vector_size  == 0){
        countl = 1;
    }
    while(countl == 0 && index < vector_size && countx == 0){                          //check x location
        if(point[0] <= street[index][0]){
            countx = countx + 1;
        }
        else{
            index = index + 1;
        }
    }
    while(countl == 0 && index < vector_size && street[index][0] <= point[0] && county == 0){       //check y location
        if(point[1] < street[index][1]){
            county = county + 1;
        }
        else{
            index = index + 1;
        }
    }

    for(int i = 0; i < index; i++){
        new_street.push_back(street[i]);
    }
    new_street.push_back(point);
    for(int i = index; i < vector_size; i++){
        new_street.push_back(street[i]);
    }
    return  new_street;
}

vector<vector<int> > Street_Generator(int s_number, int c_number){
    vector<vector<int> > tmp_street;
    for (int i = 0; i < s_number; i++){
        vector<int> tmp_point = Point_Generator(c_number);
        int trail_count = 0;
        int count = 0;
        while(trail_count< 25){
            if(!Is_Point_Exist(tmp_street, tmp_point)){
                tmp_street = Add_Point(tmp_street, tmp_point);
                count = count + 1;
            }
            trail_count = trail_count +1;
        }
        if(count == 0){
            cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
            break;
        }
    }
    return tmp_street;
}

bool Is_Street_Exist(vector<vector<vector<int> > > street_list, vector<vector<int> > street){
    for (vector<vector<int> > e_street: street_list){
        int counts = 0;
        for (int i = 0; i < e_street.size(); i++){
            if (e_street[i][0] == street[i][0] && e_street[i][1] == street[i][1]){
                counts = counts + 1;
            }
            if (counts == e_street.size()){
                return true;
            }
        }
    }
    return false;
}

vector<vector<vector<int> > > Street_List_Generator(int s_number, int n_number, int c_number){
    vector<vector<vector<int> > > tmp_street_list;
    for(int i =0; i < n_number; i++){
        vector<vector<int> >  tmp_street;
        tmp_street = Street_Generator(s_number, c_number);
        int trail_count = 0;
        int count = 0;
        while(trail_count< 25){
            if(!Is_Street_Exist(tmp_street_list, tmp_street)){
                tmp_street_list.push_back(tmp_street);
                count = count + 1;
            }
            trail_count = trail_count +1;
        }
        if(count == 0){
            cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
            break;
        }
    }
    return tmp_street_list;
}

string Street_List_to_String(vector<vector<vector<int> > > street_list){
    string output = "";
    int street_name = 0;
    for (vector<vector<int> > street: street_list){
        string tmp_street = to_string(street_name);
        street_name = street_name + 1;
        output = output + "a" + " " +'"'+ tmp_street + '"'+ " ";
        for (vector<int> point: street){
            output = output + "(" + to_string(point[0]) + "," + to_string(point[1]) + ")" + " ";
        }
        output = output + "\n";
    }
    output = output + "g";
    return output;
}

vector<string> String_Split(string String_Input, string Deliminator){           //A Function split a string and return a vector
    int String_Index = 0;                                                       //Initialized the String Index
    vector<string> Splited_String;                                              //Initialized the Vector
    string Sub_String;                                                          //Initialized the substring
    string String_For_Split = String_Input;                                     //Record the string for split
    while(String_Index != -1){                                                  //Stop the loop if deliminator is not found
        String_Index = String_For_Split.find(Deliminator);
        Sub_String = String_For_Split.substr(0,String_Index);                   //Find the substring
        Splited_String.push_back(Sub_String);                                   //Added the substring to vector
        String_For_Split.erase(0, String_Index+1);                              //Re-size the string
    }
    return Splited_String;
}

string Remove_Street_list(int number){
    string remove_street;
    int streetname = 0;
    for(streetname; streetname< number; streetname++){
        remove_street = remove_street + "r \""+ to_string(streetname)+"\""+"\n";
    }
    return  remove_street;

}

int main (int argc, char **argv) {
//initialized the random generator
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;
    int length;
    int count = 0;
    string user_input;
    vector<string> splited_user_input;
    string input_a1;


    getline(cin, user_input);                                 //Take system standard input
    if (user_input != "") {

        splited_user_input = String_Split(user_input, ",");
        s = stoi(splited_user_input[0].substr(1));
        n = stoi(splited_user_input[1].substr(1));
        l = stoi(splited_user_input[2].substr(1));
        c = stoi(splited_user_input[3].substr(1));
        while(true){
            if (count == 0){
                vector<vector<vector<int> > > street_list = Street_List_Generator(s, n, c);
                string street_list_string = Street_List_to_String(street_list);
                input_a1 = street_list_string;
                cout << input_a1 << endl;
                count = count + 1;
                sleep(l);
            }else{
                vector<vector<vector<int> > > street_list = Street_List_Generator(s, n, c);
                string remove_command = Remove_Street_list(s);
                string street_list_string = Street_List_to_String(street_list);
                input_a1 = remove_command + street_list_string;
                cout << input_a1 << endl;
                sleep(l);
            }

        }

    }
    else{
        cout << "test"<<endl;
    }
    return 0;
}

