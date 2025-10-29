import time
from __init__ import MOD


class Prescription:
    """
    *Prescription Class*
    - ID: INT
    - date: DATE
    - MedList: VARCHAR(1000)
    - DocID: INT
    - PatID: INT
    - testID: INT
    - DiagID: INT
    - ProcessID: INT
    - BillID: INT
    """
    def __init__(self):
        self.id = time.time() % MOD
        self.date = -1
        self.med_list = ""
        self.doc_id = -1
        self.pat_id = -1
        self.test_id = -1
        self.diag_id = -1
        self.process_id = -1
        self.bill_id = -1

    def get_id(self):
        """Return Prescription ID"""
        return self.id

    def get_billing(self):
        """Return Billing ID"""
        return self.bill_id

    def get_details(self):
        """
        *Prescription Details*
        - ID
        - Date
        - Medical List
        - Doctor ID
        - Patient ID
        - Medical Test ID
        - Diagnosis ID
        - Processes ID
        - Billing ID
        :return: ID -> INT
        """
        print("========== Prescription ID ==========")
        print(f"ID: {self.id}")
        print(f"Date: {self.date}")
        print(f"Medical List: {self.med_list}")
        print(f"Doctor ID: {self.doc_id}")
        print(f"Patient ID: {self.pat_id}")
        print(f"Medical test ID: {self.test_id}")
        print(f"Diagnosis ID: {self.diag_id}")
        print(f"Processes ID: {self.process_id}")
        print(f"Billing ID: {self.bill_id}")

        return self.id

    def upd_pres(self):
        pass
