
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
    def __init__(self, app_id, date, time, status, doc_id, pat_id):
        self.app_id = app_id
        self.date = date
        self.time = time
        self.status = status
        self.doc_id = doc_id
        self.pat_id = pat_id

    def get_details(self):
        """
        Retrives Appointment Details
        :return: app_id -> INT
        """
        print("========== Appointment Details ==========")
        print(f"ID: {self.app_id}")
        print(f"Date: {self.date}")
        print(f"Time: {self.time}")
        print(f"Status: {self.status}")
        print(f"Doctor ID: {self.doc_id}")
        print(f"Patient ID: {self.pat_id}")

        return self.app_id
