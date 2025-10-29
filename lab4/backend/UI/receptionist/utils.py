import streamlit as st
import datetime
import pandas as pd

def dashboard_page():
    st.title("Hospital Management Dashboard")
    st.markdown("Welcome to the central management system for our hospital.")
    st.markdown("---")

    col1, col2, col3 = st.columns(3)
    col1.metric("Total Patients", len(st.session_state.patients_df))
    col2.metric("Total Doctors", len(st.session_state.doctors_df))
    col3.metric("Pending Appointments", len(st.session_state.appointments_df[st.session_state.appointments_df['status'] == 'Scheduled']))
    
    st.markdown("---")
    
    st.subheader("Current Doctors")
    st.dataframe(st.session_state.doctors_df, use_container_width=True)

    st.subheader("All Appointments")
    st.dataframe(st.session_state.appointments_df, use_container_width=True)

def patient_registration_page():
    st.title("Manage Patient Registration")
    st.subheader("Register a New Patient")

    with st.form("patient_registration_form", clear_on_submit=True):
        name = st.text_input("Full name")
        age = st.number_input("Age", min_value=0, max_value=120, step=1)
        gender = st.selectbox("Gender", ["Male", "Female", "Other"])
        contact = st.text_input("Contact Number")
        submitted = st.form_submit_button("Register Patient")

        if submitted and name:
            new_id = f"P{1001 + len(st.session_state.patients_df)}"
            new_patient = pd.DataFrame([{'patient_id': new_id, 'name': name, 'Age': age, 'Gender': gender, 'Contact': contact}])
            st.session_state.patients_df = pd.concat([st.session_state.patients_df, new_patient], ignore_index=True)
            st.success(f"Patient {name} registered successfully with ID: {new_id}")
        elif submitted:
            st.error("Please enter the patient's name.")

    st.markdown("---")
    st.subheader("Registered Patients List")
    st.dataframe(st.session_state.patients_df, use_container_width=True)

def manage_appointments_page():
    st.title("Manage Appointments")
    st.subheader("Schedule a New Appointment")

    if st.session_state.patients_df.empty or st.session_state.doctors_df.empty:
        st.warning("Please register patients and add doctors before scheduling appointments.")
        return

    with st.form("appointment_form", clear_on_submit=True):
        patient_id = st.selectbox("Select Patient", options=st.session_state.patients_df['patient_id'], format_func=lambda x: f"{x} - {st.session_state.patients_df[st.session_state.patients_df['patient_id'] == x]['name'].values[0]}")
        doctor_id = st.selectbox("Select Doctor", options=st.session_state.doctors_df['doctor_id'], format_func=lambda x: f"{x} - {st.session_state.doctors_df[st.session_state.doctors_df['doctor_id'] == x]['name'].values[0]}")
        app_date = st.date_input("Appointment Date", min_value=datetime.date.today())
        app_time = st.time_input("Appointment Time")
        submitted = st.form_submit_button("Schedule Appointment")

        if submitted:
            app_id = f"A{101 + len(st.session_state.appointments_df)}"
            new_appointment = pd.DataFrame([{
                'appointment_id': app_id, 
                'patient_id': patient_id, 
                'doctor_id': doctor_id, 
                'Date': app_date, 
                'Time': app_time, 
                'Status': 'Scheduled',
                'ExaminationNotes': ''
            }])
            st.session_state.appointments_df = pd.concat([st.session_state.appointments_df, new_appointment], ignore_index=True)
            st.success(f"Appointment {app_id} scheduled for Patient {patient_id} with Doctor {doctor_id}.")

    st.markdown("---")
    st.subheader("Current Appointments")
    st.dataframe(st.session_state.appointments_df, use_container_width=True)

def process_billing_page():
    st.title("4.0 Process Billing")
    st.subheader("Generate Bill for an Appointment")
    
    completed_appointments = st.session_state.appointments_df[st.session_state.appointments_df['Status'] == 'Completed']
    if completed_appointments.empty:
        st.warning("No completed appointments to bill.")
        return

    appointment_id = st.selectbox("Select Completed Appointment", options=completed_appointments['appointment_id'])
    
    if appointment_id:
        with st.form("billing_form"):
            amount = st.number_input("Billing Amount ($)", min_value=0.0, step=10.0)
            submitted = st.form_submit_button("Generate Bill")

            if submitted:
                patient_id = completed_appointments[completed_appointments['appointment_id'] == appointment_id]['patient_id'].values[0]
                bill_id = f"B{501 + len(st.session_state.billing_df)}"
                new_bill = pd.DataFrame([{
                    'BillID': bill_id,
                    'appointment_id': appointment_id,
                    'patient_id': patient_id,
                    'Amount': amount,
                    'Status': 'Unpaid'
                }])
                st.session_state.billing_df = pd.concat([st.session_state.billing_df, new_bill], ignore_index=True)
                st.session_state.appointments_df.loc[st.session_state.appointments_df['appointment_id'] == appointment_id, 'Status'] = 'Billed'
                st.success(f"Bill {bill_id} generated for appointment {appointment_id}.")
                st.rerun()

    st.markdown("---")
    st.subheader("Billing Records")
    st.dataframe(st.session_state.billing_df, use_container_width=True)
