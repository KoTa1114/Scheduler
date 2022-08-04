#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>

/*生徒の人数 N 講師の人数 M とするとCreate_Tableがボトルネックで計算量は(NMlogM)*/


struct Student {
    int id, grade, subject, time, difficulty;
    std::string name;
};
struct Teacher {
    int id, subject, difficulty;
    int time[3]; //1のとき担当可能
    std::string name;
};

struct Table {
    int grade, number;
    std::string student_name, teacher_name, subject;
};

std::vector<std::pair<int, int>> student_list; //(difficulty,id)
std::vector<std::pair<int, int>> teacher_list; //(difficulty,id)
std::vector<Table> table_list1; //座席表
std::vector<Table> table_list2;
std::vector<Table> table_list3;
std::vector<Student> cannot_place_student; //配置できなかった生徒
std::map<int,Student> from_id_to_student; //生徒IDから生徒情報への写像
std::map<int,Teacher> from_id_to_teacher; //講師IDから講師情報への写像


void Input_Student(int number_of_student){
    //読み込むファイルのパスを入力する
    const std::string file_path = "student.prn";
    std::ifstream ifs(file_path);
    if(!ifs) {
        std::cout << "ファイルが開けませんでした。" << std::endl;
    }
    for(int i = 0; i < number_of_student ; i++) {
        Student student;
        ifs >> student.id >> student.grade >> student.subject >> student.time >> student.name;
        from_id_to_student[student.id] = student;
    }
}

void Input_Teacher(int number_of_teacher){
    //読み込むファイルのパスを入力する
    const std::string file_path = "teacher.prn";
    std::ifstream ifs(file_path);
    if(!ifs) {
        std::cout << "ファイルが開けませんでした。" << std::endl;
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
        std::pair<int,int> student_difficulty;
        student_difficulty.second = student.second.id;
        student_difficulty.first = Calc_Student_Difficulty_Sub(student.second);
        student_list.push_back(student_difficulty);
    }
    sort(student_list.begin(),student_list.end(),std::greater<std::pair<int,int>>());
}

void Calc_Teacher_Difficulty() {
    for(auto teacher : from_id_to_teacher) {
        std::pair<int, int> teacher_difficulty;
        teacher_difficulty.second = teacher.second.id;
        teacher_difficulty.first = Calc_Teacher_Difficulty_Sub(teacher.second);
        teacher_list.push_back(teacher_difficulty);
    }
    sort(teacher_list.begin(),teacher_list.end(),std::greater<std::pair<int,int>>());
}

void Input(int number_of_student, int number_of_teacher) {
    Input_Teacher(number_of_teacher);
    Input_Student(number_of_student);
}

void Calc_Difficulty() {
    Calc_Student_Difficulty();
    Calc_Teacher_Difficulty();
}

std::string Subject(int num) {
    if(num == 0) return "Japanese";
    if(num == 1) return "Sociology";
    if(num == 2) return "English";
    if(num == 3) return "Science";
    if(num == 4) return "Math";
    return "";
}

void Create_Table() {
    std::vector<int> table_num(3,1);
   std::set<std::pair<int,int>> was; //講師がその時間に既に配置済みであるかを管理する
    for(auto a_student : student_list) {
        Student student = from_id_to_student[a_student.second];
        bool ok = false;
        for(auto a_teacher : teacher_list) {
            Teacher teacher = from_id_to_teacher[a_teacher.second];
            if(student.subject == teacher.subject) {
                if(was.find(std::make_pair(teacher.id,student.time)) == was.end() && teacher.time[student.time-1] == 1) {
                    Table table;
                    table.grade = student.grade;
                    table.number = table_num[student.time-1];
                    table.student_name = student.name;
                    table.teacher_name = teacher.name;
                    table.subject = Subject(student.subject);
                    if(student.time == 1) table_list1.push_back(table);
                    if(student.time == 2) table_list2.push_back(table);
                    if(student.time == 3) table_list3.push_back(table);
                    was.insert(std::make_pair(teacher.id,student.time));
                    table_num[student.time-1]++;
                    ok = true;
                    break;
                }
            }
        }
        if(ok == false) {
            cannot_place_student.push_back(student);
        }
    }
}

void Show_Result() {
    std::cout << "1限" << std::endl;
    std::cout << std::endl;
    for(auto table : table_list1) {
        std::cout << "座席番号: " << table.number << std::endl;
        std::cout << table.student_name << ' ' << table.grade << ' ' << table.subject << std::endl;
        std::cout << "担当講師: " << table.teacher_name << std::endl;
        std::cout << std::endl;
    }
    std::cout << "2限" << std::endl;
    std::cout << std::endl;
    for(auto table : table_list2) {
        std::cout << "座席番号: " << table.number << std::endl;
        std::cout << table.student_name << ' ' << table.grade << ' ' << table.subject << std::endl;
        std::cout << "担当講師: " << table.teacher_name << std::endl;
        std::cout << std::endl;
    }
    std::cout << "3限" << std::endl;
    std::cout << std::endl;
    for(auto table : table_list3) {
        std::cout << "座席番号: " << table.number << std::endl;
        std::cout << table.student_name << ' ' << table.grade << ' ' << table.subject << std::endl;
        std::cout << "担当講師: " << table.teacher_name << std::endl;
        std::cout << std::endl;
    }
    if(cannot_place_student.size() != 0) {
        std::cout << "配置できなかった生徒がいます。" << std::endl;
        for(Student student : cannot_place_student) {
            std::cout << "生徒氏名: " << student.name << "さんを配置できませんでした。" << std::endl;
        }
    }
    else {
        std::cout << "問題なく全ての生徒を配置することができました。" << std::endl;
    }
}

int main(){
    //ファイルから情報を入力する
    Input(10,10);
    //情報から優先順位を決定する
    Calc_Difficulty();
    //生徒と講師を配置する
    Create_Table();
    //結果を表示する
    Show_Result();
    return 0;
}