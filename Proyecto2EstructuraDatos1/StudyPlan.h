#pragma once
#ifndef STUDYPLAN_H
#define STUDYPLAN_H
//#define _CRT_SECURE_NO_advertencias
#include <string>


struct Subject {//in memory
	char* name;
	char* code;
	int UV_requirement;
	int UV;
	Subject** son;
	int quantitySon;
	int quantityFather;
	Subject(const char* _code, const char* _name, int _UV_requirement, int _UV, int _quantitySon, int _quantityFather) {
		name = new char[strlen(_name) + 1];
		strcpy_s(this->name, strlen(_name) + 1, _name);
		code = new char[strlen(_code) + 1];
		strcpy_s(this->code, strlen(_code) + 1, _code);
		UV_requirement = _UV_requirement;
		UV = _UV;
		quantitySon = _quantitySon;
		quantityFather = _quantityFather;
		son = 0;
	}
};


struct FileStudyPlan {//in hardDisk
	int code;
	char name[30];
	int quantitySubject;	
	int quantityBlocks;
	int ratingUnits;//unidades valorativas
};


struct FileSubject{//in HardDisk
	char code[7];
	char name[30];
	int UV_requirement;
	int UV;
	int  quantitySon;
	char ListFather[4][7];
	char ListSon[4][7];
	int quantityFather;
};


struct Student{ //in memory
	int accountNumber;
	char* name;
	int codePlan;
	double academyIndex;
	int classApprovedTotal;
};

struct FileStudent {//hardDisk
	int code;
	char name[30];
	int codeSP;
	double averageAcademic;
	int subjectAproved;
};

struct StudentHistory{//save in hardDisk
	char codeC[7];//codigo de la clase
	char name[30];//nombre de la clase;
	int UV;
	int UV_requirement;
	int year;// año de la clase;
	int period;
	double grade;
	bool status;
};

class StudyPlan{ //in memory, this is the class
private: 
	Subject* root;
	Subject* Classes[10];
	int Counting=0;
public:
	StudyPlan();
	int CountingSP = 0;
	bool isEmpty();
	void LoadSon(std::string, char*, char*);
	void CreatePlan();
	void CreateSubject();
	void addStudent();
	bool FindStudyPlan(int);
	bool FindSubject(char*);
	void OpenEnrollment(int);
	void CloseEnrollment();
	bool FindStudent(int);
	void LoadStudyPlan(int);
	void Tree(FileSubject, char*);
	Subject* FindDad(char* a);
	Subject* FindDadRec(Subject*, char*);
	void Print();
	void PrintRec(Subject*);
	void Available();
	void AvailableRec(Subject*);
	bool IfExist(char*);
	void AvailableF(int,char*, bool);
	void AvailableFRec(int,char*);
	void PrintAvailable();
	void CheckClasses(std::string, int);
	void History();
	void DeleteTree();
	void DeleteTreeRec(Subject*);
	int getSize(std::string);
	FileSubject ExtractClass(char*, int);
	void UpdateGrade(int, double ,int);
	void WatchPensul(int);
};

#endif