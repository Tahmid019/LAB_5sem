from Appointment import Appointment
import time
from __init__ import MOD


class Doctor:
    """
    *Doctor Class for Storing Details*

    - ID -> INT
    - Name -> VARCHAR(50)
    - Specialization -> VARCHAR(50)
    - Schedule -> DATETIME
    - PatientID -> INT
    """
    def __init__(self, name, specialization, schedule):
        self.id = time.time() % MOD
        self.name = name
        self.specialization = specialization
        self.schedule = schedule
        self.pat_id = -1

        self.app = None

    def get_id(self):
        """Return Doc ID"""
        return self.id

    def get_details(self):
        """
        Details of Doctor:
            ID, Name, Specialization, Schedule
        :return: int->ID
        """
        print("========== Doctor Details ==========")
        print(f"ID: {self.id}")
        print(f"Name: {self.name}")
        print(f"Specialization: {self.specialization}")
        print(f"Schedule: {self.schedule}")

        return self.id

    def get_patient(self):
        """
        Get Patient ID
        :return: int->pat_id
        """

        print("========== Patients ==========")
        print(f"Patient ID: {self.pat_id}")
        print("--------------------")

        return self.pat_id

    def set_appointment(self, new_pat, new_date, new_time):
        """
        Schedules Appointments with the Patient
        :return: Appointment Object
        """
        # Schedule Checking
        random_id = time.clock() % MOD
        new_doc = self.id
        new_app = Appointment(random_id, new_date, new_time, False, new_doc, -1)

        if self.pat_id == -1:
            self.pat_id = new_pat
            new_app.pat_id = new_pat
            new_app.status = True

        if not new_app.status:
            print("[!] Doctor Schedule Occupied.")
        else:
            new_app.get_details()

        self.app = new_app
        return new_app

    def get_app(self):
        """
        Current Ongoing App.
        :return: App. Object
        """
        return self.app










