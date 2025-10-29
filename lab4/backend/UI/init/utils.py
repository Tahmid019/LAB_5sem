import streamlit as st
import pandas as pd
from database import *

def initialize_data():
    init_db()
    conn = get_connection()
    if conn is None:
        st.error("Cannot connect to the database!")
        return

    
    load_data()
    

    conn.close()
