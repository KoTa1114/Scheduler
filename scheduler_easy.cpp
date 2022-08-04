#include <bits/stdc++.h>
#define rep(i,n) for (int i=0; i < (n); ++i)
#define all(v) v.begin(),v.end()
using namespace std;
using ll = long long;

/**/

struct Student {
    int id, grade, subject, time, difficulty;
    string name;
};
struct Teacher {
    int id, subject, difficulty;
    int time[3]; //1のとき担当可能
    string name;
};

struct Table {
    int grade, number;
    string student_name, teacher_name, subject;
};

vector<pair<int, int>> student_list; //(difficulty,id)
vector<pair<int, int>> teacher_list; //(difficulty,id)
vector<Table> table_list; //座席表
map<int,Student> from_id_to_student; //生徒IDから生徒情報への写像
map<int,Teacher> from_id_to_teacher; //講師IDから講師情報への写像

void Input_Student(int number_of_student){
    //読み込むファイルのパスを入力する
    const string file_path = "";
    ifstream ifs(file_path);
    if(!ifs) {
        cout << "ファイルが開けませんでした。" << endl;
    }
    for(int i = 0; i < number_of_student ; i++) {
        Student student;
        ifs >> student.id >> student.grade >> student.subject >> student.time >> student.name;
        from_id_to_student[student.id] = student;
    }
}

void Input_Teacher(int number_of_teacher){
    //読み込むファイルのパスを入力する
    const string file_path = "";
    ifstream ifs(file_path);
    if(!ifs) {
        cout << "ファイルが開けませんでした。" << endl;
    }
    for(int i = 0; i < number_of_teacher ; i++) {
        Teacher teacher;
        ifs >> teacher.id >> teacher.subject >> teacher.name >> teacher.time[0] >> teacher.time[1] >> teacher.time[2];
        from_id_to_teacher[teacher.id] = teacher;
    }
}

int Calc_Student_Difficulty_Sub(Student student) {
    int difficulty = 0;
    difficulty += student.grade;
    difficulty += student.subject;
    difficulty += 3 - student.time;
    return difficulty;
}

int Calc_Teacher_Difficulty_Sub(Teacher teacher) {
    int difficulty = 0;
    difficulty += 3 - (teacher.time[0] + teacher.time[1] + teacher.time[2]);
    difficulty += 4 - teacher.subject;
    return difficulty;
}

void Calc_Student_Difficulty() {
    for(auto student : from_id_to_student) {
        pair<int,int> student_difficulty;
        student_difficulty.second = student.second.id;
        student_difficulty.first = Calc_Student_Difficulty_Sub(student.second);
        student_list.emplace_back(student_difficulty);
    }
    sort(student_list.begin(),student_list.end(),greater<int>());
}

void Calc_Teacher_Difficulty() {
    for(auto teacher : from_id_to_teacher) {
        pair<int,int> teacher_difficulty;
        teacher_difficulty.second = teacher.second.id;
        teacher_difficulty.first = Calc_Teacher_Difficulty_Sub(teacher.second);
        teacher_list.emplace_back(teacher_difficulty);
    }
    sort(teacher_list.begin(),teacher_list.end(),greater<int>());
}

void Calc_Difficulty() {
    Calc_Student_Difficulty();
    Calc_Teacher_Difficulty();
}

void Input() {
    Input_Teacher();
    Input_Student();
}



int main(){
    //ファイルから情報を入力する
    Input();
    //情報から優先順位を決定する
    Calc_Difficulty();
    //生徒と講師を配置する
    Create_Table();
    //結果を表示する or ファイルに出力する
    Show_Result();
    return 0;
}