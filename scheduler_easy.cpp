#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stdlib.h>

/*生徒の人数 N 講師の人数 M とするとCreate_Tableがボトルネックで計算量は(NMlogM)*/


class Student {
    int id, grade, subject, time;
    std::string name;
    public:
    void SetId(int id_) {
        if(id_ >= 0) {
            id = id_;
        } else {
            std::cout << "idの入力にエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetGrade(int grade_) {
        if(1 <= grade_ && grade_ <= 3) {
            grade = grade_;
        } else {
            std::cout << "gradeにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetSubject(int subject_) {
        if(0 <= subject_ && subject_ <= 4) {
            subject = subject_;
        } else {
            std::cout << "subjectにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetTime(int time_) {
        if(1 <= time_ && time_ <= 3) {
            time = time_;
        } else {
            std::cout << "timeにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetName(std::string name_) {
        if(!name_.empty()) {
            name = name_;
        } else {
            std::cout << "nameにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    int GetId() {return id;}
    int GetGrade() {return grade;}
    int GetSubject() {return subject;}
    int GetTime() {return time;}
    std::string GetName() {return name;}
};
class Teacher {
    int id, subject;
    int time[3]; //1のとき担当可能
    std::string name;
    public:
    void SetId(int id_) {
        if(id_ >= 0) {
            id = id_;
        } else {
            std::cout << "idにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetSubject(int subject_) {
        if(0 <= subject_ && subject_ <= 4) {
            subject = subject_;
        } else {
            std::cout << "subjectにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetTime(int index_, int time_) {
        if(0 <= index_ && index_ <= 2 && 0 <= time_ && time_ <= 1) {
            time[index_] = time_;
        } else {
            std::cout << "timeにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetName(std::string name_) {
        if(!name_.empty()) {
            name = name_;
        } else {
            std::cout << "nameにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    int GetId() {return id;}
    int GetSubject() {return subject;}
    int GetTime(int index) {return time[index];}
    std::string GetName() {return name;}
};

class Table {
    int grade, number;
    std::string student_name, teacher_name, subject;
    public:
    void SetGrade(int grade_) {
        if(1 <= grade_ && grade_ <= 3) {
            grade = grade_;
        } else {
            std::cout << "gradeにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetNumber(int number_) {
        if(1 <= number_) {
            number = number_;
        } else {
            std::cout << "numberにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetStudent_name(std::string student_name_) {
        if(!student_name_.empty()) {
            student_name = student_name_;
        } else {
            std::cout << "student_nameにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetTeacher_name(std::string teacher_name_) {
        if(!teacher_name_.empty()) {
            teacher_name = teacher_name_;
        } else {
            std::cout << "teacher_nameにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    void SetSubject(std::string subject_) {
        if(!subject_.empty()) {
            subject = subject_;
        } else {
            std::cout << "subjectにエラーがあります" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    int GetGrade() {return grade;}
    int GetNumber() {return number;}
    std::string GetStudent_name() {return student_name;}
    std::string GetTeacher_name() {return teacher_name;}
    std::string GetSubject() {return subject;}
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
        int id, grade, subject, time;
        std::string name;
        ifs >> id >> grade >> subject >> time >> name;
        student.SetId(id);
        student.SetGrade(grade);
        student.SetSubject(subject);
        student.SetTime(time);
        student.SetName(name);
        from_id_to_student[student.GetId()] = student;
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
        int id, subject, time0, time1, time2;
        std::string name;
        ifs >> id >> subject >> name >> time0 >> time1 >> time2;
        teacher.SetId(id);
        teacher.SetSubject(subject);
        teacher.SetTime(0,time0);
        teacher.SetTime(1,time1);
        teacher.SetTime(2,time2);
        teacher.SetName(name);
        from_id_to_teacher[teacher.GetId()] = teacher;
    }
}

int Calc_Student_Difficulty_Sub(Student student) {
    int difficulty = 0;
    difficulty += student.GetGrade();
    difficulty += student.GetSubject();
    difficulty += 3 - student.GetTime();
    return difficulty;
}

int Calc_Teacher_Difficulty_Sub(Teacher teacher) {
    int difficulty = 0;
    difficulty += 3 - (teacher.GetTime(0) + teacher.GetTime(1) + teacher.GetTime(2));
    difficulty += 4 - teacher.GetSubject();
    return difficulty;
}

void Calc_Student_Difficulty() {
    for(auto student : from_id_to_student) {
        std::pair<int,int> student_difficulty;
        student_difficulty.second = student.second.GetId();
        student_difficulty.first = Calc_Student_Difficulty_Sub(student.second);
        student_list.push_back(student_difficulty);
    }
    sort(student_list.begin(),student_list.end(),std::greater<std::pair<int,int>>());
}

void Calc_Teacher_Difficulty() {
    for(auto teacher : from_id_to_teacher) {
        std::pair<int, int> teacher_difficulty;
        teacher_difficulty.second = teacher.second.GetId();
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
            if(student.GetSubject() == teacher.GetSubject()) {
                if(was.find(std::make_pair(teacher.GetId(),student.GetTime())) == was.end() && teacher.GetTime(student.GetTime()-1) == 1) {
                    Table table;
                    table.SetGrade(student.GetGrade());
                    table.SetNumber(table_num[student.GetTime()-1]);
                    table.SetStudent_name(student.GetName());
                    table.SetTeacher_name(teacher.GetName());
                    table.SetSubject(Subject(student.GetSubject()));
                    if(student.GetTime() == 1) table_list1.push_back(table);
                    if(student.GetTime() == 2) table_list2.push_back(table);
                    if(student.GetTime() == 3) table_list3.push_back(table);
                    was.insert(std::make_pair(teacher.GetId(),student.GetTime()));
                    table_num[student.GetTime()-1]++;
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

void Show_Result_Sub(std::vector<Table>& table_list) {
    for(auto table : table_list) {
        std::cout << "座席番号: " << table.GetNumber() << std::endl; 
        std::cout << table.GetStudent_name() << ' ' << table.GetGrade() << ' ' << table.GetSubject() << std::endl;
        std::cout << "担当講師: " << table.GetTeacher_name() << std::endl;
        std::cout << std::endl;
    }
}

void Show_Result() {
    std::cout << "1限" << std::endl;
    std::cout << std::endl;
    Show_Result_Sub(table_list1);
    std::cout << "2限" << std::endl;
    std::cout << std::endl;
    Show_Result_Sub(table_list2);
    std::cout << "3限" << std::endl;
    std::cout << std::endl;
    Show_Result_Sub(table_list3);
    if(cannot_place_student.size() != 0) {
        std::cout << "配置できなかった生徒がいます。" << std::endl;
        for(Student student : cannot_place_student) {
            std::cout << "生徒氏名: " << student.GetName() << "さんを配置できませんでした。" << std::endl;
        }
    } else {
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