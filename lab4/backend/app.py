import streamlit as st
import pandas as pd

from UI.doctor.utils import *
from UI.patient.utils import *
from UI.receptionist.utils import *
from UI.login.utils import *
from UI.init.utils import *

st.set_page_config(
    page_title="Hospital Management System",
    layout="wide",
    initial_sidebar_state="expanded",
)



def main():
    
    initialize_data()

    if not st.session_state.get('logged_in', False):
        choice = st.radio("Select Option", ["Login", "New Patient Registration", "New Doctor Registrations", "New Receptionist Registration"])

        if choice == "Login":
            login_page()  
        elif choice == "New Patient Registration":
            patient_self_registration()  
        elif choice == "New Doctor Registrations":
            doctor_self_registration()
        elif choice == "New Receptionist Registration":
            receptionist_self_registration()

        return 

    st.sidebar.title(f"{st.session_state['role']} Portal")
    st.sidebar.write(f"Welcome, {st.session_state['username']}!")
    st.sidebar.markdown("---")
    
    role_pages = {
        "Receptionist": ["Dashboard", "Patient Registration", "Manage Appointments", "Process Billing"],
        "Doctor": ["My Appointments", "Patient Examination"],
        "Patient": ["My Appointments", "My Bills"]
    }
    
    pages = role_pages.get(st.session_state['role'], [])
    page = st.sidebar.radio("Select a Page", pages)

    if st.sidebar.button("Logout"):
        st.session_state['logged_in'] = False
        st.session_state.pop('role', None)
        st.session_state.pop('username', None)
        st.session_state.pop('associated_id', None)
        st.rerun()

    if st.session_state['role'] == 'Receptionist':
        if page == "Dashboard": dashboard_page()
        elif page == "Patient Registration": patient_registration_page()
        elif page == "Manage Appointments": manage_appointments_page()
        elif page == "Process Billing": process_billing_page()
    elif st.session_state['role'] == 'Doctor':
        if page == "My Appointments": view_doctor_appointments()
        elif page == "Patient Examination": patient_examination_page()
    elif st.session_state['role'] == 'Patient':
        if page == "My Appointments": view_patient_appointments()
        elif page == "My Bills": view_patient_bills()




if __name__ == "__main__":
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

    main()

