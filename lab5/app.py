import streamlit as st
import sqlite3
import pandas as pd
import hashlib
import time

DB_NAME = "bank_db"

def get_db_connection():
    conn = sqlite3.connect(DB_NAME)
    conn.row_factory = sqlite3.Row
    return conn

def hash_password(password):
    return hashlib.sha256(password.encode()).hexdigest()

def init_db():
    conn = get_db_connection()
    c = conn.cursor()

    c.execute('''
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY,
            username TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            first_name TEXT,
            last_name TEXT
        )
    ''')

    c.execute('''
        CREATE TABLE IF NOT EXISTS accounts (
            account_id INTEGER PRIMARY KEY,
            user_id INTEGER NOT NULL,
            account_type TEXT NOT NULL, -- e.g., 'Checking', 'Savings'
            balance REAL NOT NULL DEFAULT 0.00,
            FOREIGN KEY (user_id) REFERENCES users(user_id)
        )
    ''')

    c.execute('''
        CREATE TABLE IF NOT EXISTS transactions (
            transaction_id INTEGER PRIMARY KEY,
            account_id INTEGER NOT NULL,
            type TEXT NOT NULL, -- 'Deposit' or 'Withdraw'
            amount REAL NOT NULL,
            timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (account_id) REFERENCES accounts(account_id)
        )
    ''')

    c.execute('''
        CREATE TABLE IF NOT EXISTS investments (
            investment_id INTEGER PRIMARY KEY,
            user_id INTEGER NOT NULL,
            name TEXT NOT NULL,
            amount REAL NOT NULL,
            FOREIGN KEY (user_id) REFERENCES users(user_id)
        )
    ''')

    try:
        test_username = 'testuser'
        test_password_hash = hash_password('password123')

        c.execute("INSERT INTO users (username, password_hash, first_name, last_name) VALUES (?, ?, ?, ?)",
                  (test_username, test_password_hash, 'Test', 'User'))

        user_id = c.lastrowid

        c.execute("INSERT INTO accounts (user_id, account_type, balance) VALUES (?, ?, ?)",
                  (user_id, 'Checking', 500.00))
        c.execute("INSERT INTO accounts (user_id, account_type, balance) VALUES (?, ?, ?)",
                  (user_id, 'Savings', 1500.00))

        c.execute("INSERT INTO investments (user_id, name, amount) VALUES (?, ?, ?)",
                  (user_id, 'Tech Index Fund', 2500.00))

        conn.commit()
        st.success(f"Database initialized. Test User: **{test_username}** / **password123**")
    except sqlite3.IntegrityError:
        pass
    except Exception as e:
        st.error(f"Error during DB initialization: {e}")
    finally:
        conn.close()


def authenticate(username, password):
    conn = get_db_connection()
    c = conn.cursor()
    password_hash = hash_password(password)

    c.execute("SELECT * FROM users WHERE username = ? AND password_hash = ?", (username, password_hash))
    user = c.fetchone()
    conn.close()

    if user:
        st.session_state['logged_in'] = True
        st.session_state['user_id'] = user['user_id']
        st.session_state['username'] = user['username']
        st.session_state['name'] = f"{user['first_name']} {user['last_name']}"
        return True
    return False

def get_user_accounts(user_id):
    conn = get_db_connection()
    df = pd.read_sql_query(f"SELECT * FROM accounts WHERE user_id = {user_id}", conn)
    conn.close()
    return df

def get_transaction_history(account_id):
    conn = get_db_connection()
    df = pd.read_sql_query(f"SELECT timestamp, type, amount FROM transactions WHERE account_id = {account_id} ORDER BY timestamp DESC", conn)
    conn.close()
    return df

def get_user_investments(user_id):
    conn = get_db_connection()
    df = pd.read_sql_query(f"SELECT name, amount FROM investments WHERE user_id = {user_id}", conn)
    conn.close()
    return df

def process_transaction(user_id, account_id, amount, transaction_type):
    conn = get_db_connection()
    c = conn.cursor()

    c.execute("SELECT balance, account_id, account_type FROM accounts WHERE account_id = ? AND user_id = ?", (account_id, user_id))
    account = c.fetchone()

    if not account:
        conn.close()
        st.error("Error: Account not found or does not belong to your user.")
        return False

    current_balance = account['balance']

    if transaction_type == 'Withdraw':
        if amount > current_balance:
            conn.close()
            st.error("Transaction failed: Insufficient funds.")
            return False
        new_balance = current_balance - amount
    else:
        new_balance = current_balance + amount

    try:
        c.execute("UPDATE accounts SET balance = ? WHERE account_id = ?", (new_balance, account_id))

        c.execute("INSERT INTO transactions (account_id, type, amount) VALUES (?, ?, ?)",
                  (account_id, transaction_type, amount))

        conn.commit()
        st.success(f"{transaction_type} of ${amount:,.2f} successful! New {account['account_type']} balance: ${new_balance:,.2f}")
        return True
    except Exception as e:
        conn.rollback()
        st.error(f"Transaction failed due to a database error: {e}")
        return False
    finally:
        conn.close()


def show_login_page():
    st.title("Secure Banking Portal")
    st.subheader("Login")

    username = st.text_input("Username")
    password = st.text_input("Password", type="password")

    if st.button("Login"):
        if authenticate(username, password):
            st.success("Login successful!")
            st.rerun()
        else:
            st.error("Invalid username or password.")

    st.markdown("---")
    st.info("The application uses a local SQLite database. Click the button below to ensure the database is set up with a test account.")
    if st.button("Initialize/Reset Database"):
        init_db()


def show_dashboard():
    st.title(f"Welcome, {st.session_state['name']}")

    user_id = st.session_state['user_id']
    accounts_df = get_user_accounts(user_id)

    st.subheader("Account Balances")
    cols = st.columns(len(accounts_df))

    for i, row in accounts_df.iterrows():
        account_type = row['account_type']
        balance = row['balance']
        with cols[i]:
            st.metric(label=f"{account_type} Account", value=f"${balance:,.2f}")

    st.markdown("---")

    st.subheader("Transaction History")

    account_mapping = {f"{row['account_type']} (ID: {row['account_id']})": row['account_id'] for _, row in accounts_df.iterrows()}
    selected_account_name = st.selectbox("Select Account for History:", list(account_mapping.keys()))

    if selected_account_name:
        selected_account_id = account_mapping[selected_account_name]
        history_df = get_transaction_history(selected_account_id)

        if not history_df.empty:
            history_df.columns = ["Timestamp", "Type", "Amount"]
            history_df['Amount'] = history_df['Amount'].apply(lambda x: f"${x:,.2f}")
            st.dataframe(history_df, use_container_width=True, height=200)
        else:
            st.info("No transactions yet for this account.")

    st.markdown("---")

    st.subheader("View Investments")
    investments_df = get_user_investments(user_id)

    if not investments_df.empty:
        investments_df.columns = ["Investment Name", "Amount Invested"]
        investments_df['Amount Invested'] = investments_df['Amount Invested'].apply(lambda x: f"${x:,.2f}")
        st.dataframe(investments_df, use_container_width=True)
    else:
        st.info("No investments recorded.")


def show_transactions_page():
    st.title("Account Transactions")

    user_id = st.session_state['user_id']
    accounts_df = get_user_accounts(user_id)

    account_mapping = {f"{row['account_type']} (Current Balance: ${row['balance']:,.2f})": row['account_id'] for _, row in accounts_df.iterrows()}
    account_options = list(account_mapping.keys())

    if not account_options:
        st.error("You have no accounts! Please contact support.")
        return

    st.subheader("Select Transaction Type")
    col1, col2 = st.columns(2)
    transaction_type = col1.radio("Operation:", ["Deposit", "Withdraw"], horizontal=True)

    st.subheader("Transaction Details")
    selected_account_name = st.selectbox("Select Account:", account_options)

    if selected_account_name:
        selected_account_id = account_mapping[selected_account_name]
        amount = st.number_input(f"Enter {transaction_type} Amount (Must be > 0):", min_value=0.01, format="%.2f")

        if st.button(f"Process {transaction_type}"):
            if amount > 0:
                with st.spinner(f"Processing {transaction_type}..."):
                    time.sleep(1) # delay
                    process_transaction(user_id, selected_account_id, amount, transaction_type)
            else:
                st.error("Amount must be greater than zero.")


def main():
    st.set_page_config(page_title="ATM System", layout="wide")

    if 'logged_in' not in st.session_state:
        st.session_state['logged_in'] = False
        st.session_state['current_page'] = 'Dashboard'

    st.sidebar.image("https://placehold.co/100x50/374151/FFFFFF?text=BANK+APP", use_container_width=True)
    st.sidebar.title("Navigation")


    if st.session_state['logged_in']:
        pages = {
            "Dashboard": show_dashboard,
            "Transactions (Deposit/Withdraw)": show_transactions_page,
        }

        selection = st.sidebar.radio("Go to:", list(pages.keys()))
        pages[selection]()

        st.sidebar.markdown("---")
        if st.sidebar.button("Logout"):
            st.session_state['logged_in'] = False
            st.session_state['user_id'] = None
            st.session_state['username'] = None
            st.session_state['name'] = None
            st.rerun()

    else:
        show_login_page()

init_db()

if __name__ == "__main__":
    main()
