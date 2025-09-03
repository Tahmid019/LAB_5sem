class Patient:
    """
    *Patient Class*

    - ID -> INT
    - Name -> VARCHAR(50)
    - Age -> INT
    - Gender -> VARCHAR(10)
    - ContactInfo -> INT
    - Address -> VARCHAR(100)
    - DocID -> INT
    - PresID -> INT
    - TestID -> INT
    """
    def __init__(self, pat_id, name, age, gender, contact_info, address):
        self.id = pat_id
        self.name = name
        self.age = age
        self.gender = gender
        self.contact = contact_info
        self.address = address
        self.doc_id = -1
        self.pres_id = -1
        self.test_id = -1

    def get_details(self):
        """
        *Patient Details*
        :return: Patient ID -> INT
        """
        print("========== Patient ID ==========")
        print(f"ID: {self.id}")
        print(f"Name: {self.name}")
        print(f"Age: {self.age}")
        print(f"Gender: {self.gender}")
        print(f"Contact Information: {self.contact}")
        print(f"Address: {self.address}")

        print(" --- Current status ---")
        print(f"Doctor ID: {self.doc_id}")
        print(f"Prescription ID: {self.doc_id}")
        print(f"Medical Test ID: {self.test_id}")

        return self.id

    def get_doc(self):
        """Returns Doc ID -> INT"""
        return self.doc_id

    def get_prescription(self):
        """Return Prescription ID -> INT"""
        return self.pres_id

    def get_test(self):
        """return Medical test ID -> INT"""
        return self.test_id
