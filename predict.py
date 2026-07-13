import joblib

# Load the trained model
model = joblib.load("model.pkl")

print("=================================")
print(" SMART BMS AI PREDICTION SYSTEM")
print("=================================")

# Get input from user
voltage = float(input("Enter Battery Voltage (V): "))
current = float(input("Enter Battery Current (A): "))
temperature = float(input("Enter Temperature (°C): "))
soc = float(input("Enter Battery SoC (%): "))

# Predict battery condition
prediction = model.predict([[voltage, current, temperature, soc]])

print("\n==============================")
print("Battery Status:", prediction[0])
print("==============================")