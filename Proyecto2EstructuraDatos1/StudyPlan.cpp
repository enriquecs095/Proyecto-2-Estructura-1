#include "StudyPlan.h"
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <string>

StudyPlan::StudyPlan():root(0){}

bool StudyPlan::isEmpty() { return root == 0; }

void StudyPlan::CreatePlan(){
	FileStudyPlan save;
	FileSubject saveS;
	std::string path;
	std::string tmp;
	std::cout << "Write the code of the Study Plan: ";
	std::cin >> save.code;
	if (FindStudyPlan(save.code)) {
		std::cout << "Study Plan already exist\n";
	}else{
		std::cout << "Write the name of the Study Plan: ";
		std::cin.ignore();
		std::getline(std::cin, tmp);
		strcpy_s(save.name, tmp.size() + 1, tmp.c_str());
		strcpy_s(saveS.name, tmp.size() + 1, tmp.c_str());
		std::cout << "Write the quantity of subjects: ";
		std::cin >> save.quantitySubject;
		std::cout << "Write the rating units: ";
		std::cin >> save.ratingUnits;
		std::cout << "Write the quantity of blocks: ";
		std::cin >> save.quantityBlocks;
		strcpy_s(saveS.code, 7, "000000");
		saveS.quantityFather = 0;
		saveS.quantitySon = save.quantityBlocks;
		saveS.UV = 0;
		saveS.UV_requirement = 0;
		for (int i = 0; i < save.quantityBlocks; i++)
		{
			std::cout << "Write the code of the class that starts the "<<i+1<<" block: ";
			std::cin >> saveS.ListSon[i];
		}
		path = "Files/" + std::to_string(save.code)+".dat";
		std::ofstream File(path, std::ios::out | std::ios::binary | std::ios::app);
		if (!File){
			std::cout << "Error reading File\n";
			return;
		}
		else {
			File.write(reinterpret_cast<const char*> (&save), sizeof(FileStudyPlan));
			File.write(reinterpret_cast<const char*> (&saveS), sizeof(FileSubject));
			File.close();
			std::cout << "Study Plan created\n";
		}
	}
}

void StudyPlan::CreateSubject(){
	FileSubject save;
	std::string path;
	std::string tmp;;
	int codeStudyPlan;
	std::cout << "Write the code of the Study Plan: ";
	std::cin >> codeStudyPlan;
	if (FindStudyPlan(codeStudyPlan)){
		std::cout << "Write the code of the subject: ";
		std::cin >> save.code;
		std::cout << "Write name of the subject: ";
		std::cin.ignore();
		std::getline(std::cin, tmp);
		strcpy_s(save.name, tmp.size() + 1, tmp.c_str());
		std::cout << "Write the UV requirement for this class: ";
		std::cin >> save.UV_requirement;
		std::cout << "Write the UV of the class: ";
		std::cin >> save.UV;
		std::cout << "Write the quantity of the subjects that open: ";
		std::cin >> save.quantitySon;
		if (save.quantitySon > 0) {
			for (int i = 0; i < save.quantitySon; i++)
			{
				std::cout << "Write the code of the subject that open: ";
				std::cin >> save.ListSon[i];
			}
		}else {
			for (int i = 0; i < save.quantityFather; i++)
			{
				strcpy_s(save.ListSon[i], 7, "000000");
			}
		}
		std::cout << "Write the quantity of the subject required:";
		std::cin >> save.quantityFather;
		if (save.quantityFather > 0) {
			for (int i = 0; i < save.quantityFather;i++)
			{
				std::cout << "Write the code of the subject required: ";
				std::cin >> save.ListFather[i];
			}
		}else {
			for (int i = 0; i < save.quantityFather; i++)
			{
				strcpy_s(save.ListFather[i], 7, "000000");
			}
		}
				path = "Files/" + std::to_string(codeStudyPlan) + ".dat";
				std::ofstream File(path, std::ios::out | std::ios::binary | std::ios::app);
				if (!File){
					std::cout << "Error reading file";
					return;
				}else {
					File.write(reinterpret_cast<const char*> (&save), sizeof(FileSubject));
					File.close();
				}

	}else {
		std::cout << "Code of study plan doesn't exist\n";
	}
}

void StudyPlan::addStudent(){
	FileStudent fileStudent;
	std::string tmp;
	std::ofstream File("Files/Student.dat", std::ios::in | std::ios::binary | std::ios::app);
	std::cout << "Insert the code of the student: ";
	std::cin >> fileStudent.code;
	if (FindStudent(fileStudent.code)){
		std::cout << "Student code already exist\n";
	}else {
		std::cout << "Insert the name of the student: ";
		std::cin.ignore();
		std::getline(std::cin, tmp);
		strcpy_s(fileStudent.name, tmp.size() + 1, tmp.c_str());
		std::cout << "Insert the code of the Study Plan: ";
		std::cin >> fileStudent.codeSP;
		fileStudent.subjectAproved = 0;
		fileStudent.averageAcademic = 0;
		if (FindStudyPlan(fileStudent.codeSP)) {
			if (!File) {
				std::cout << "Error creating file\n";
				return;
			}else {
				File.write(reinterpret_cast<const char*>(&fileStudent), sizeof(FileStudent));
				File.close();
			}
		}else {
			std::cout << "Study Plan doesn't exist\n";
		}
	}
}

void StudyPlan::LoadStudyPlan(int code){
	FileStudyPlan studyPlan;
	FileSubject subject;
	int count = 1;
	int QSon;
	if (FindStudyPlan(code)){
		std::string path = std::to_string(code) + ".dat";
		std::ifstream File("Files/"+ path, std::ios::in | std::ios::binary);
		if (!File) {
			std::cout << "Error loading the Study Plan\n";
			return;
		}else{
			File.seekg(sizeof(studyPlan), std::ios::beg);
			File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
			while (!File.eof()) {
				char codeF[7];
				strcpy_s(codeF, sizeof(subject.code), subject.code);
				QSon = subject.quantitySon;
				Tree(subject, subject.ListFather[0]);//agrego al padre  
				if (QSon > 0) {
					char tmpS[4][7];
					for(int i = 0; i < subject.quantitySon; i++) {
						strcpy_s(tmpS[i], sizeof(subject.ListSon[i]), subject.ListSon[i]);//copio los hijos porque los tengo que agregar
					}
					for(int i = 0; i < subject.quantitySon; i++) {
						LoadSon(path, tmpS[i], codeF);
					}
				}
				File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
			}
			File.close();
		}
	}else{
		std::cout << "Study Plan does not exist!";
	}
}

void StudyPlan::LoadSon(std::string path, char* codeS, char* codeF) {
	FileStudyPlan studyPlan;
	FileSubject subject; 
	std::ifstream File("Files/" + path, std::ios::in | std::ios::binary);
if (!File){
	std::cout <<"Error loading the Study Plan\n";
	return;
}else {
	File.seekg(sizeof(FileStudyPlan), std::ios::beg);
	File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
	while (!File.eof()) {
		if (strcmp(subject.code, codeS) == 0) {
			Tree(subject, codeF);
		}
		File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
	}
	File.close();
}
}

void StudyPlan::Tree(FileSubject subject, char* codeF){
	//start new node
	Subject* newS = new Subject(subject.code, subject.name, subject.UV_requirement, 
	subject.UV, 0, subject.quantityFather);
	//end new node
	if (isEmpty()){
		root = newS;
		return;
	}
	if (FindDad(subject.code) == 0){
		Subject* father = FindDad(codeF);
		if (father!= 0) {
			Subject** tmp = new Subject * [father->quantitySon + 1];
			for (int i = 0; i < father->quantitySon; i++)
			{
				tmp[i] = father->son[i];
			}
			tmp[father->quantitySon] = newS;
			if (father->quantitySon != 0) {
				delete father->son;
			}
			father->quantitySon++;
			father->son = tmp;
		}
	}
}

Subject* StudyPlan::FindDad(char* a) {
	return FindDadRec(root, a);
}

Subject* StudyPlan::FindDadRec(Subject* _root, char* code) {
	if (_root == 0) {
		return 0;
	}
	if (strcmp(_root->code, code) == 0){
		return _root;
	}
	for (int i = 0; i < _root->quantitySon; i++)
	{
		Subject* tmp = FindDadRec(_root->son[i], code);
		if (tmp != 0){
			return tmp;
		}
	}
	return 0;
}

bool StudyPlan::FindStudyPlan(int code) {
	std::string name = std::to_string(code) + ".dat";
	char* nameN = new char[name.size() + 1];
	strcpy_s(nameN, name.size() + 1, name.c_str());
	DIR* dirp = opendir("Files/");
	struct dirent* dp;
	while ((dp = readdir(dirp)) != NULL) {
		if (strcmp(dp->d_name, nameN) == 0) {
			return true;
		}
	}
	closedir(dirp);
	return false;
}

void StudyPlan::OpenEnrollment(int countSP){
	int code=0;
	int codeSP;
	int count = 0;
	int number=0;
	int Period=0;
	int Year=0;
	bool finish = false;
	std::string Path;
	StudentHistory add;
	FileStudent student;
	int cpyC = 0;
	std::cout << "Can only enroll a maximum of 3 classes.\n";
	std::cout << "Write the code of the student: ";
	std::cin >> code;
	if (FindStudent(code)){
	std::ifstream FileI("Files/Student.dat", std::ios::in | std::ios::binary);
	if (!FileI) {
		std::cout << "Error opening the file";
	}else {
		FileI.seekg(0, std::ios::beg);
		FileI.read(reinterpret_cast<char*>(&student), sizeof(FileStudent));
		while (!FileI.eof()) {
			if (code == student.code){
				codeSP = student.codeSP;//necesito el codigo del plan de estudio
			}
			FileI.read(reinterpret_cast<char*>(&student), sizeof(FileStudent));
		}
		FileI.close();
	}//cargo el plan de estudio
		std::cout << "Write the year:";
		std::cin >> Year;
		std::cout << "Write the period of the year: ";
		std::cin >> Period;
		Path = std::to_string(code) + ".dat";//creo el archivo con el codigo de estudiante
		std::ofstream File("Files/"+Path,std::ios::out | std::ios::binary | std::ios::app);
		if (!File) {
			std::cout << "Error reading the file\n";
		}else {
			int size = getSize(Path);
			if (size == 0){
				LoadStudyPlan(codeSP);
				Available();
			}else{
				CheckClasses(Path, codeSP);
			}
			while (!finish) {
				if (count == 3) {
					std::cout << "You are in the limit\n";
					finish = true;
				}else {
					PrintAvailable();
					std::cout << "Year " << Year << " Period" << Period << "\n";
					std::cout << "Write \"0\" to finish the enrollment\n";
					std::cout << "Write the number of the class: ";
					std::cin >> number;
					if (number > Counting) {
						std::cout << "Please write the correct number\n";
					}else if (number == 0) {
						finish = true;
					} else{
					     	FileSubject Extract = ExtractClass(Classes[number - 1]->code, codeSP);
							strcpy_s(add.codeC, sizeof(Extract.code), Extract.code);
							strcpy_s(add.name, sizeof(Extract.name), Extract.name);
							add.UV = Extract.UV;
							add.UV_requirement = Extract.UV_requirement;
							add.grade = 0;
							add.status = false;
							add.period = Period;
							add.year = Year;
							File.write(reinterpret_cast<const char*>(&add), sizeof(StudentHistory));
							count++;
					}
				}
			}	
			File.close();
	}
	}else{
		std::cout <<"Student code does not exist\n";
	}
	if (Classes != 0) {
		for (int i = 0; i < 10; i++)
		{
			if (Classes[i] != 0) {
				delete Classes[i];
			}
		}
		Counting = 0;
	}
}

FileSubject StudyPlan::ExtractClass(char* code, int codeSP) {
	std::string Path;
	Path = std::to_string(codeSP) + ".dat";
	FileStudyPlan SP;
	FileSubject subject;
	std::ifstream File("Files/"+Path, std::ios::in | std::ios::binary);
	if (!File) {
		std::cout << "Error open file";
	}else {
		File.seekg(sizeof(FileStudyPlan), std::ios::beg);
		File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
		while (!File.eof()) {
			if (strcmp(subject.code,code)==0) {
				return subject;
			}
			File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
		}
		File.close();
	}
}

void StudyPlan::CloseEnrollment() {
	int period;
	int year;
	int code;
	double gradeN;
	int pos=0;
	std::string Path;
	StudentHistory history;
	std::cout << "Write the year: ";
	std::cin >> year;
	std::cout << "Write the period: ";
	std::cin >> period;
	std::cout << "Write the code of the student: ";
	std::cin >>code;
	if (FindStudent(code)) {
		Path = std::to_string(code) + ".dat";
		std::fstream File("Files/" + Path, std::ios::in |std::ios::out |std::ios::binary);
		if (!File) {
			std::cout << "Error reading the File\n";
		}
		else {
			File.seekg(0, std::ios::beg);
			File.read(reinterpret_cast<char*>(&history), sizeof(StudentHistory));
			while (!File.eof()) {
				if (history.period == period && history.year == year) {
					std::cout << "Code: " << history.codeC << "\n";
					std::cout << "Class: " << history.name << "\n";
					std::cout << "Write the grade of the class: ";
					std::cin >> gradeN;
					history.grade = gradeN;
					if (gradeN >= 60) {
						history.status = true;
					}else {
						history.status = false;
					}
					File.seekp(pos * sizeof(StudentHistory), std::ios::beg);
					File.write(reinterpret_cast<const char*>(&history), sizeof(StudentHistory));
				}
				pos++;
				File.seekg(pos* sizeof(StudentHistory), std::ios::beg);
				File.read(reinterpret_cast<char*>(&history), sizeof(StudentHistory));
			}
			File.close();
		}
	}else {
		std::cout << "Student code does not exist";
	}
}

bool StudyPlan::FindStudent(int code){
	FileStudent fileStudent;
	std::ifstream File("Files/Student.dat", std::ios::in | std::ios::binary);
if(!File) {
	std::cout << "Error open file";
	return false;
}
File.seekg(0, std::ios::beg);
File.read(reinterpret_cast<char*>( &fileStudent), sizeof(FileStudent));
while (!File.eof()) {
	if (fileStudent.code == code) {
		return true;
	}
	File.read(reinterpret_cast<char*>(&fileStudent), sizeof(FileStudent));
 }
File.close();
return false;
}

void StudyPlan::Available() {//cuando el archivo esta vacio
	AvailableRec(root);
}

void StudyPlan::AvailableRec(Subject* _root) {
	if (_root == 0) {
		return;
	}
	if (_root->quantityFather==0 && strcmp(_root->code,"000000")!=0){
		Classes[Counting] = _root;
		Counting++;
	}
	for (int i = 0; i < _root->quantitySon; i++)
	{
		AvailableRec(_root->son[i]);
	}
}

void StudyPlan::CheckClasses(std::string Path, int codeSP) {
	StudentHistory history;
	std::ifstream File("Files/" + Path, std::ios::in | std::ios::binary);
	if (!File){
		std::cout << "Error reading File\n";
	}else {
		File.seekg(0, std::ios::beg);
		File.read(reinterpret_cast<char*>(&history), sizeof(StudentHistory));
		while (!File.eof()){
			if (!IfExist(history.codeC)){
					AvailableF(codeSP, history.codeC, history.status);
			}
			File.read(reinterpret_cast<char*>(&history), sizeof(StudentHistory));
		}
		File.close();
	}	
}

void StudyPlan::AvailableF(int codeSP,char* code, bool Aprove) {//cuando el archivo no esta vacio
	FileStudyPlan studyPlan;
	FileSubject subject;
	int count = 1;
	int QSon;
	if (FindStudyPlan(codeSP)) {
		std::string path = std::to_string(codeSP) + ".dat";
		std::ifstream File("Files/" + path, std::ios::in | std::ios::binary);
		if (!File) {
			std::cout << "Error loading the Study Plan\n";
			return;
		}
		else {
			File.seekg(sizeof(studyPlan), std::ios::beg);
			File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
			while (!File.eof()) {
				QSon = subject.quantitySon;
				if (strcmp(subject.code, code) == 0 && !Aprove){
					Subject* tmp = new Subject(subject.code, subject.name,subject.UV,subject.UV_requirement, subject.quantitySon, subject.quantityFather);
					Classes[Counting] = tmp;
					Counting++;
				}else {
					if (strcmp(subject.code, code) == 0 && Aprove) {
						if (QSon > 0) {
							char tmpS[4][7];
							for (int i = 0; i < subject.quantitySon; i++) {
								strcpy_s(tmpS[i], sizeof(subject.ListSon[i]), subject.ListSon[i]);//copio los hijos porque los tengo que agregar
								AvailableFRec(codeSP,tmpS[i]);
							}
						}
					}
				}
				File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
			}
			File.close();
		}
	}else {
		std::cout << "Study Plan does not exist!";
	}
}

	void StudyPlan::AvailableFRec(int codeSP, char* code) {
		FileStudyPlan studyPlan;
		FileSubject subject;
		int count = 1;
		int QSon;
		if (FindStudyPlan(codeSP)) {
			std::string path = std::to_string(codeSP) + ".dat";
			std::ifstream File("Files/" + path, std::ios::in | std::ios::binary);
			if (!File) {
				std::cout << "Error loading the Study Plan\n";
				return;
			}
			else {
				File.seekg(sizeof(studyPlan), std::ios::beg);
				File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
				while (!File.eof()){
					if (strcmp(subject.code,code)==0){
						Subject* tmp = new Subject(subject.code, subject.name, subject.UV, subject.UV_requirement, subject.quantitySon, subject.quantityFather);
						Classes[Counting] = tmp;
				   	Counting++;
				}
					File.read(reinterpret_cast<char*>(&subject), sizeof(FileSubject));
				}
				File.close();
			}
		}
	}


bool StudyPlan::IfExist(char* code) {
	if (Classes == 0) {
		return false;
	}
	for (int i = 0; i < Counting; i++){
		if (strcmp(Classes[i]->code, code) == 0) {
			return true;
		}
	}
	return false;
}

void StudyPlan::History(){
	int code = 0;
	int pos = 0;
	int counting = 0;
	int countA = 0;
	double average;
	double grade = 0;
	std::string Path;
	StudentHistory history;
	std::cout << "Write the code of the student: ";
	std::cin >> code;
	if (FindStudent(code)) {
		Path = std::to_string(code) + ".dat";
		std::ifstream File("Files/" + Path, std::ios::in | std::ios::binary);
		if (!File) {
			std::cout << "Error opening the file\n";
		}else {
			File.seekg(0, std::ios::beg);
			File.read(reinterpret_cast<char*>(&history), sizeof(StudentHistory));
			while (!File.eof()) {
				if (history.grade > 0) {
					std::cout << "Code: " << history.codeC << " Name: " << history.name << " Period: " <<
						history.period << " Year: " << history.year << "UV: " << history.UV << " Grade: " <<
						history.grade;
					if (history.status) {
						std::cout << " \"Passed\"\n";
						countA++;
					}else {
						std::cout << " \"Didn't pass\" :(\n";
					}
					grade += history.grade;
					counting++;
				}
				File.read(reinterpret_cast<char*>(&history), sizeof(StudentHistory));
			}
			File.close();
			average = grade / counting;
			std::cout << "The academic average is: " << average<<"\n";
			std::cout << "The quantity of classes aproved is: " << countA<<"\n";
			UpdateGrade(code, average, counting);
		}
	}else {
		std::cout << "Code does not exist\n";
	}
}

void StudyPlan::UpdateGrade(int code, double average, int countA) {
	int pos=0;
	std::fstream File("Files/Student.dat", std::ios::in | std::ios::out | std::ios::binary);
	FileStudent student;
	if (!File) {
		std::cout << "Error opening the file\n";
	}else {
		File.seekg(0, std::ios::beg);
		File.read(reinterpret_cast<char*>(&student), sizeof(FileStudent));
		while (!File.eof()) {
			if (student.code == code) {
				student.averageAcademic = average;
				student.subjectAproved = countA;
				File.seekp(pos * sizeof(FileStudent), std::ios::beg);
				File.write(reinterpret_cast<const char*>(&student), sizeof(FileStudent));
				return;
			}
			pos++;
			File.read(reinterpret_cast<char*>(&student), sizeof(FileStudent));
		}
		File.close();
	}
}

	void StudyPlan::Print() {
		PrintRec(root);
	}


	void StudyPlan::PrintRec(Subject * _root) {
		if (_root == 0) {
			return;
		}
		if (strcmp(_root->code, "000000") != 0) {
			std::cout << "Subject { code: " << _root->code <<
				", name: " << _root->name << " UV: " << _root->UV << ", UV requirements: "
				<< ", Quantity of Son: " << _root->quantitySon << ", Quantity of father: "
				<< _root->quantityFather << " } ";
				std::cout << "\n";
		}
			for (int i = 0; i < _root->quantitySon; i++)
			{
				PrintRec(_root->son[i]);
			}
}

	void StudyPlan::PrintAvailable(){
		if (Counting == 0) {
			return;
		}
		for (int i = 0; i < Counting; i++)
		{
			std::cout << i + 1 << " Code: " << Classes[i]->code << " Class: " << Classes[i]->name <<
				" UV: " << Classes[i]->UV << " UV requirement: " << Classes[i]->UV_requirement;
			std::cout << "\n";
		}
	}

	int StudyPlan::getSize(std::string Path) {
		int size;
		std::ifstream File("Files/" + Path, std::ios::in | std::ios::binary);
		if (!File) {
			std::cout<< "Can't get the size of the file\n";
		}else {
			File.seekg(0, std::ios::end);
			size = File.tellg();
			return size;
		}
		return -1;
	}

	void StudyPlan::WatchPensul(int countSP) {
		int code;
		std::cout << "Write the code of the study plan: ";
		std::cin >> code;
	//	if (countSP == 0) {
			LoadStudyPlan(code);
		//}
		Print();
	}