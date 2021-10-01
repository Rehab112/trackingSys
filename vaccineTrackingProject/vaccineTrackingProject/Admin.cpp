#include "Admin.h"
#include <string>
#include <iostream>
using namespace std;

void Admin :: logIn(UsersList* user, QueueArr<int>* list) {
    userList = user;
    waitingList = list;
    string pass;
    cout << "Admin Password: ";
    cin >> pass;
    if (pass != ADMIN_PASSWORD) {
        cout << "Wrong Password! Please Try Again\n";
        logIn(userList, waitingList);
    }
}

Admin::~Admin() {
    userList = nullptr;
    waitingList = nullptr;
}

void Admin::adminOperations() {
    cout << "\n1.Operate on Certain Record\n2.Deliver Vaccine for Users in Queue\n3.Delete all Records\n4.Display all Records\n";
    cout << "5.Display Waiting List\n6.Display Statistics According to the System Users\n";
    cout << "7.Display Statistics According to the Population of Egypt\n8.Log Out\n";
    int choice;
operationFlag:
    cin >> choice;
    cout << endl;
    switch (choice) {
    case 1:
        if (certainRecord() == -1)
            return adminOperations();
        break;
    case 2:
        deliverVaccine();
        break;
    case 3:
        userList->deleteAll();
        waitingList->deleteAll();
        break;
    case 4:
        displayAll();
        break;
    case 5:
        cout << "\nIDs of Users on the Waiting List:\n\n";
        waitingList->read_data();
        break;
    case 6:
        Percentage();
        Percentage(userList->length());
        break;
    case 7:
        Percentage(POPULATION_OF_EGYPT);
        break;
    case 8:
        //logout
        return;
    default:
        cout << "Invalid Choice !\nPlease Enter the number that Refers to Your Choice\n";
        goto operationFlag;
    }
    cout << "\n1.Another Operation\n2.Log Out from Admin Account\n";
check:
    cin >> choice;
    switch (choice) {
    case 1:
        adminOperations();
        break;
    case 2:
        break;
    default:
        cout << "Invalid Choice !\nPlease Enter the number that Refers to Your Choice\n";
        goto check;
    }
}

void Admin::deliverVaccine() {
    if (waitingList->empty()) {
        cout << "No Users in the Waiting List\n";
        return;
    }
    int userInQueueId = waitingList->Front();
    int index = userList->checkId(userInQueueId);
    cout << "User in Queue:\nName: " << userList->At(index).fName << " " << userList->At(index).lName;
    cout << "\nID: " << userInQueueId << endl;

    if (userList->At(index).vaccinated)
        userList->updateVaccinationAt(index, true, false, false);
    else
        userList->updateVaccinationAt(index, true, true, false);
    cout << "User Received Vaccine Successfully\n";
    waitingList->dequeue();
}

void Admin::displayAll() {
    if (userList->length() == 0) {
        cout << "No Records Exist!\n";
        return;
    }
    for (int i = 0; i < userList->length(); i++)
        userList->displayUserAt(i);
}

int Admin::certainRecord() {
    int choice;
    cout << "Enter ID: ";
    int id, index;
    adminIdFlag:
    cin >> id;
    index = userList->checkId(id);
    if (index < 0) {
        cout << "Non Exist ID!\n1.Try Again\n2.Back\n";
        backFlag:
        cin >> choice;
        switch (choice) {
        case 1:
            goto adminIdFlag;
        case 2:
            return -1;
        default:
            cout << "Invalid Choice !\nPlease Enter the number that Refers to Your Choice\n";
            goto backFlag;
        }

    }
    userList->displayUserAt(index);
    menuFlag:
    cout << "1.Delete Record\n2.Back\n";
    deleteFlag:
    cin >> choice;
    switch (choice) {
    case 1:
        // remove it from the waiting list
        waitingList->removeId(id);
        userList->deleteAt(index);
        cout << "Record Has been Deleted Successfully\n";
        break;
    case 2:
        return -1;
    default:
        cout << "Invalid Choice !\nPlease Enter the number that Refers to Your Choice\n";
        goto deleteFlag;
    }
    return 0;
}

void Admin::Percentage() {

    float counter = 0;
    cout << "Users Outside Egypt: ";
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).country != 0) {
            counter++;
        }
    }
    float total = counter / userList->length();
    cout << total * 100 << " %\n\n";

    cout << "Users in Egypt:\n";
    float governPer[10] = { 0 };
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < userList->length(); j++) {
            if (userList->At(j).governorate == i) {
                governPer[i]++;
            }
        }
    }
    for (int i = 0; i < 10; i++) {
        cout << userList->At(0).getGovernorate(0, i) << " : " << governPer[i] / userList->length() * 100 << " % \n";
    }
    cout << endl;

    counter = 0;
    total = 0;
    cout << "Users > 18 Years Old : ";
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).age >= 18) {
            counter++;
        }
    }
    total = counter / userList->length();
    cout << total * 100 << " % \n\n";


    float maleCounter = 0, femaleCounter = 0;
    total = 0;
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).male == 1) {
            maleCounter++;
        }
        else {
            femaleCounter++;
        }
    }
    total = maleCounter / userList->length();
    cout << "Male : " << total * 100 << "%\n";
    total = femaleCounter / userList->length();
    cout << "Female : " << total * 100 << " % \n\n";
}

void Admin::Percentage(double population) {

    float total;
    if (population == POPULATION_OF_EGYPT) {
        //percentage of useres applyed for vaccine generally (all users on the system)
        //according to population of egypt
        cout << "Applied for Vaccine: ";
        total = userList->length() / population;
        cout << total * 100 << " %\n\n";
    }
    //vaccinated generally (one or both doses)
    cout << "Vaccinated Users: ";
    float counter = 0;
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).vaccinated) {
            counter++;
        }
    }
    total = counter / population;
    cout << total * 100 << " %\n";
    //fully vaccinated
    cout << "Fully Vaccinated: ";
    counter = 0;
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).vaccinated && !userList->At(i).oneDose) {
            counter++;
        }
    }
    total = counter / population;
    cout << total * 100 << " %\n";
    //vaccinated (one dose)
    cout << "Vaccinated and Received Only One Dose: ";
    counter = 0;
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).vaccinated && userList->At(i).oneDose) {
            counter++;
        }
    }
    total = counter / population;
    cout << total * 100 << " %\n\n";
    //on waitinglist generraly (applying for first or second dose)
    cout << "Applying for Vaccine (on waiting list): ";
    total = waitingList->length() / population;
    cout << total * 100 << " %\n";
    //unvaccinated (applying for first dose)
    cout << "Unvaccinated: ";
    counter = 0;
    for (int i = 0; i < userList->length(); i++) {
        if (!userList->At(i).vaccinated) {
            counter++;
        }
    }
    total = counter / population;
    cout << total * 100 << " %\n";
    //applying for second dose
    cout << "Applying for Second Dose: ";
    counter = 0;
    for (int i = 0; i < userList->length(); i++) {
        if (userList->At(i).vaccinated && userList->At(i).atWaitingListBool) {
            counter++;
        }
    }
    total = counter / population;
    cout << total * 100 << " %\n\n";
}