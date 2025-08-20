from graphviz import Digraph

# Create ER diagram
er = Digraph("ER_Diagram", format="png")
er.attr(rankdir="LR", size="8")

# Entities
entities = {
    "Doctor": ["doctorID (PK)", "name", "specialization", "schedule"],
    "Patient": ["patientID (PK)", "name", "age", "gender", "contactInfo", "address"],
    "Prescription": ["prescriptionID (PK)", "date", "medicineList"],
    "Test": ["testID (PK)", "testName", "result"],
    "Diagnosis": ["diagnosisID (PK)", "description"],
    "MedicalProcess": ["processID (PK)", "processName"],
    "Reception": ["staffID (PK)", "name"],
    "Appointment": ["appointmentID (PK)", "date", "time", "status"],
    "HospitalAdmission": ["admissionID (PK)", "date", "facilities"],
    "Billing": ["billID (PK)", "amount", "date"]
}

for entity, attrs in entities.items():
    label = f"{entity}|{{" + "|".join(attrs) + "}}"
    er.node(entity, label=label, shape="record")

# Relationships (edges with labels)
relationships = [
    ("Doctor", "Prescription", "creates"),
    ("Doctor", "Test", "prescribes"),
    ("Doctor", "Diagnosis", "adds"),
    ("Doctor", "MedicalProcess", "schedules"),
    ("Patient", "Prescription", "has"),
    ("Patient", "Test", "undergoes"),
    ("Patient", "Appointment", "books"),
    ("Patient", "HospitalAdmission", "admitted to"),
    ("Appointment", "Reception", "managed by"),
    ("Reception", "Billing", "creates"),
    ("HospitalAdmission", "Billing", "generates")
]

for src, dst, rel in relationships:
    er.edge(src, dst, label=rel)

# Save and render
file_path = "/mnt/data/Hospital_ER_Diagram"
er.render(file_path, cleanup=True)

file_path + ".png"
