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
#include <algorithm>

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

vector<vector<int>> Line_Segment_Generator(vector<int> last_point, int range){
    vector<vector<int>> tmp_line_segment;
    vector<int> point_1 = last_point;
    vector<int> ponit_2 = Point_Generator(range);
    tmp_line_segment.push_back(point_1);
    tmp_line_segment.push_back(ponit_2);
    return tmp_line_segment;
}

bool Is_OverLap(vector<vector<int>> line1, vector<vector<int>> line2 ){
    int x1 = line1[0][0];
    int y1 = line1[0][1];
    int x2 = line1[1][0];
    int y2 = line1[1][1];
    int x3 = line2[0][0];
    int y3 = line2[0][1];
    int x4 = line2[1][0];
    int y4 = line2[1][1];
    double xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    double yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    int judge = 0;
    if(x3==x4 and y3 == y4){
        judge = judge + 1;
    }
    //double xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    //double ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    if (xden == 0 and yden == 0){
        if (((x3 > min(x1,x2)) && (x3 < max(x1,x2))) && ((y3 > min(y1,y2)) && (y3 < max(y1,y2)))){
            judge = judge + 1;
        }
        else if(((x4 > min(x1,x2)) && (x4 < max(x1,x2))) && ((y4 > min(y1,y2)) && (y4 < max(y1,y2)))){
            judge = judge + 1;
        }
        else if(((x1 > min(x3,x4)) && (x1 < max(x3,x4))) && ((y1 > min(y3,y4)) && (y1 < max(y3,y4)))){
            judge = judge + 1;
        }
        else if(((x2 > min(x3,x4)) && (x2 < max(x3,x4))) && ((y2 > min(y3,y4)) && (y2 < max(y3,y4)))){
            judge = judge + 1;
        }
        if (x3==x1 && y3 == y1){
            if (((x4 >= min(x1,x2)) && (x4 <= max(x1,x2))) && ((y4 >= min(y1,y2)) && (y4 <= max(y1,y2)))){
                judge = judge + 1;
            }
            else if(((x2 >= min(x3,x4)) && (x2 <= max(x3,x4))) && ((y2 >= min(y3,y4)) && (y2 <= max(y3,y3)))){
                judge = judge + 1;
            }
        }
        else if (x3==x2 && y3 == y2){
            if (((x4 >= min(x1,x2)) && (x4 <= max(x1,x2))) && ((y4 >= min(y1,y2)) && (y4 <= max(y1,y2)))){
                judge = judge + 1;
            }
            else if(((x1 >= min(x3,x4)) && (x1 <= max(x3,x4))) && ((y1 >= min(y3,y4)) && (y1 <= max(y3,y3)))){
                judge = judge + 1;
            }
        }
        else if (x4==x1 && y4 == y1){
            if (((x3 >= min(x1,x2)) && (x3 <= max(x1,x2))) && ((y3 >= min(y1,y2)) && (y3 <= max(y1,y2)))){
                judge = judge + 1;
            }
            else if(((x2 >= min(x3,x4)) && (x2 <= max(x3,x4))) && ((y2 >= min(y3,y4)) && (y2 <= max(y3,y3)))){
                judge = judge + 1;
            }
        }
        else if (x4==x2 && y4 == y2){
            if (((x3 >= min(x1,x2)) && (x3 <= max(x1,x2))) && ((y3 >= min(y1,y2)) && (y3 <= max(y1,y2)))){
                judge = judge + 1;
            }
            else if(((x1 >= min(x3,x4)) && (x1 <= max(x3,x4))) && ((y1 >= min(y3,y4)) && (y1 <= max(y3,y3)))){
                judge = judge + 1;
            }
        }
    }
    if (judge > 0){
        return true;
    } else{
        return false;
    }
}

bool Is_Cross(vector<vector<int>> line1, vector<vector<int>> line2){
    int x1 = line1[0][0];
    int y1 = line1[0][1];
    int x2 = line1[1][0];
    int y2 = line1[1][1];
    int x3 = line2[0][0];
    int y3 = line2[0][1];
    int x4 = line2[1][0];
    int y4 = line2[1][1];
    double xden = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
    double yden = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    double xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4));
    double ynum = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
    int judge = 0;
    if(x3==x4 && y3 == y4){
        judge = judge + 1;
    }
    if (xden != 0 && yden != 0) {
        double xcoor = xnum / xden;
        double ycoor = ynum / yden;
        if ((xcoor <= max(x1, x2) && xcoor >= min(x1, x2)) && (ycoor <= max(y1, y2) && ycoor >= min(y1, y2))) {
            if ((xcoor <= max(x3, x4) && xcoor >= min(x3, x4)) && (ycoor <= max(y3, y4) && ycoor >= min(y3, y4))) {
                judge = judge + 1;
            }
        }
        if (xcoor == x2 && ycoor == y2){
            if (xcoor == x3 && ycoor == y3){
                judge = 0;
            }
        }
    }
    if (xden == 0 && yden == 0){
        if (x2== x3 && y2 == y3){
            if (((x4 >= min(x1,x2)) && (x4 <= max(x1,x2))) && ((y4 >= min(y1,y2)) && (y4 <= max(y1,y2)))){
                judge = judge + 1;
            } else if (((x1 >= min(x3,x4)) && (x1 <= max(x3,x4))) && ((y1 >= min(y3,y4)) && (y1 <= max(y3,y4)))){
                judge = judge + 1;
            }
        }
    }
    if (judge > 0){
        return true;
    } else{
        return false;
    }

}

//bool Is_Point_Exist(vector<vector<int> > street, vector<int> point){
    //for (vector<int> element : street){
        //if((element[0] == point[0])&& (element[1] == point[1])){
            //return true;
        //}
    //}
    //return false;
//}

//vector<vector<int> > Add_Point(vector<vector<int> > street, vector<int> point){         //function to add point
    //int vector_size = street.size();
    //vector<vector<int> > new_street;
    //int index = 0;
    //int countx = 0;
    //int county = 0;
    //int countl = 0;
    //if (vector_size  == 0){
        //countl = 1;
    //}
    //while(countl == 0 && index < vector_size && countx == 0){                          //check x location
        //if(point[0] <= street[index][0]){
            //countx = countx + 1;
        //}
        //else{
            //index = index + 1;
        //}
    //}
    //while(countl == 0 && index < vector_size && street[index][0] <= point[0] && county == 0){       //check y location
        //if(point[1] < street[index][1]){
            //county = county + 1;
        //}
        //else{
            //index = index + 1;
        //}
    //}

    //for(int i = 0; i < index; i++){
        //new_street.push_back(street[i]);
    //}
    //new_street.push_back(point);
    //for(int i = index; i < vector_size; i++){
        //new_street.push_back(street[i]);
    //}
    //return  new_street;
//}

vector<vector<int> > Street_Generator(int n_number, int c_number,vector<vector<vector<int> > > Street_List, int street_size){
    vector<vector<int> > tmp_street;
    vector<int> last_point;
    vector<vector<int>> tmp_line_segment;
    vector<int> frist_point = Point_Generator(c_number);
    tmp_street.push_back(frist_point);
    int count = 0;
    for (int i = 0; i < n_number; i++) {
        last_point = tmp_street[i];
        int judgeout = 0;
        while(count <= 25 && judgeout ==0){
            int judgeinside = 0;
            tmp_line_segment = Line_Segment_Generator(last_point, c_number);
            //cout<<tmp_line_segment[0][0]<<","<<tmp_line_segment[0][1] <<endl;
            //cout<<tmp_line_segment[1][0]<<","<<tmp_line_segment[1][1] <<endl;
            for(int x = 0; x < tmp_street.size()-1; x++){
                vector<vector<int>> tmp_line1;
                tmp_line1.push_back(tmp_street[x]);
                tmp_line1.push_back(tmp_street[x+1]);

                if (Is_OverLap(tmp_line1, tmp_line_segment)){
                    count = count + 1;
                    judgeinside = judgeinside + 1;
                }
                if (Is_Cross(tmp_line1, tmp_line_segment)){
                    count = count + 1;
                    judgeinside = judgeinside + 1;
                }
            }
            if (street_size > 0){
                for(int y = 0; y < Street_List.size()-1; y++){
                    for(int z;z < Street_List[y].size() - 2;z++){
                        vector<vector<int>> tmp_line1;
                        tmp_line1.push_back(tmp_street[z]);
                        tmp_line1.push_back(tmp_street[z+1]);
                        if (Is_OverLap(tmp_line1, tmp_line_segment)){
                            count = count + 1;
                            judgeinside = judgeinside + 1;
                        }
                        //if (Is_Cross(tmp_line1, tmp_line_segment)){
                            //count = count + 1;
                            //judgeinside = judgeinside + 1;
                        //}
                    }
                }
            }
            if(count == 25){
                cerr << "Error: failed to generate valid input for 25 simultaneous attempts" << endl;
                exit (EXIT_FAILURE);
            }
            if (judgeinside == 0){
                judgeout = judgeout + 1;
                tmp_street.push_back(tmp_line_segment[1]);
            }
        }
    }

    return tmp_street;
}



//bool Is_Street_Exist(vector<vector<vector<int> > > street_list, vector<vector<int> > street){
    //for (vector<vector<int> > e_street: street_list){
        //int counts = 0;
        //for (int i = 0; i < e_street.size(); i++){
            //if (e_street[i][0] == street[i][0] && e_street[i][1] == street[i][1]){
                //counts = counts + 1;
            //}
            //if (counts == e_street.size()){
                //return true;
            //}
        //}
    //}
    //return false;
//}

vector<vector<vector<int> > > Street_List_Generator(int s_number, int n_number, int c_number){
    vector<vector<vector<int> > > tmp_street_list;
    for(int i =0; i < s_number; i++){
        vector<vector<int> >  tmp_street;
        tmp_street = Street_Generator(n_number, c_number, tmp_street_list,i);
        tmp_street_list.push_back(tmp_street);
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

int Read_Command_Line(int argc, vector<char*> arg, char command, int limit, int inital){
    int i = 0;
    while(i < argc){
        if (arg[i][1] == command){
            int tmp_int = atoi(arg[i+1]);
            if (tmp_int >= limit){
                return tmp_int;
            }
            else {
                cerr <<"Error: The command: "<< command <<" value should be bigger or equal to " << limit << endl;
                break;
            }
        }
        i++;
    }
    return inital;
}


int main (int argc, char* argv[]) {
//initialized the random generator
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;
    int count = 0;
    string user_input;
    vector<string> splited_user_input;
    string input_a1;

    vector<char *> arg_list;
    for (int i = 0; i < argc; ++i) {
        arg_list.push_back(argv[i]);
    }
    //update the command line variables
    s = Read_Command_Line(argc, arg_list, 's', 2, 10);
    n = Read_Command_Line(argc, arg_list, 'n', 1, 5);
    l = Read_Command_Line(argc, arg_list, 'l', 5, 5);
    c = Read_Command_Line(argc, arg_list, 'c', 1, 20);

    //vector<vector<int>> line1;
    //vector<vector<int>> line2;
    //vector<int> point1 = {-2,-1};
    //vector<int> point2 = {1,-1};
    //vector<int> point3 = {-1,-2};
    //vector<int> point4 = {-1,0};
    //line1.push_back(point1);
    //line1.push_back(point2);
    //line2.push_back(point3);
    //line2.push_back(point4);
    //cout<< Is_Cross(line1,line2) << endl;


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

    return 0;
}

