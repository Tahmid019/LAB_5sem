import streamlit as st
import pandas as pd
from database import get_connection

def view_doctor_appointments():
    st.title("My Scheduled Appointments")
    doctor_id = st.session_state['associated_id']

    conn = get_connection()
    if conn is None:
        st.error("Cannot connect to the database.")
        return

    query = "SELECT * FROM appointments WHERE doctor_id = %s"
    doc_appointments = pd.read_sql(query, conn, params=(doctor_id,))
    conn.close()

    if doc_appointments.empty:
        st.info("You have no appointments scheduled.")
    else:
        st.dataframe(doc_appointments, use_container_width=True)

def patient_examination_page():
    st.title("3.0 Conduct Patient Examination")
    st.subheader("Record Diagnosis and Prescription")
    
    doctor_id = st.session_state['associated_id']

    conn = get_connection()
    if conn is None:
        st.error("Cannot connect to the database.")
        return

    query = """
        SELECT a.*, p.name AS patient_name 
        FROM appointments a
        JOIN patients p ON a.patient_id = p.patient_id
        WHERE a.status = 'Scheduled' AND a.doctor_id = %s
    """
    scheduled_appointments = pd.read_sql(query, conn, params=(doctor_id,))
    
    if scheduled_appointments.empty:
        st.warning("No scheduled appointments available for examination.")
        conn.close()
        return

    appointment_id = st.selectbox(
        "Select Appointment to Examine", 
        options=scheduled_appointments['appointment_id']
    )

    if appointment_id:
        appointment_details = scheduled_appointments[scheduled_appointments['appointment_id'] == appointment_id]
        patient_name = appointment_details['patient_name'].values[0]
        st.write(f"**Patient:** {patient_name} ({appointment_details['patient_id'].values[0]})")

        with st.form("examination_form"):
            notes = st.text_area("Diagnosis & Prescription Notes", height=200)
            submitted = st.form_submit_button("Save Examination Details")

            if submitted and notes:
                cursor = conn.cursor()
                update_query = """
                    UPDATE appointments
                    SET examination_notes=%s, status='Completed'
                    WHERE appointment_id=%s
                """
                cursor.execute(update_query, (notes, appointment_id))
                conn.commit()
                cursor.close()
                st.success(f"Examination details for appointment {appointment_id} have been saved.")
                st.rerun()
            elif submitted:
                st.error("Please enter examination notes before saving.")
    conn.close()
