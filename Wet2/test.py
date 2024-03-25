import re
import sys



# Define regular expressions for each line format
patterns = {
    'transaction_failed_account_exists': r"Error \d+: Your transaction failed - account with the same id exists",
    'new_account': r"\d+: New account id is \d+ with password \d+ and initial balance \d+",
    'transaction_failed_incorrect_password': r"Error \d+: Your transaction failed - password for account id \d+ is incorrect",
    'deposit': r"\d+: Account \d+ new balance is \d+ after \d+ \$ was deposited",
    'withdrawal': r"\d+: Account \d+ new balance is \d+ after \d+ \$ was withdrew",
    'balance_check': r"\d+: Account \d+ balance is \d+",
    'account_closure': r"\d+: Account \d+ is now closed. Balance was \d+",
    'transaction_failed_low_balance': r"Error \d+: Your transaction failed - account id \d+ balance is lower than \d+",
    'transfer': r"\d+: Transfer \d+ from account \d+ to account \d+ new account balance is \d+ new target account balance is \d+",
    'transaction_failed_account_does_not_exist': r"Error \d+: Your transaction failed - account id \d+ does not exist",
    'bank_commission': r"Bank: commissions of \d+ % were charged, the bank gained \d+ \$ from account \d+"
}

# Initialize dictionaries to count occurrences
successful_actions = {}
errors = {}

# Open the log file and process each line
with open("log.txt", "r") as file:
    for line in file:
        # Check each pattern to see if the current line matches
        matched = False
        for key, pattern in patterns.items():
            if re.search(pattern, line):
                matched = True
                if key.startswith("transaction_failed"):
                    errors.setdefault(key, 0)
                    errors[key] += 1
                else:
                    successful_actions.setdefault(key, 0)
                    successful_actions[key] += 1
                break
        if not matched:
            print("Error: Unrecognized line:", line.strip())

# Print successful actions and their counts
print("Successful Actions:")
for key, count in successful_actions.items():
    print("{}: {}".format(key, count))

# Print errors and their counts
print("\nFailed Actions:")
for key, count in errors.items():
    print("{}: {}".format(key, count))

#my code from here

# Initialize a counter for the total number of lines in all input files
total_line_count = 0

# Iterate through the file paths passed as command-line arguments
for file_path in sys.argv[1:]:
    # Open the current file and count its lines
    with open(file_path, "r") as file:
        num_lines_in_file = sum(1 for line in file)
        total_line_count += num_lines_in_file

# Print the total number of lines in all input files
print("\nInput files lines:", total_line_count)

with open("log.txt", "r") as file:
    unmatched_lines_count = 0
    for line in file:
        # Check if the current line matches the 'bank_commission' pattern
        if not re.search(patterns['bank_commission'], line):
            unmatched_lines_count += 1

# Print the number of lines not matching the 'bank_commission' pattern
print("Log.txt lines (not matching 'bank_commission'):", unmatched_lines_count)

# Perform checks based on the line counts
if total_line_count == unmatched_lines_count:
    print("\nLines check succeeded!!!")
elif total_line_count > unmatched_lines_count:
    print("\nError: Less lines in log.txt than expected.")
else:
    print("\nError: More lines in log.txt than expected.")