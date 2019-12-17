#include "StudyPlan.h"
#include <iostream>

int main() {
	StudyPlan studyPlan;
	bool close = false;
	int option=0;
	int code;//eliminarla
	int countSP = 0;
	do {
		std::cout << "Enrollment System\n";
		std::cout << "1. Create Study Plan\n";
		std::cout << "2. Create Subject\n";
		std::cout << "3. Add student\n";
		std::cout << "4. Start class enrollment\n";
		std::cout << "5. Close class enrollment\n";
		std::cout << "6. Academic information\n";
		std::cout << "7. Watch Pensul\n";
		std::cout << "8. Close program\n";
		std::cout << "Choose an option: ";
		std::cin >> option;
		switch (option)
		{
		case 1:
			studyPlan.CreatePlan();
			break;

		case 2:
			studyPlan.CreateSubject();
			break;

		case 3:
			studyPlan.addStudent();
			break;

		case 4:
			studyPlan.OpenEnrollment(countSP);
			break;
		case 5:
			studyPlan.CloseEnrollment();
			break;

		case 6:
			studyPlan.History();
			break;

		case 7:
			studyPlan.WatchPensul(countSP);

			break;

		case 8:
			close = true;
			break;
		default:
			break;
		}
		
	} while (!close);
}
