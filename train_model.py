import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
import joblib

# Load the dataset
data = pd.read_csv("battery_data.csv")

# Select input features
X = data[["Voltage", "Current", "Temperature", "SoC"]]

# Select output label
y = data["Status"]

# Split data into training and testing
X_train, X_test, y_train, y_test = train_test_split(
    X,
    y,
    test_size=0.2,
    random_state=42
)

# Create the AI model
model = RandomForestClassifier(n_estimators=100, random_state=42)

# Train the model
model.fit(X_train, y_train)

# Test the model
predictions = model.predict(X_test)

# Display accuracy
accuracy = accuracy_score(y_test, predictions)
print("Model Accuracy:", accuracy)

# Save the trained model
joblib.dump(model, "model.pkl")

print("Model saved successfully as model.pkl")