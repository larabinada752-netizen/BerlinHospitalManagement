#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define MAX_DOCTORS 50
#define MAX_APPOINTMENTS 200
#define NAME_LEN 50

typedef struct {
    int id;
    char name[NAME_LEN];
    int age;
    char gender[10];
} Patient;

typedef struct {
    int id;
    char name[NAME_LEN];
    char specialty[NAME_LEN];
} Doctor;

typedef struct {
    int patientId;
    int doctorId;
    char date[20]; // YYYY-MM-DD
} Appointment;

Patient patients[MAX_PATIENTS];
Doctor doctors[MAX_DOCTORS];
Appointment appointments[MAX_APPOINTMENTS];

int patientCount = 0, doctorCount = 0, appointmentCount = 0;

// Function declarations
void addPatient();
void editPatient();
void deletePatient();
void addDoctor();
void editDoctor();
void deleteDoctor();
void scheduleAppointment();
void editAppointment();
void deleteAppointment();
void listPatients();
void listDoctors();
void listAppointments();
void sortAppointmentsByDate();
void saveData();
void loadData();
int findPatientIndexById(int id);
int findDoctorIndexById(int id);
int findAppointmentIndex(int pid, int did, char* date);

// ===================== Functions =====================

void loadData() {
    FILE *fp;

    // Load patients
    fp = fopen("patients.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d;%49[^;];%d;%9[^\n]\n",
                      &patients[patientCount].id,
                      patients[patientCount].name,
                      &patients[patientCount].age,
                      patients[patientCount].gender) == 4) {
            patientCount++;
        }
        fclose(fp);
    }

    // Load doctors
    fp = fopen("doctors.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d;%49[^;];%49[^\n]\n",
                      &doctors[doctorCount].id,
                      doctors[doctorCount].name,
                      doctors[doctorCount].specialty) == 3) {
            doctorCount++;
        }
        fclose(fp);
    }

    // Load appointments
    fp = fopen("appointments.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d;%d;%19[^\n]\n",
                      &appointments[appointmentCount].patientId,
                      &appointments[appointmentCount].doctorId,
                      appointments[appointmentCount].date) == 3) {
            appointmentCount++;
        }
        fclose(fp);
    }
}

void addPatient() {
    if (patientCount >= MAX_PATIENTS) { printf("Cannot add more patients.\n"); return; }

    Patient p;
    p.id = patientCount + 1;

    printf("Enter patient name: "); scanf(" %[^\n]", p.name);
    printf("Enter age: "); scanf("%d", &p.age);
    printf("Enter gender: "); scanf(" %[^\n]", p.gender);

    patients[patientCount++] = p;
    printf("Patient added successfully! ID: %d\n", p.id);
}

void editPatient() {
    int id;
    printf("Enter Patient ID to edit: "); scanf("%d", &id);
    int idx = findPatientIndexById(id);
    if (idx == -1) { printf("Patient not found.\n"); return; }

    printf("Enter new name: "); scanf(" %[^\n]", patients[idx].name);
    printf("Enter new age: "); scanf("%d", &patients[idx].age);
    printf("Enter new gender: "); scanf(" %[^\n]", patients[idx].gender);
    printf("Patient updated successfully.\n");
}

void deletePatient() {
    int id;
    printf("Enter Patient ID to delete: "); scanf("%d", &id);
    int idx = findPatientIndexById(id);
    if (idx == -1) { printf("Patient not found.\n"); return; }

    for (int i = idx; i < patientCount - 1; i++)
        patients[i] = patients[i + 1];
    patientCount--;
    printf("Patient deleted successfully.\n");
}

void addDoctor() {
    if (doctorCount >= MAX_DOCTORS) { printf("Cannot add more doctors.\n"); return; }

    Doctor d;
    d.id = doctorCount + 1;

    printf("Enter doctor name: "); scanf(" %[^\n]", d.name);
    printf("Enter specialty: "); scanf(" %[^\n]", d.specialty);

    doctors[doctorCount++] = d;
    printf("Doctor added successfully! ID: %d\n", d.id);
}

void editDoctor() {
    int id;
    printf("Enter Doctor ID to edit: "); scanf("%d", &id);
    int idx = findDoctorIndexById(id);
    if (idx == -1) { printf("Doctor not found.\n"); return; }

    printf("Enter new name: "); scanf(" %[^\n]", doctors[idx].name);
    printf("Enter new specialty: "); scanf(" %[^\n]", doctors[idx].specialty);
    printf("Doctor updated successfully.\n");
}

void deleteDoctor() {
    int id;
    printf("Enter Doctor ID to delete: "); scanf("%d", &id);
    int idx = findDoctorIndexById(id);
    if (idx == -1) { printf("Doctor not found.\n"); return; }

    for (int i = idx; i < doctorCount - 1; i++)
        doctors[i] = doctors[i + 1];
    doctorCount--;
    printf("Doctor deleted successfully.\n");
}

void scheduleAppointment() {
    if (appointmentCount >= MAX_APPOINTMENTS) { printf("Cannot schedule more appointments.\n"); return; }

    int pid, did;
    char date[20];

    printf("Enter Patient ID: "); scanf("%d", &pid);
    if (findPatientIndexById(pid) == -1) { printf("Patient not found.\n"); return; }

    printf("Enter Doctor ID: "); scanf("%d", &did);
    if (findDoctorIndexById(did) == -1) { printf("Doctor not found.\n"); return; }

    printf("Enter appointment date (YYYY-MM-DD): "); scanf(" %[^\n]", date);

    Appointment a; a.patientId = pid; a.doctorId = did; strcpy(a.date, date);
    appointments[appointmentCount++] = a;

    printf("Appointment scheduled successfully!\n");
}

void editAppointment() {
    int pid, did; char date[20];
    printf("Enter Patient ID of appointment: "); scanf("%d", &pid);
    printf("Enter Doctor ID of appointment: "); scanf("%d", &did);
    printf("Enter date of appointment (YYYY-MM-DD): "); scanf(" %[^\n]", date);

    int idx = findAppointmentIndex(pid, did, date);
    if (idx == -1) { printf("Appointment not found.\n"); return; }

    printf("Enter new Patient ID: "); scanf("%d", &appointments[idx].patientId);
    printf("Enter new Doctor ID: "); scanf("%d", &appointments[idx].doctorId);
    printf("Enter new date (YYYY-MM-DD): "); scanf(" %[^\n]", appointments[idx].date);

    printf("Appointment updated successfully.\n");
}

void deleteAppointment() {
    int pid, did; char date[20];
    printf("Enter Patient ID of appointment: "); scanf("%d", &pid);
    printf("Enter Doctor ID of appointment: "); scanf("%d", &did);
    printf("Enter date of appointment (YYYY-MM-DD): "); scanf(" %[^\n]", date);

    int idx = findAppointmentIndex(pid, did, date);
    if (idx == -1) { printf("Appointment not found.\n"); return; }

    for (int i = idx; i < appointmentCount - 1; i++)
        appointments[i] = appointments[i + 1];
    appointmentCount--;

    printf("Appointment deleted successfully.\n");
}

void listPatients() {
    if (patientCount == 0) { printf("No patients found.\n"); return; }

    printf("\n--- Patients ---\n");
    for (int i = 0; i < patientCount; i++)
        printf("ID: %d | Name: %s | Age: %d | Gender: %s\n",
               patients[i].id, patients[i].name, patients[i].age, patients[i].gender);
}

void listDoctors() {
    if (doctorCount == 0) { printf("No doctors found.\n"); return; }

    printf("\n--- Doctors ---\n");
    for (int i = 0; i < doctorCount; i++)
        printf("ID: %d | Name: %s | Specialty: %s\n",
               doctors[i].id, doctors[i].name, doctors[i].specialty);
}

void sortAppointmentsByDate() {
    for (int i = 0; i < appointmentCount - 1; i++) {
        for (int j = i + 1; j < appointmentCount; j++) {
            if (strcmp(appointments[i].date, appointments[j].date) > 0) {
                Appointment temp = appointments[i];
                appointments[i] = appointments[j];
                appointments[j] = temp;
            }
        }
    }
}

void listAppointments() {
    if (appointmentCount == 0) { printf("No appointments found.\n"); return; }

    sortAppointmentsByDate();

    printf("\n--- Appointments ---\n");
    for (int i = 0; i < appointmentCount; i++) {
        int pid = appointments[i].patientId;
        int did = appointments[i].doctorId;
        printf("Patient: %s (ID %d) | Doctor: %s (ID %d) | Date: %s\n",
               patients[findPatientIndexById(pid)].name, pid,
               doctors[findDoctorIndexById(did)].name, did,
               appointments[i].date);
    }
}

int findPatientIndexById(int id) {
    for (int i = 0; i < patientCount; i++) if (patients[i].id == id) return i;
    return -1;
}

int findDoctorIndexById(int id) {
    for (int i = 0; i < doctorCount; i++) if (doctors[i].id == id) return i;
    return -1;
}

int findAppointmentIndex(int pid, int did, char* date) {
    for (int i = 0; i < appointmentCount; i++)
        if (appointments[i].patientId == pid &&
            appointments[i].doctorId == did &&
            strcmp(appointments[i].date, date) == 0)
            return i;
    return -1;
}

void saveData() {
    FILE *fp;

    // Save patients
    fp = fopen("patients.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < patientCount; i++) {
            fprintf(fp, "%d;%s;%d;%s\n",
                    patients[i].id,
                    patients[i].name,
                    patients[i].age,
                    patients[i].gender);
        }
        fclose(fp);
    }

    // Save doctors
    fp = fopen("doctors.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < doctorCount; i++) {
            fprintf(fp, "%d;%s;%s\n",
                    doctors[i].id,
                    doctors[i].name,
                    doctors[i].specialty);
        }
        fclose(fp);
    }

    // Save appointments
    fp = fopen("appointments.txt", "w");
    if (fp != NULL) {
        for (int i = 0; i < appointmentCount; i++) {
            fprintf(fp, "%d;%d;%s\n",
                    appointments[i].patientId,
                    appointments[i].doctorId,
                    appointments[i].date);
        }
        fclose(fp);
    }

    printf("Data saved successfully.\n");
}

// ===================== Main =====================

int main() {
    loadData();

    int choice;
    while (1) {
        printf("\n==== Berlin Hospital Management ====\n");
        printf("1. Add Patient\n");
        printf("2. Edit Patient\n");
        printf("3. Delete Patient\n");
        printf("4. Add Doctor\n");
        printf("5. Edit Doctor\n");
        printf("6. Delete Doctor\n");
        printf("7. Schedule Appointment\n");
        printf("8. Edit Appointment\n");
        printf("9. Delete Appointment\n");
        printf("10. List Patients\n");
        printf("11. List Doctors\n");
        printf("12. List Appointments\n");
        printf("13. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addPatient(); break;
            case 2: editPatient(); break;
            case 3: deletePatient(); break;
            case 4: addDoctor(); break;
            case 5: editDoctor(); break;
            case 6: deleteDoctor(); break;
            case 7: scheduleAppointment(); break;
            case 8: editAppointment(); break;
            case 9: deleteAppointment(); break;
            case 10: listPatients(); break;
            case 11: listDoctors(); break;
            case 12: listAppointments(); break;
            case 13: saveData(); printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice.\n"); break;
        }
    }
    return 0;
}
