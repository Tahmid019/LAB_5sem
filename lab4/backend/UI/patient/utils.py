import streamlit as st
import pandas as pd
from database import get_connection

def view_patient_appointments():
    st.title("My Appointments")
    patient_id = st.session_state['associated_id']

    conn = get_connection()
    if conn is None:
        st.error("Cannot connect to the database.")
        return

    query = "SELECT * FROM appointments WHERE patient_id = %s"
    patient_appointments = pd.read_sql(query, conn, params=(patient_id,))
    conn.close()

    if patient_appointments.empty:
        st.info("You have no appointments scheduled.")
    else:
        st.dataframe(patient_appointments, use_container_width=True)

def view_patient_bills():
    st.title("My Billing History")
    patient_id = st.session_state['associated_id']

    conn = get_connection()
    if conn is None:
        st.error("Cannot connect to the database.")
        return

    query = "SELECT * FROM bills WHERE patient_id = %s"
    patient_bills = pd.read_sql(query, conn, params=(patient_id,))
    conn.close()

    if patient_bills.empty:
        st.info("You have no bills.")
    else:
        st.dataframe(patient_bills, use_container_width=True)
