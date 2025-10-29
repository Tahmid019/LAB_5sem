import mysql.connector
from mysql.connector import Error
import pandas as pd
import streamlit as st

DB_HOST = "localhost"
DB_USER = "root"         
DB_PASSWORD = ""        
DB_NAME = "hospital_db"

def get_connection():
    try:
        conn = mysql.connector.connect(
            host=DB_HOST,
            user=DB_USER,
            password=DB_PASSWORD,
            database=DB_NAME
        )
        return conn
    except Error as e:
        print(f"Error connecting to MySQL: {e}")
        return None
    
def load_data():
    conn = get_connection()
    
    patient_df = pd.read_sql('SELECT * FROM patients', conn)
    doctor_df = pd.read_sql('SELECT * FROM doctors', conn)
    receptionits_df = pd.read_sql('SELECT * FROM receptionists', conn)
    appointments_df = pd.read_sql('SELECT * FROM appointments', conn)
    billing_df = pd.read_sql('SELECT * FROM bills', conn)
    
    conn.close()
    
    if 'patients_df' not in st.session_state:
        st.session_state.patients_df = pd.DataFrame(columns=['patient_id', 'name', 'age', 'gender', 'contact', 'address'])

    if 'doctors_df' not in st.session_state:
        st.session_state.doctors_df = pd.DataFrame(columns=['doctor_id', 'name', 'specialty', 'schedule'])

    if 'appointments_df' not in st.session_state:
        st.session_state.appointments_df = pd.DataFrame(columns=['appointment_id', 'patient_id', 'doctor_id', 'date', 'time', 'status'])

    if 'billing_df' not in st.session_state:
        st.session_state.billing_df = pd.DataFrame(columns=['bill_id', 'appointment_id', 'patient_id', 'amount', 'status'])
        
    if 'receptionsists_df' not in st.session_state:
        st.session_state.receptionists_df = pd.DataFrame(columns=['reception_id', 'name'])
    
    st.session_state.patients_df = patient_df
    st.session_state.doctors_df = doctor_df
    st.session_state.appointments_df = appointments_df
    st.session_state.billing_df = billing_df
    st.session_state.receptionists_df = receptionits_df
    
    

def init_db():
    conn = get_connection()
    if conn is None:
        return

    cursor = conn.cursor()

    # Users table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INT AUTO_INCREMENT PRIMARY KEY,
            username VARCHAR(50) UNIQUE,
            password VARCHAR(255),
            role VARCHAR(20),
            associated_id INT
        )
    ''')

    # Patients table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS patients (
            patient_id INT AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(100),
            age INT,
            gender VARCHAR(10),
            contact VARCHAR(50),
            address VARCHAR(255)
        )
    ''')

    # Doctors table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS doctors (
            doctor_id INT PRIMARY KEY,
            name VARCHAR(100),
            specialty VARCHAR(50),
            schedule VARCHAR(100)
        )
    ''')

    # Appointments table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS appointments (
            appointment_id INT AUTO_INCREMENT PRIMARY KEY,
            patient_id INT,
            doctor_id INT,
            date DATE,
            time TIME,
            status VARCHAR(20) DEFAULT 'Pending',
            FOREIGN KEY (patient_id) REFERENCES patients(patient_id),
            FOREIGN KEY (doctor_id) REFERENCES doctors(doctor_id)
        )
    ''')

    # Billing table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS bills (
            bill_id INT AUTO_INCREMENT PRIMARY KEY,
            appointment_id INT,
            patient_id INT,
            amount DECIMAL(10,2),
            status VARCHAR(20) DEFAULT 'Unpaid',
            FOREIGN KEY (appointment_id) REFERENCES appointments(appointment_id),
            FOREIGN KEY (patient_id) REFERENCES patients(patient_id)
        )
    ''')
    
    # Receptionists table
    cursor.execute('''
        CREATE TABLE IF NOT EXISTS receptionists (
            reception_id INT AUTO_INCREMENT PRIMARY KEY,
            name VARCHAR(100),
            FOREIGN KEY (appointment_id) REFERENCES appointments(appointment_id),
            FOREIGN KEY (bill_id) REFERENCES bills(bill_id)
        )
    ''')

    conn.commit()
    cursor.close()
    conn.close()
