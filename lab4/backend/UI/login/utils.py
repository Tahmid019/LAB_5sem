import streamlit as st
from database import get_connection

def authenticate(username, password, role):
    conn = get_connection()
    if conn is None:
        st.error("Cannot connect to the database.")
        return False, None
    cursor = conn.cursor(dictionary=True)
    query = """
        SELECT * FROM users 
        WHERE username=%s AND password=%s AND role=%s
    """
    cursor.execute(query, (username, password, role))
    user_record = cursor.fetchone()
    cursor.close()
    conn.close()
    return user_record is not None, user_record

def login_page():
    st.title("Hospital Management System Login")
    st.markdown("---")
    with st.form("login_form"):
        role = st.selectbox("Select Your Role", ["Patient", "Doctor", "Receptionist"])
        username = st.text_input("Username")
        password = st.text_input("Password", type="password")
        submitted = st.form_submit_button("Login")

        if submitted:
            is_authenticated, user_record = authenticate(username, password, role)
            if is_authenticated:
                st.session_state['logged_in'] = True
                st.session_state['role'] = role
                st.session_state['username'] = username
                st.session_state['associated_id'] = user_record['associated_id']
                st.rerun()
            else:
                st.error("Invalid username, password, or role.")

                
                
def patient_self_registration():
    st.title("New Patient Registration")

    name = st.text_input("Full Name")
    age = st.number_input("Age", min_value=0)
    gender = st.selectbox("Gender", ["Male", "Female", "Other"])
    contact = st.text_input("Contact Number")
    address = st.text_area("Address")
    username = st.text_input("Create Username")
    password = st.text_input("Create Password", type="password")

    if st.button("Register"):
        if not (name and age and gender and contact and address and username and password):
            st.warning("Please fill out all fields.")
            return

        conn = get_connection()
        if conn is None:
            st.error("Cannot connect to the database.")
            return

        cursor = conn.cursor()

        try:
            cursor.execute("""
                INSERT INTO patients (name, age, gender, contact, address)
                VALUES (%s, %s, %s, %s, %s)
            """, (name, age, gender, contact, address))
            
            patient_id = cursor.lastrowid

            cursor.execute("""
                INSERT INTO users (username, password, role, associated_id)
                VALUES (%s, %s, %s, %s)
            """, (username, password, 'Patient', patient_id))

            conn.commit()
            st.success(f"Registration successful! Your Patient ID is {patient_id}. You can now log in.")
        except Exception as e:
            conn.rollback()
            if "Duplicate entry" in str(e):
                st.error("Username already exists. Please choose another.")
            else:
                st.error(f"Error during registration: {e}")
        finally:
            cursor.close()
            conn.close()
            
            
def doctor_self_registration():
    st.title("New Doctor Registration")

    name = st.text_input("Full Name")
    specialization = st.text_input("Specialization")
    schedule = st.text_input("Schedule")
    username = st.text_input("Create Username")
    password = st.text_input("Create Password", type="password")
    

    if st.button("Register"):
        if not (name and specialization and schedule):
            if not name:
                st.warning("Please fill out name.")
            if not specialization:
                st.warning("Please fill out specialization.")
            if not schedule:
                st.warning("Please fill out Schedule")
            
            st.warning("Please fill out all fields.")
            return

        conn = get_connection()
        if conn is None:
            st.error("Cannot connect to the database.")
            return

        cursor = conn.cursor()

        try:
            cursor.execute("""
                INSERT INTO doctors (name, specialty, schedule)
                VALUES (%s, %s, %s)
            """, (name, specialization, schedule))
            
            doctor_id = cursor.lastrowid


            cursor.execute("""
                INSERT INTO users (username, password, role, associated_id)
                VALUES (%s, %s, %s, %s)
            """, (username, password, 'Doctor', doctor_id))


            conn.commit()
            st.success(f"Registration successful! Your Doctor ID is {doctor_id}. You can now log in.")
        except Exception as e:
            conn.rollback()
            if "Duplicate entry" in str(e):
                st.error("Username already exists. Please choose another.")
            else:
                st.error(f"Error during registration: {e}")
        finally:
            cursor.close()
            conn.close()
            
def receptionist_self_registration():
    st.title("New Recipent Registration")

    name = st.text_input("Full Name")
    username = st.text_input("Create Username")
    password = st.text_input("Create Password", type="password")
    

    if st.button("Register"):
        if not (name):
            if not name:
                st.warning("Please fill out name.")
            
            st.warning("Please fill out all fields.")
            return

        conn = get_connection()
        if conn is None:
            st.error("Cannot connect to the database.")
            return

        cursor = conn.cursor()

        try:
            cursor.execute("""
                INSERT INTO receptionists (name)
                VALUES (%s)
            """, (name,))
            
            staff_id = cursor.lastrowid


            cursor.execute("""
                INSERT INTO users (username, password, role, associated_id)
                VALUES (%s, %s, %s, %s)
            """, (username, password, 'Receptionist', staff_id))


            conn.commit()
            st.success(f"Registration successful! Your Doctor ID is {staff_id}. You can now log in.")
        except Exception as e:
            conn.rollback()
            if "Duplicate entry" in str(e):
                st.error("Username already exists. Please choose another.")
            else:
                st.error(f"Error during registration: {e}")
        finally:
            cursor.close()
            conn.close()


