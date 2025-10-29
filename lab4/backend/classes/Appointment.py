from __init__ import MOD
import time


class Appointment:
    """
    Appointment Class:
        ID -> INT
        Date -> DATE
        Time -> TIME
        Status -> BOOLEAN
        DocID -> INT
        PatientID -> INT

    """
    def __init__(self, doc_id, pat_id):
        self.id = time.time() % MOD
        self.date = -1
        self.time = -1
        self.status = False
        self.doc_id = doc_id
        self.pat_id = pat_id

    def get_id(self):
        """Return Application ID"""
        return self.id

    def get_details(self):
        """
        Retrives Appointment Details
        :return: app_id -> INT
        """
        print("========== Appointment Details ==========")
        print(f"ID: {self.id}")
        print(f"Date: {self.date}")
        print(f"Time: {self.time}")
        print(f"Status: {self.status}")
        print(f"Doctor ID: {self.doc_id}")
        print(f"Patient ID: {self.pat_id}")

        return self.id
